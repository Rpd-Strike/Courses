from table import Player, S_DOG
from state import State
from typing import Any, Callable, List

EventType = int
"""
Functia 'Brain', in functie de eventul de joc, schimba starea jocului 
si returneaza un tuplu: Daca s-a schimbat starea jocului (GUI sau chiar mutare), 
si mutarea efectiva, daca s-a efectuat o mutare pe tabla. 
  (In cazul omului, el poate sa schimbe doar GUI state)
"""

def name_player(player: Player) -> str:
    if player == S_DOG:
        return "Dogs"
    return "Rabbit"

def print_time_statistics(think_times: Any, comp_nodes: List[int], is_ai: bool, player: Player):
    """
    Afiseaza statistici despre joc
    """
    print(f"  -->>  Informatii despre mutarile: {name_player(player)}")
    print(f"is it AI?  {'DA' if is_ai else 'NU'}")
    if len(think_times) == 0:
        print("No moves performed!")
        return
    min_t = min(think_times)
    max_t = max(think_times)
    n = len(think_times)
    medie = sum(think_times) / n
    median = think_times[(n - 1) // 2]

    print(f"Numar mutari: {n}")
    print(f"Timp minim: {min_t:.2f} s")
    print(f"Timp maxim: {max_t:.2f} s")
    print(f"Timp mediu: {medie:.2f} s")
    print(f"Timp median: {median:.2f} s")

    if is_ai:
        print(f"Informatii calcul noduri:")
        if len(comp_nodes) == 0:
            print("No moves / no data about computed nodes!")
            return
        min_t = min(comp_nodes)
        max_t = max(comp_nodes)
        n = len(comp_nodes)
        medie = sum(comp_nodes) / n
        median = comp_nodes[(n - 1) // 2]

        print(f"Numar mutari: {n}")
        print(f"Nr. noduri minim: {min_t} noduri")
        print(f"Nr. noduri maxim: {max_t} noduri")
        print(f"Nr. noduri mediu: {medie:.2f} noduri")
        print(f"Nr. noduri median: {median} noduri")

EstimationType = Callable[[State], int]
    