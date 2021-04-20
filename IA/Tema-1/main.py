from io import TextIOWrapper
from typing import Callable, List

from utils import filesInFolder, createDirectory, Initialize, getElapsed
from algoritm import SearchAlgorithms
from estimare import Estimations
from state import StateNode, getStartNode

def runAlgorithm(alg: Callable, est: Callable, 
                 nr_sols: int, timeout: float, 
                 fout: TextIOWrapper):
    states: List[StateNode]

    if not getStartNode().validSuccesor():
        print("The start state can not possibly reach the target final state", file=fout)
        return

    states, info = alg(est, nr_sols, timeout)
    if states == []:
        msg = "Did not find solutions, timed out!"
        if getElapsed() < timeout:
            msg = "No solutions, really, nothing found!"
        print(msg, file=fout)
        return 
    
    print(f"Found {len(states)} solutions:\n", file=fout)
    print(f"Found {len(states)} solutions")
    for i, state in enumerate(states):
        print(f" =============================================", file=fout)
        print(f"  Solution {i + 1}:", file=fout)
        print(f"  Steps:", file=fout)
        state.printPath(fout)
        print(f"  More info about specific path:", file=fout)
        print(f"    Time: {info['time'][i]}", file=fout)
        print(f"    Distance (nr of nodes in path): {len(state.getPath())}", file=fout)
        print(f"    Cost of path: {state.cost}", file=fout)
        print(f"    Visited nodes: {info['visited'][i]}", file=fout)
        print(f"    Computed nodes: {info['computed'][i]}\n\n", file=fout)
    print("\n\n")

"""
Punctul de intrare al programului
"""
def main():
    # Asteptam numele folderului cu teste
    folder =  input("\nFolder for input files: ")
    try: 
        files = filesInFolder(folder)
    except:
        raise RuntimeError("Inexisting folder or other problem encountered.")
    
    # Asteptam numele folderului de output
    out_folder = input("Folder for saving results: ")
    try:
        createDirectory(out_folder)
    except:
        raise RuntimeError("Error creating destination folder.")
        
    # Citim numarul de solutii cautate
    try:
        nr_sols = int(input("\nNumber of possible solutions to search \n" + 
                            "(A* Fast/IDA* can only do a single one): "))
        if nr_sols <= 0:
            raise ValueError("Number of solutions should be an integer of value at least 1")
    except Exception as e:
        # print(e)
        raise e
    
    # Alegem estimarea
    print("\nPossible estimations:")
    for id, (name, _) in enumerate(Estimations):
        print(f" {id + 1}: {name}")
    est_nr = int(input("Choose estimation (1/2/3/4): "))
    est_fn = Estimations[est_nr - 1][1]

    # Alegem algoritmul
    print("\nPossible algorithms:")
    for id, (name, _) in enumerate(SearchAlgorithms):
        print(f" {id + 1}: {name}")
    alg_nr = int(input("Choose algorithm (1/2/3/4): "))
    alg_fn = SearchAlgorithms[alg_nr - 1][1]

    # Citim timpul de timeout in secunde
    timeout = float(input("\nTimeout in seconds: "))

    # Pentru fiecare fisier de test, rulam algoritmul
    for file in files:
        out_file = out_folder + "/" + file + ".out"
        in_file = folder + "/" + file
        try:
            print(f"Running test file: {in_file}")
            Initialize(in_file)
            print(f"Finished reading from file: {in_file}")
            with open(out_file, "w") as fd:
                runAlgorithm(alg_fn, est_fn, nr_sols, timeout, fd)
                print(f"   Other info:", file=fd)
                print(f"   Algorithm: {SearchAlgorithms[alg_nr - 1][0]}", file=fd)
                print(f"   Estimation: {Estimations[est_nr - 1][0]}\n", file=fd)

        except Exception as e:
            # print(e.args)
            print(e)
            print("Something bad happen. Read above why.")
            raise e

if __name__ == "__main__":
    main()