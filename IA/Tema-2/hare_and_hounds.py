from colors import BACK_COOLOR
from constants import FONT_NORMAL
from utils import EventType, name_player, print_time_statistics
from table import INVALID_MOVE, Player, S_DOG, S_RAB, Table
from state import State, other_player
from brains import getBrain, get_computed_nodes
from typing import Any, Dict, List, Optional, Tuple
import pygame, sys, time

from game import BrainType, Game

GAME_WIDTH, GAME_HEIGHT = 1024, 720
TITLE_OFFSET_H = 108
GAME_TITLE = "Hare and Hounds  -  Felix Puscasu"

is_DOG_AI, is_RAB_AI = False, False

def show_title(content: str):
    """
    Randeaza titlul
    """
    text_x = GAME_WIDTH // 2
    text_y = TITLE_OFFSET_H // 2
    text = FONT_NORMAL.render(content, True, (20, 20, 20))
    text_rect = text.get_rect()
    text_rect.center = (text_x, text_y)
    surface = pygame.display.get_surface()
    surface.blit(text, text_rect)

    pygame.display.flip()

def ask_question(question: str, table: List[List[Optional[str]]]) -> Optional[str]:
    """
    Randeaza optiunile la ecran si asteapta ca utilizatorul sa apese o optiune
    Returneaza sirul selectat de utilizator
    Asteapta pana cand utilizatorul da click pe o optiune valida
    """
    display = pygame.display
    surface = display.get_surface()
    surface.fill((220, 255 ,255))
    show_title(question)
    
    N = len(table)
    M = len(table[0])

    H = (GAME_HEIGHT - TITLE_OFFSET_H) // N
    W = GAME_WIDTH // M

    # print("box width: " + str(W))

    height = H - 12
    width = W - 12


    for i in range(N):
        for j in range(M):
            if table[i][j] is None:
                continue
            rect = pygame.Rect(
                W * j + 6,
                H * i + 6 + TITLE_OFFSET_H,
                width,
                height
            )
            
            pygame.draw.rect(surface, (100, 100, 100), rect, width=3, border_radius=4) 

            # print(f"box: {i}, {j}")

            text_x = W * j + W // 2
            text_y = TITLE_OFFSET_H + H * i + H // 2
            text = FONT_NORMAL.render(table[i][j], True, (20, 20, 20))
            text_rect = text.get_rect()
            text_rect.center = (text_x, text_y)
            surface.blit(text, text_rect)

    display.flip()

    choice = None
    while choice is None:
        # Loop through the events of the game.
        for event in pygame.event.get():
            # Quit.
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit(0)
            
            # Something was pressed.
            if event.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()
                pos = (pos[0], pos[1] - TITLE_OFFSET_H)

                # print(f"Click on pos: {pos[0]}, {pos[1]}")

                pos = (pos[1] // H, pos[0] // W)
                # print(f"Index of table: {pos[0]}, {pos[1]}")
                if pos[0] < 0 or pos[0] >= len(table):
                    choice = None
                    continue
                if pos[1] < 0 or pos[1] >= len(table[pos[0]]):
                    choice = None
                    continue
                
                choice = table[pos[0]][pos[1]]
    
    return choice

def ask_player_config(question: str, player: Player) -> BrainType:
    """
    O insiruire de intrebari pentru a selecta tipul de jucator
    """
    player_type = ask_question(question, [
        ["Human", "Min-Max Easy", "Alpha-Beta Easy"],
        [None, "Min-Max Medium", "Alpha-Beta Medium"],
        [None, "Min-Max Expert", "Alpha-Beta Expert"]
    ])

    estimation = None 
    if player_type != "Human":
        estimation = ask_question("Choose Estimation", [
            ["simple", "advanced"]
        ])
    
    global is_DOG_AI, is_RAB_AI
    if player_type != 'Human':
        if player == S_DOG:
            is_DOG_AI = True
        else:
            is_RAB_AI = True

    return getBrain(player_type, estimation)

def ask_config() -> Tuple[BrainType, BrainType]:
    """
    Intreaba Tipul de jucator pentru caini si pentru iepure.
    Poate fi uman, sau AI (A/B sau M/M) (Easy sau Medium sau Expert)
    """
    player_one = ask_player_config("Choose Player 1 (Dogs)", S_DOG)
    player_two = ask_player_config("Choose Player 2 (Rabbit)", S_RAB)
    return player_one, player_two


def perform_brain_move(game: Game, event: EventType, brain: BrainType) -> Tuple[bool, bool]:
    """
    Takes a brain and event, tries to perform a move
    Return (game or UI changed) , (game state changed)
    """
    player = game.state.turn
    change_status, move = brain(game, event)
            
    if not change_status:
        return False, False

    if move == INVALID_MOVE:
        # Some human player made a first click
        # print("Human player made first click to move piece")
        return True, False
        
    if not game.state.table.valid_move(player, move):
        print("  XXXXXXXX - Brain outputed invalid move!")

    if not game.perform_move(move):
        print("  XXXXXXXX - Move should be valid")

    if game.state.turn == player:
        print("  XXXXXXXX - Player should be changed after move!!!")
    
    return True, True


def game_loop(brains: Tuple[BrainType, BrainType]) -> \
        Tuple[Dict[str, Any], Game]:
    """
    Principala functie ce asculta inputurile din pygame si 
    aplica mutarile jucatorilor asupra jocului
    """
    def get_turn_brain(player: Player) -> BrainType:
        if player == S_DOG:
            return brains[0]
        return brains[1]

    def try_brain_move(game: Game, ev: EventType) -> bool:
        """
        Picking the brain for current player, tries to update game with the received move
        Returns true if a valid move is performed
        """
        # print("Try brain move")

        brain = get_turn_brain(game.state.turn)
        ui_change, state_change = perform_brain_move(game, ev, brain)

        # print(f"Tryingmove: UI - {ui_change} | State - {state_change}")

        return ui_change or state_change

    game = Game(State(Table(None), S_DOG, []), [])

    game_start_time = time.time()
    last_move_time = time.time()
    changed_game = False
    last_turn = game.state.turn

    print("   ------  Starea initiala")
    game.console_draw()

    dog_data: List[float] = []
    rabbit_data: List[float] = []
    dog_nodes: List[int] = []
    rabbit_nodes: List[int] = []


    while not game.state.isFinal():
        changed_game = False
        for ev in pygame.event.get():
            game.draw(pygame.display.get_surface())
            # print(f"Event type: {ev}")
            if ev.type == pygame.QUIT:
                print("  Intrerupt abrupt -------- ")
                show_stats({'winner': "Intrerupt / Invalid",
                            'dog_data': dog_data, 
                            'rabbit_data': rabbit_data,
                            'dog_nodes': dog_nodes,
                            'rabbit_nodes': rabbit_nodes}, game)
                pygame.display.quit()
                pygame.quit()
                sys.exit()
            
            if ev.type == pygame.MOUSEMOTION:
                continue


            changed_game = try_brain_move(game, ev.type)
            
            if changed_game:
                break


        if not changed_game:
            changed_game = try_brain_move(game, pygame.K_DELETE)
        
        game.draw(pygame.display.get_surface())
        
        if changed_game:    
            if game.state.turn == last_turn:
                continue
            # Calcule la schimbarea jucatorului
            last_turn = game.state.turn
            time_move = time.time() - last_move_time
            last_move_time = time.time()
            # Logging la shimbarea jucatorului
            print(f"{name_player(other_player(last_turn))} time for move: {time_move:.2f} s")
            if other_player(last_turn) == S_DOG:
                rabbit_data.append(time_move)
                if is_DOG_AI:
                    dog_nodes.append(get_computed_nodes())
                    print(f"Numar stari calculate la mutare: {get_computed_nodes()}")
            else:
                dog_data.append(time_move)
                if is_RAB_AI:
                    rabbit_nodes.append(get_computed_nodes())
                    print(f"Numar stari calculate la mutare: {get_computed_nodes()}")
            
            print(" --------- ")
            game.console_draw()

    print(" ========================== ")
    print(f"Total game time: {time.time() - game_start_time:.2f} s")
    print(f"Numar mutari Dog: {len(dog_data)}")
    print(f"Numar mutari Rabbit: {len(rabbit_data)}")

    print("\n Finished game")
    winner = S_DOG
    if game.state.rabbits_win():
        winner = S_RAB
    return {'winner': name_player(winner),
            'dog_data': dog_data, 
            'rabbit_data': rabbit_data,
            'dog_nodes': dog_nodes,
            'rabbit_nodes': rabbit_nodes}, game

def show_stats(data: Dict[str, Any], game: Game, other_title: Optional[str] = None):
    """
    Afiseaza datele colectate in consola 
    Asteapta ca utilizatorul sa inchida aplicatia prin quit
    """
    surface = pygame.display.get_surface()
    surface.fill(BACK_COOLOR)

    game.draw(surface, False)

    if other_title is None:
        "Winner: " + data['winner']
    show_title("Winner: " + data['winner'])

    print_time_statistics(data['dog_data'], data['dog_nodes'], is_DOG_AI, S_DOG)
    print_time_statistics(data['rabbit_data'], data['rabbit_nodes'], is_RAB_AI, S_RAB)

    running = True
    while running:
        # Loop through the events of the game.
        for event in pygame.event.get():
            # Quit.
            if event.type == pygame.QUIT:
                running = False
            
def main():
    """
    Entry point
    """
    pygame.init()
    pygame.font.init()
    pygame.display.set_mode((GAME_WIDTH, GAME_HEIGHT))
    pygame.display.set_caption(GAME_TITLE)

    brains = ask_config()

    data, game = game_loop(brains)

    show_stats(data, game)

    pygame.quit()

if __name__ == "__main__":
    main()