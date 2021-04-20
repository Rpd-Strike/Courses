from typing import Callable, List, Tuple

from utils import setTime, getElapsed, printState
from state import StateNode, getStartNode

"""
Algoritmul UCS
estimation: Functia de estimare  (in cazul acesta nu este folosita)
nr_sols: numarul de solutuii cautate
timeout: timpul in secunde pentru timeout
"""
def UCS(estimation: Callable, nr_sols: int, timeout: float) -> Tuple[List[StateNode], dict]:
    setTime()

    queue = [getStartNode()]
    ans = []
    data = {
        "time": [],
        "visited": [],
        "computed": []
    }
    visited = 0

    def costToNode(node: StateNode) -> float:
        return node.cost

    while queue != [] and len(ans) < nr_sols:
        if getElapsed() > timeout:
            break

        queue.sort(key = costToNode)

        node = queue[0]
        queue.pop(0)
        visited += 1

        if node.isFinal():
            ans.append(node)
            data["time"].append(getElapsed())
            data["visited"].append(visited)
            data["computed"].append(visited + len(queue))

        for nex in node.genSuccesori():
            # Not needed, as the state is strictly different than the 
            # ones above in path (less columns / lines)
            if not node.inPath(nex):
                queue.append(nex)
        
    return ans, data

"""
Algoritmul A*
estimation: Functia de estimare 
nr_sols: numarul de solutuii cautate
timeout: timpul in secunde pentru timeout
"""
def AStar(estimation: Callable, nr_sols: int, timeout: float) -> Tuple[List[StateNode], dict]:
    setTime()

    queue = [getStartNode()]
    ans = []
    data = {
        "time": [],
        "visited": [],
        "computed": []
    }
    visited = 0

    def costToNode(node: StateNode) -> float:
        return node.cost + estimation(node)

    while queue != [] and len(ans) < nr_sols:
        if getElapsed() > timeout:
            break

        queue.sort(key = costToNode)

        node = queue[0]
        queue.pop(0)
        visited += 1

        # node.showState()

        if node.isFinal():
            ans.append(node)
            data["time"].append(getElapsed())
            data["visited"].append(visited)
            data["computed"].append(visited + len(queue))
            # print("Found solution: Cost = ", node.cost)
            continue

        for nex in node.genSuccesori():            
            # No need to check, because if it is in the path
            # because there are strictly more columns and lines
            queue.append(nex)

    return ans, data

"""
Algoritmul  A* Optimizat
estimation: Functia de estimare 
nr_sols: numarul de solutuii cautate  (Datorita listelor open si closed, este gasita doar o singura solutie)
timeout: timpul in secunde pentru timeout
"""
def AStarFast(estimation: Callable, nr_sols: int, timeout: float) -> Tuple[List[StateNode], dict]:
    setTime()

    lopen: List[StateNode] = [getStartNode()]
    lclosed: List[StateNode] = []

    ans = []
    data = {
        "time": [],
        "visited": [],
        "computed": []
    }

    def isInClosed(node: StateNode) -> bool:
        for s in lclosed:
            if node.isEquivalent(s):
                return True
        return False
    
    def costToNode(node: StateNode) -> float:
        return node.cost + estimation(node)

    def updateOpen(node: StateNode) -> bool:
        for i in range(len(lopen)):
            if node.isEquivalent(lopen[i]):
                if node.cost < lopen[i].cost:
                    lopen[i] = node
                return True
        return False
    
    while lopen != [] and len(ans) < nr_sols:
        if getElapsed() > timeout:
            break

        lopen.sort(key=costToNode)

        node = lopen[0]
        lopen.pop(0)
        lclosed.append(node)

        if node.isFinal():
            ans.append(node)
            data["time"].append(getElapsed())
            data["visited"].append(len(lopen))
            data["computed"].append(len(lopen) + len(lclosed))
            # print("Found solution: Cost = ", node.cost)
            continue

        for nex in node.genSuccesori():
            if isInClosed(nex):
                continue
            if not updateOpen(nex):
                lopen.append(nex)
    
    return ans, data

"""
Algoritmul  IDA*
estimation: Functia de estimare 
nr_sols: numarul de solutuii cautate (In acest caz algoritmul se opreste la prima solutie)
timeout: timpul in secunde pentru timeout
"""
def IDAStar(estimation: Callable, nr_sols: int, timeout: float) -> Tuple[List[StateNode], dict]:
    setTime()

    ans = []
    data = {
        "time": [],
        "visited": [],
        "computed": []
    }
    visited, computed = 0, 0
    curr_estimation = 0

    def costToNode(node: StateNode) -> float:
        return node.cost + estimation(node)

    def searchPath(node: StateNode, bound: int) -> Tuple[StateNode, int]:
        nonlocal visited, computed, timeout

        if getElapsed() > timeout:
            return None, float("inf")

        if costToNode(node) > bound:
            return None, costToNode(node)
        
        if node.isFinal():
            return node, node.cost

        min_cost = float("inf")

        visited += 1

        for nex in node.genSuccesori():
            computed += 1

            # Not needed as it's guaranteed for successors to not be in path
            # if node.inPath(nex):
            #     continue

            f_node, best_cost = searchPath(nex, bound)

            if f_node is not None:
                return f_node, best_cost
            
            min_cost = min(min_cost, best_cost)
        
        return None, min_cost

    if getStartNode().isFinal():
        ans.append(getStartNode())
        data["time"].append(getElapsed())
        data["visited"].append(visited)
        data["computed"].append(computed)
        return ans, data

    while True:
        if getElapsed() > timeout:
            break

        f_node, curr_estimation = searchPath(getStartNode(), curr_estimation)
        computed += 1

        if f_node is not None:
            ans.append(f_node)
            data["time"].append(getElapsed())
            data["visited"].append(visited)
            data["computed"].append(computed)
            break

    return ans, data


SearchAlgorithms: List[Tuple[str, Callable]] = [
    ('UCS', UCS),
    ('A*', AStar),
    ('Fast A*', AStarFast),
    ('ID A*', IDAStar)
]
