from colors import BACK_COOLOR, BLACK, GREEN, GREY, WALL_COLOR, YELLOW
import pygame
from constants import FONT_NORMAL, GAME_HEIGHT, GAME_WIDTH, TITLE_OFFSET_H
import copy
from utils import EventType, name_player
from table import Cell, Move, S_DOG, S_EMP, S_INV, S_RAB, Table
from typing import Any, Callable, List, Optional, Tuple
from state import State


def compute_cells_location(x: int, y: int) -> List[List[Tuple[int, int]]]:
    """
    Dandu-se numarul de celule pe verticala si orizontala,
    Calculeaza pozitia in pixeli a centrelor cercurilor
    """
    p: List[List[Tuple[int, int]]] = []
    LOW_OFFSET = 0
    H = (GAME_HEIGHT - TITLE_OFFSET_H - LOW_OFFSET) // x
    # W = (GAME_WIDTH - 30) // y
    W = H
    for i in range(x):
        l: List[Tuple[int, int]] = []
        for j in range(y):
            l.append((j * W + W // 2, i * H + H // 2 + TITLE_OFFSET_H))
        p.append(l)
    return p

def hover_cell(pos: Tuple[int, int]) -> bool:
    """
    Returneaza daca utilizatorul este cu mouseul peste o piesa
    """
    if dist_2d(pygame.mouse.get_pos(), pos) <= 15 * 15:
        return True
    return False

def dim_if_hover(pos: Tuple[int, int], color: pygame.Color) -> pygame.Color:
    """
    Daca utilizatorul este cu mouse-ul peste o piesa, 
    returneaza culoarea mai diferita, pentru GUI mai bun
    """
    def lighten(val: int) -> int:
        return min(255, val + 40)
    
    if hover_cell(pos):
        if color == BLACK:
            return pygame.Color(100, 100, 100)
        color = pygame.Color(lighten(color[0]), lighten(color[1]), lighten(color[2]))
    return color

def draw_clicked(surface: Any, pos: Tuple[int, int]):
    """
    Deseneaza cercul galben semnaland piesa selectat pentru mutare
    """
    pygame.draw.circle(surface, dim_if_hover(pos, YELLOW), pos, 20)
    
def draw_cell(surface: Any, pos: Tuple[int, int], cell: Cell):  
    """
    Deseneaza tipul de celula dat, la locatia data ca argument
    """  
    if cell == S_INV:
        return
    elif cell == S_DOG:
        pygame.draw.circle(surface, dim_if_hover(pos, GREY), pos, 15)
    elif cell == S_RAB:
        pygame.draw.circle(surface, dim_if_hover(pos, GREEN), pos, 15)
    elif cell == S_EMP:
        pygame.draw.circle(surface, dim_if_hover(pos, BLACK), pos, 10)

def dist_2d(a: Tuple[int, int], b: Tuple[int, int]) -> int:
    dx = a[0] - b[0]
    dy = a[1] - b[1]
    return dx * dx + dy * dy

def show_title(content: str):
    """
    Randeaza titlul sus
    """
    text_x = GAME_WIDTH // 2
    text_y = TITLE_OFFSET_H // 2
    text = FONT_NORMAL.render(content, True, (20, 20, 20))
    text_rect = text.get_rect()
    text_rect.center = (text_x, text_y)
    surface = pygame.display.get_surface()
    surface.blit(text, text_rect)

class Game:
    """
    Clasa ce contine informatia unei stari de joc,
    impreuna cu ce piesa este selectata momentan
    Stie sa deseneze starea jocului grafic
    """
    CLICK_RADIUS_SQ = 16 ** 2

    def __init__(self, state: State, clicked: List[Tuple[int, int]]):
        # Constant
        self.cells = compute_cells_location(Table.HEIGHT, Table.WIDTH)
        
        # Updated for each change
        self.state = state
        self.clicked = clicked


    def clicked_cell(self, x: int, y: int) -> Optional[Tuple[int, int]]:
        """
        If clicked on a valid cell (i.e. '.' or 'D' or 'R')
        returns the table coordinates
        """
        for i, line in enumerate(self.cells):
            for j, cell in enumerate(line):
                if dist_2d(cell, (x, y)) < Game.CLICK_RADIUS_SQ:
                    return i, j
        return None

    def draw(self, surface: Any, draw_who_moves: bool = True):
        """
        Deseneaza starea curenta grafic
        """
        surface.fill(BACK_COOLOR)

        if draw_who_moves:
            show_title("Move: " + name_player(self.state.turn))

        self.draw_bars(surface)

        for cx, cy in self.clicked:
            draw_clicked(surface, self.cells[cx][cy])
        for i in range(Table.HEIGHT):
            for j in range(Table.WIDTH):
                draw_cell(surface, self.cells[i][j], 
                    self.state.table.state[i][j])      

        pygame.display.flip()


    def perform_move(self, move: Move) -> bool:
        """
        Tries to perform given move
        Returns true if it actually changes the current state
        """
        if not self.state.table.valid_move(self.state.turn, move):
            return False
        
        if not self.state.perform_move(move):
            print("Performing valid move from game shouldn't fail")

        self.clicked = []

        return True

    def draw_bars(self, surface: Any):
        """
        Deseneaza conexiunile intre celulele care sunt vecini
        """
        for fro in Table.POSITIONS:
            for to in Table.POSITIONS:
                if not self.state.table.neighbours(fro, to):
                    continue
                if fro[1] > to[1]:
                    continue
                tb = self.state.table.state
                if tb[fro[0]][fro[1]] == S_INV:
                    continue
                if tb[to[0]][to[1]] == S_INV:
                    continue
                x_shift, y_shift = 0, 0
                # print(f"Vecini: {fro}, {to}")
                fromCord, toCord = self.cells[fro[0]][fro[1]], self.cells[to[0]][to[1]]
                lg = dist_2d(fromCord, toCord) ** 0.5
                # lg -= 2
                angle = 0
                if to[1] != fro[1]:
                    if fro[0] < to[0]:
                        angle = 90 * 3 + 45
                    elif fro[0] > to[0]:
                        angle = 45
                        y_shift = toCord[1] - fromCord[1]
                else:
                    if fro[0] < to[0]:
                        angle = 90
                    else:
                        continue
                # Create rectangle
                Width = 4
                myBar: Any = pygame.Surface((lg, Width), pygame.SRCALPHA)
                myBar.fill(WALL_COLOR)
                myBar = pygame.transform.rotate(myBar, angle)
                x_shift += fromCord[0]
                y_shift += fromCord[1]
                surface.blit(myBar, (x_shift - Width // 2, y_shift - Width // 2))


    def console_draw(self):
        """
        Informatii sumare despre jucatorul curent
        """
        print("Jucator la mutare: " + name_player(self.state.turn))
        tb = self.state.table.state
        for line in tb:
            print("".join(line))

    def copy(self):
        return Game(self.state.copy(), copy.deepcopy(self.clicked))

BrainType = Callable[[Game, EventType], Tuple[bool, Move]]
