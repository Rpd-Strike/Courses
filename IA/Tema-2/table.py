from typing import List, Literal, Optional, Tuple

import copy

Player = Literal['D', 'R']
Cell = Literal[Player, '#', '.']
Move = Tuple[Tuple[int, int], Tuple[int, int]]

INVALID_MOVE: Move = (-1, -1), (-1, -1)

# Tipurile de stari ale nodurilor
S_INV: Cell = '#'
S_EMP: Cell = '.'
S_DOG: Player = 'D'
S_RAB: Player = 'R'

class Table:
    # Marimile jocului
    HEIGHT = 3
    WIDTH  = 5
    POSITIONS = [(i, j) for i in range(3) for j in range(5)]
    # Start table
    START_STATE: List[List[Cell]] = [
        ['#', 'D', '.', '.', '#'],
        ['D', '.', '.', '.', 'R'],
        ['#', 'D', '.', '.', '#']
    ]
    
    def __init__(self, state: Optional[List[List[Cell]]]):
        if state is None:
            state = copy.deepcopy(Table.START_STATE)
        self.state = state
    
    def neighbours(self, a: Tuple[int, int], b: Tuple[int, int]):
        """
        Sunt pozitiile a si b vecini?
        """
        ax, ay = a
        bx, by = b
        if (ax, ay) == (bx, by):
            return False
        if abs(ax - bx) > 1 or abs(ay - by) > 1:
            return False
        if min(ax, bx) < 0 or min(ay, by) < 0:
            return False
        if max(ax, bx) >= Table.HEIGHT or max(ay, by) >= Table.WIDTH:
            return False
        return True

    def valid_move(self, player: Player, move: Move) -> bool:
        """
        Returneaza daca mutarea este valida, fiind jucata de un anumit jucator
        """
        (fromX, fromY), (toX, toY) = move
        if (fromX, fromY) == (toX, toY):
            return False
        if abs(fromX - toX) > 1 or abs(fromY - toY) > 1:
            return False
        if min(fromX, toX) < 0 or min(fromY, toY) < 0:
            return False
        if max(fromX, toX) >= Table.HEIGHT or max(fromY, toY) >= Table.WIDTH:
            return False
        if self.state[fromX][fromY] != player:
            return False
        if self.state[toX][toY] != '.':
            return False
        if player == S_DOG and fromY > toY:
            return False
        return True

    def available_moves(self, player: Player) -> List[Move]:
        """
        Mutarile disponibile pentru un anumit jucator
        """
        moves: List[Move] = []
        for fromX, fromY in Table.POSITIONS:
            for dx, dy in [(i - 1, j - 1) for i in range(3) for j in range(3)]:
                toX, toY = fromX + dx, fromY + dy
                move = ((fromX, fromY), (toX, toY))
                if not self.valid_move(player, move):
                    continue
                moves.append(move)
        return moves

    def perform_move(self, player: Player, move: Move) -> bool:
        """
        Incearca sa faca mutarea
        Daca mutarea e valida, returneaza True, altfel False
        """
        if not self.valid_move(player, move):
            return False

        (fX, fY), (tX, tY) = move
        self.state[fX][fY], self.state[tX][tY] = self.state[tX][tY], self.state[fX][fY]
        
        return True

    def get_player_pieces(self, player: Player) -> List[Tuple[int, int]]:
        """
        Returneaza pozitiile pieselor unui anumit jucator
        """
        positions: List[Tuple[int, int]] = []
        for x, y in Table.POSITIONS:
            if self.state[x][y] == player:
                positions.append((x, y))
        return positions

    def rabbit_win_condition(self) -> bool:
        """
        Este iepurele mai in stanga decat cainii ?
        """
        leftest_dog = 10
        for _, y in self.get_player_pieces(S_DOG):
            leftest_dog = min(leftest_dog, y)
        rab_y = self.get_player_pieces(S_RAB)[0][1]
        
        return rab_y < leftest_dog

    def copy(self) -> 'Table':
        return Table(copy.deepcopy(self.state))