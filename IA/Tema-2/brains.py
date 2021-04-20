from estimations import advancedEstimation, simpleEstimation
from state import State
from typing import Optional, Tuple
from game import BrainType, Game
from table import INVALID_MOVE, Move, S_DOG, S_RAB
from utils import EstimationType, EventType, name_player
import pygame

alg_nodes_computed = 0
INF: int = 10 ** 9

def min_max(state: State, depth: int, est: EstimationType) \
    -> Tuple[int, Optional[Move]]:
    '''
        Min-Max algorithm.
    '''
    global alg_nodes_computed
    alg_nodes_computed += 1

    if depth == 0 or state.isFinal():
        return est(state), None

    player = state.turn
    estimate = -INF if player == S_DOG else INF
    action = None

    for move in state.table.available_moves(player):
        new_state = state.copy()
        new_state.perform_move(move)
        
        act, _ = min_max(new_state, depth - 1, est)

        if (player == S_DOG and act > estimate) or (player == S_RAB and act < estimate):
            estimate = act
            action = move

    return estimate, action
    

def alpha_beta(state: State, alpha: int, beta: int, depth: int, est: EstimationType) \
    -> Tuple[int, Optional[Move]]:
    '''
        Alpha-Beta algorithm
    '''
    global alg_nodes_computed
    alg_nodes_computed += 1

    if depth == 0 or state.isFinal():
        return est(state), None

    player = state.turn
    estimate = -INF if player == S_DOG else INF
    action = None

    next_states = [(state.copy(), move) for move in state.table.available_moves(player)]
    for st, move in next_states:
        st.perform_move(move)
    next_states = sorted(next_states, key=lambda x: est(x[0]))

    for new_state, move in next_states:
        # new_state = state.copy()
        # if not new_state.perform_move(move):
        #     print("OOOppssiieeee, good move seen as bad move ???")
        act, _ = alpha_beta(new_state, alpha, beta, depth - 1, est)

        if (player == S_DOG and act > estimate) or (player == S_RAB and act < estimate):
            estimate = act
            action = move

        if player == S_DOG:
            alpha = max(alpha, act)
        else:
            beta = min(beta, act)

        if alpha >= beta:
            break

    assert(action is not None)
    return estimate, action


def humanBrain(game: Game, event: EventType) -> Tuple[bool, Move]:
    """
    Brain-l echivalent pentru un om.
    Acesta trateaza evenimentul dat ca parametru si incearca sa obtina mutarea
    din nodul deja selectat si cel selectat acum.
    Returneaza daca schimba starea jocului 
    (atentie, nu neaparat cine muta, adica poate doar a selectat / deselectat ce nod muta)
    si daca se face o mutare, returneaza si Mutarea
    """
    if event != pygame.MOUSEBUTTONDOWN:
        return False, INVALID_MOVE
    mx, my = pygame.mouse.get_pos()
    cell_pos = game.clicked_cell(mx, my)
    # print(f"Click handled by human brain: {cell_pos}")
    if cell_pos is None:
        print(" :(   Nu ai apasat pe o celula!")
        return False, INVALID_MOVE
    else:
        cx, cy = cell_pos

    cell_pressed = game.state.table.state[cx][cy]

    # First cell is clicked 
    if len(game.clicked) == 0:
        if cell_pressed == game.state.turn:
            game.clicked.append((cx, cy))
            print("Ai selectat celula pe care o vei muta!")
            return True, INVALID_MOVE
        print(f" :(   Nu ai apasat pe o celula a jucatorului: {name_player(game.state.turn)}")
        return False, INVALID_MOVE
    
    first_clicked = game.clicked[0]

    # Unselect first choice if clicked the same
    if first_clicked == cell_pos:
        game.clicked = []
        print("Ai deselectat celula pe care o vei muta!")
        return True, INVALID_MOVE

    # Some initial cell of the player is clicked, check if move is valid
    player = game.state.turn
    if not game.state.table.valid_move(player, (first_clicked, cell_pos)):
        print(" :(   Celula pe care ai apasat nu genereaza o mutare valida")
        return False, INVALID_MOVE

    # Move is valid
    print("Mutare valida inregistrata pentru jucatorul de tip uman!")
    return True, (first_clicked, cell_pos)


def getMinMaxBrain(level: int, est: EstimationType) -> BrainType:
    """
    Functie ce returneaza algoritmul min-max parametrizat cu functia de estimare
    si cu adancimea data.
    In acelasi timp, salveaza si cate noduri s-au calculat
    """
    def le_brain(game: Game, event: EventType) -> Tuple[bool, Move]:
        global alg_nodes_computed
        alg_nodes_computed = 0
        scor, move = min_max(game.state, level, est)
        print(f"Min-Max scor: {scor}")
        print(f"Numar noduri calculate: {alg_nodes_computed}")
        if move is None:
            return False, INVALID_MOVE
        return True, move
    
    return le_brain

def getAlphaBetaBrain(level: int, est: EstimationType) -> BrainType:
    """
    Functie ce returneaza algoritmul alpha-beta parametrizat cu functia de estimare
    si cu adancimea data.
    In acelasi timp, salveaza si cate noduri s-au calculat
    """
    def le_brain(game: Game, event: EventType) -> Tuple[bool, Move]:
        global alg_nodes_computed
        alg_nodes_computed = 0
        scor, move = alpha_beta(game.state, -INF, INF, level, est)
        print(f"Alpha-Beta scor: {scor}")
        print(f"Numar noduri calculate: {alg_nodes_computed}")
        if move is None:
            return False, INVALID_MOVE
        return True, move
    
    return le_brain

def get_computed_nodes() -> int:
    """
    Pentru a obtine din alte fisiere cate noduri s-au calculat
    """
    return alg_nodes_computed

def getBrain(tip: Optional[str], estimation: Optional[str]) -> BrainType:
    """
    Dandu-se optiunile alese de utilizator
    returneaza creierul pentru respectivul jucator (Uman / AI cu anume parametrii)
    """
    if tip != "Human":
        print(f"Type of AI: {tip}")
        [alg_tip, alg_level] = str(tip).split()
        if alg_level == 'Easy':
            alg_level = 1
        elif alg_level == 'Medium':
            alg_level = 3
        else:
            alg_level = 6
        
        if estimation == 'simple':
            est_fnc = simpleEstimation
        else:
            est_fnc = advancedEstimation

        if alg_tip == 'Min-Max':
            return getMinMaxBrain(alg_level, est_fnc)
        else:
            return getAlphaBetaBrain(alg_level, est_fnc)

    return humanBrain