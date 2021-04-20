from table import S_DOG, S_RAB
from state import State

"""
O estimare reprezinta un cost de forma:
Score_dogs - Scor_rabbits
Astfel, cainii doresc un scor mare, in timp ce iepurii, un scor mic
"""

BIG = 10 ** 8

def different_dog_lines(state: State) -> int:
    """
    Pe cate linii distincte se gasesc caini
    """
    lines = [x for x, _ in state.table.get_player_pieces(S_DOG)]
    lines = sorted(lines)
    diff = 1
    for i in range(1, len(lines)):
        if lines[i] != lines[i - 1]:
            diff += 1
    return diff

def dog_column_sums(state: State) -> int:
    lines = [x for x, _ in state.table.get_player_pieces(S_DOG)]
    val = 0
    for x in lines:
        val += x
    return val

def rabbit_column(state: State) -> int:
    _, ry = state.table.get_player_pieces(S_RAB)[0]
    return ry

def is_rabbit_middle(state: State) -> int:
    rx, _ = state.table.get_player_pieces(S_RAB)[0]
    return 1 if rx == 1 else 0


def simpleEstimation(state: State) -> int:
    """
    Cainii sunt incentivizati sa se imprastie cat mai mult pe linii distincte
    (deoarece asa baricadam iepurele)
    Iepurele este incentivizat sa se duca cat mai in stanga
    (deoarece asa este in stanga lupilor)
    """
    if state.isFinal():
        if state.rabbits_win():
            return -BIG
        return BIG
    dogs = different_dog_lines(state) * 300
    rabbit = (5 - rabbit_column(state)) * 250
    return dogs - rabbit

def advancedEstimation(state: State) -> int:
    """
    Pe langa estimare simpla, 
    incentivizam cainii sa fie cat mai in dreapta
    (pentru a bloca iepurele)
    Iar iepurele sa fie pe mijloc
    (pentru a avea cat mai multe optiuni)
    """
    dog_score = dog_column_sums(state) * 5
    rab_score = is_rabbit_middle(state) * 50
    return dog_score - rab_score + simpleEstimation(state)