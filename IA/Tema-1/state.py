from io import TextIOWrapper
from typing import Final, List, Tuple

START_Node: 'StateNode' = None
FINAL_Node: 'StateNode' = None

class StateNode:
    """
    Table: Initial table the state is a subtable of
    state: Tuple of list of lines and columns that are active -->> Tuple[List_lines, List_cols]
    move: Tuple of lines of columns deleted from the previous state
    previous: Previous state
    """    
    def __init__(self, table: List[List[int]], 
                       state: Tuple[List[int], List[int]], 
                       move: Tuple[List[int], List[int]],
                       cost: int,
                       previous: 'StateNode'):
        self.table = table
        self.state = state
        self.move = move
        self.cost = cost
        self.previous = previous
    
    
    ## Next states if deleting lines
    def removeLines(self) -> List['StateNode']:
        """
        Genereaza starile succesor daca scoatem linii consecutive
        """
        lines = self.state[0]
        states: List[StateNode] = []
        for i in range(len(lines)):
            for j in range(i + 1, len(lines) + 1):
                new_lines = lines[:i] + lines[j:]
                if len(new_lines) == 0:
                    continue
                states.append(StateNode(self.table, 
                                        (new_lines, self.state[1]),
                                        (lines[i:j], []),
                                        self.cost + len(self.state[1]),
                                        self))
        return states

    def getFrq(self):
        """
        Returneaza vectorul de frecventa pentru literele din stare
        """
        fr = [0 for i in range(26)]
        for i in self.state[0]:
            for j in self.state[1]:
                fr[ord(self.table[i][j]) - ord('a')] += 1
        return fr

    def count_pairs(self, lines: List[int], cols: List[int]) -> int:
        """
        Pentru starea data, dandu-se liniile si coloanele active,
        calculeaza numarul de perechi adiacente diferite din zona decupata
        """
        mat = [[self.table[i][j] for j in cols] for i in lines]
        diff = 0
        for i in range(len(mat)):
            for j in range(len(mat[0])):
                if i + 1 < len(mat) and mat[i][j] != mat[i + 1][j]:
                    diff += 1
                if j + 1 < len(mat[i]) and mat[i][j] != mat[i][j + 1]:
                    diff += 1
        # if lines == [0, 1, 2, 3] and cols == [0, 1]:
        #     print("COUNT PAIRS ON FIRST MOVE: ", diff)
        #     print(mat)
        return diff

    ## Next states if deleting columns
    def removeCols(self) -> List['StateNode']:
        """
        Genereaza starile succesor daca scoatem coloane consecutive
        """
        cols = self.state[1]
        states: List[StateNode] = []
        for i in range(len(cols)):
            for j in range(i + 1, len(cols) + 1):
            # for j in range(i + 1, i + 2):
                new_cols = cols[:i] + cols[j:]
                if len(new_cols) == 0:
                    continue
                states.append(StateNode(self.table, 
                                        (self.state[0], new_cols),
                                        ([], cols[i:j]),
                                        self.cost + j - i + self.count_pairs(self.state[0], cols[i:j]),
                                        self))
        return states

    def validSuccesor(self):
        """
        Functie ce verifica daca o stare poate ajunge la starea finala
        Verificam daca avem cel putin la fel de multe linii si coloane
        Verificam daca avem cel putin caracterele necesare
            pentru a forma starea finala folosind vectori de frecevnta
        """
        lines, cols = len(FINAL_Node.state[0]), len(FINAL_Node.state[1])
        finalFrq = FINAL_Node.getFrq()
        def tooLess(node: StateNode) -> bool:
            return len(node.state[0]) < lines or len(node.state[1]) < cols
        def badFrq(node: StateNode) -> bool:
            frq = node.getFrq()
            for i in range(26):
                if frq[i] < finalFrq[i]:
                    return True
            return False
        
        return not tooLess(self) and not badFrq(self)

    def genSuccesori(self) -> List['StateNode']:
        """
        Genereaza lista de succesori ce au o sansa sa ajunga in starea finala
        """
        candidates = self.removeCols() + self.removeLines()
        return [x for x in candidates if x.validSuccesor()]

    def getPath(self) -> List['StateNode']:
        """
        Returneaza lista nodurilor de la radacina la nodul curent
        """
        rest = []
        if self.previous is not None:
            rest = self.previous.getPath()
        return rest + [self]

    def printPath(self, f: TextIOWrapper):
        """
        Afiseaza drumul catre acest nod de la nodul de start 
        prin streamul f dat ca parametru
        """
        prev_cost = 0
        if self.previous is not None:
            self.previous.printPath(f)
            prev_cost = self.previous.cost
        
        del_str = "Removed " + ("lines:" if len(self.move[0]) > 0 else "columns:")
        for x in self.move[0]:
            del_str += f" {x}"
        for x in self.move[1]:
            del_str += f" {x}"
        print(del_str + f"\nCost: {self.cost}  (+ {self.cost - prev_cost})", file=f)
        for i in self.state[0]:
            for j in self.state[1]:
                print(self.table[i][j], end = "", file=f)
            print("", file=f)
        print("------------------", file=f)

    def isEquivalent(self, oth: 'StateNode') -> bool:
        """
        Verifica daca starea 'oth' este echivalenta cu starea 'self'
        (Pot proveni din matrice initiale diferite, dar pot fi decupate si sa devina echivalente)
        """
        a = [self.table[i][j] for i in self.state[0] for j in self.state[1]]
        b = [oth.table[i][j] for i in oth.state[0] for j in oth.state[1]]
        if len(a) != len(b):
            return False
        if len(a) < 1 or len(b) < 1 or len(a[0]) != len(b[0]):
            return False
        for i in range(len(a)):
            for j in range(len(a[0])):
                if a[i][j] != b[i][j]:
                    return False
        return True

    def isSameState(self, oth: 'StateNode') -> bool:
        """
        Verificam doar cu ajutorul liniilor si coloanelor active salvate daca doua stari sunt echivalente
        """
        def sameElements(a: list, b: list):
            return len([1 for i, j in zip(a, b) if i == j]) == len(a) and len(a) == len(b)
        if self.table != oth.table:
            return False
        a_lin, a_col = self.state
        b_lin, b_col = oth.state
        return sameElements(a_lin, b_lin) and sameElements(a_col, b_col)

    def inPath(self, oth: 'StateNode') -> bool:
        """
        Verificam daca starea 'oth' se afla in drumul de la radacina pana la nodul 'self'
        """
        if self == oth:
            return True
        if self.isSameState(oth):
            return True
        if self.previous is not None:
            return self.previous.inPath(oth)

    def isFinal(self) -> bool:
        """
        Verificam daca starea curenta este echivalenta cu starea finala
        i.e. daca starea este finala
        """
        return self.isEquivalent(FINAL_Node)

    def showState(self):
        """
        Afisam la consola detalii sumare despre stare
        """
        for i in self.state[0]:
            for j in self.state[1]:
                print(self.table[i][j], end="")
            print("")


def readState(f: TextIOWrapper) -> StateNode:
    """
    Citim din fisier o stare
    Presupunem ca urmeaza n linii cu acelasi numar de caractere
    """
    table = []
    line = f.readline().strip()
    while len(line) > 0:
        table.append(line)
        line = f.readline().strip()
    line_lengths = [len(x) for x in table]

    # print("Table: ", table)
    # print("Lengths of  table: ", line_lengths)

    if len(table) == 0:
        raise ValueError("State is missing first line of data!")
    if min(line_lengths) != max(line_lengths):
        raise ValueError("State doesn't have all lines of equal size!")
    return StateNode(
        table, 
        (list(range(len(table))), list(range(len(table[0])))), 
        ([], []), 
        0, 
        None
    )


def parseInput(f: TextIOWrapper):
    """
    Citim starea initiala si finala din fisier
    """
    global START_Node, FINAL_Node
    START_Node = readState(f)
    FINAL_Node = readState(f)



"""
Urmatoarele 2 functii sunt folosite din alte fisiere 
pentru a obtine starea finala si initiala citite din fisiere
"""
def getStartNode() -> StateNode:
    return START_Node

def getFinalNode() -> StateNode:
    return FINAL_Node