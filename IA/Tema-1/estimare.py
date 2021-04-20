from typing import Callable, List, Tuple

from state import StateNode, getFinalNode

"""
Aceste estimari sunt descrise in readme.md
"""

def inadmissibleEst(node: StateNode) -> float:
    return len(node.state[0]) * 1000

def basicEst(node: StateNode) -> float:
    return 1

def columnsNeededEst(node: StateNode) -> float:
    return len(node.state[1]) - len(getFinalNode().state[1])

def smartEst(node: StateNode) -> float:
    col_cost = len(node.state[1]) - len(getFinalNode().state[1])
    lin_cost = len(getFinalNode().state[1])
    return col_cost + lin_cost

Estimations: List[Tuple[str, Callable]] = [
    ("Basic", basicEst),
    ("column based", columnsNeededEst),
    ("columns and lines based", smartEst),
    ("Inadmissible", inadmissibleEst),
]
