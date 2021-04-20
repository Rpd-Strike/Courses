from typing import List
import copy
# from utils import other_player

from table import Move, Player, S_DOG, S_RAB, Table

class State:
    def __init__(self, table: Table, turn: Player,
                       dog_moves: List[bool]):
        self.table = table
        self.turn: Player = turn
        self.dog_moves = dog_moves
            

    def isFinal(self) -> bool:
        """
        Returneaza daca starea este finala / terminala
        """
        nr_moves = len(self.table.available_moves(self.turn))
        if self.table.rabbit_win_condition():
            return True

        if self.turn == S_RAB:
            return nr_moves == 0

        # Acum stiu sigur ca self.turn == S_DOG
        if [True] * 5 == self.dog_moves:
            return True
        if nr_moves == 0:
            print(" XXXXXXX - Ciudat, cainii mereu ar treebui sa poata muta")
        return nr_moves == 0

    def rabbits_win(self) -> bool:
        """
        Daca iepurii sunt castigatori
        """
        if not self.isFinal():
            return False
        if self.table.rabbit_win_condition():
            return True
        if self.turn == S_DOG:
            if self.dog_moves == [True] * 5:
                return True
        return False

    def perform_move(self, move: Move) -> bool:
        """
        Modifica starea curenta aplicand mutarea data
        Returneaza daca mutarea a fost valida si update-ul a avut loc
        """
        if not self.table.valid_move(self.turn, move):
            return False
        self.table.perform_move(self.turn, move)
        if self.turn == S_DOG:
            self.dog_moves.append(move[0][1] == move[1][1])
            if len(self.dog_moves) > 5:
                self.dog_moves.pop(0)
        # self.score = self.est(self)
        self.turn = other_player(self.turn)
        return True

    def gen_next_states(self) -> List['State']:
        """
        Returneaza lista starilor succesor
        """
        succs: List[State] = []
        for move in self.table.available_moves(self.turn):
            another = copy.deepcopy(self)
            another.perform_move(move)
            succs.append(another)
        return succs   

    def copy(self) -> 'State':
        return State(self.table.copy(), self.turn,
                copy.deepcopy(self.dog_moves))

def other_player(player: Player) -> Player:
    if player == S_DOG:
        return S_RAB
    return S_DOG