import os, time
from os import listdir, path
from typing import List

from state import StateNode, parseInput

local_time = 0
def setTime():
    """
    Functie ce reseteaza variabila 'local_time'
    """
    global local_time
    local_time = time.time()


def getElapsed():
    """
    Functie ce returneaza timpul trecut de la ultimul apel "setTime()"
    """
    return time.time() - local_time


def createDirectory(directory: str):
    """
    Creates direcotry if it doesnt exist
    """
    if os.path.exists(directory):
        return

    os.mkdir(directory)


def filesInFolder(folder: str) -> List[str]:
    """
    Returneaza numele fisierelor aflate intr-un folder dat ca parametru
    """
    return [file for file in listdir(folder) if path.isfile(path.join(folder, file))]


def Initialize(file: str):
    """
    Reads a file and loads START and FINAL state
    """
    with open(file, "r") as f:
        parseInput(f)

def printState(node: StateNode, msg: str):
    """
    Afisam sumar niste informatii despre un nod
    """
    print(msg, node)
    print("State: ", node.state)