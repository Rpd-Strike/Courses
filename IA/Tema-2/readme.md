## Clase:

### Game-State specific:
--------------
- Game -> Holds information about state of the game, whose turn is and in which graphic-state the GUI is. (Waiting for first select, waiting for second select)
- State -> A valid state of the game after specific moves, and whose turn is. Can compute next states and can be estimated by an estimator
- Table -> More of a helper class for State, holds information about possible moves for either player, doesnt know who is to move. (Kind of a "snapshot")
- Player -> Class that holds a method `performMove(Game, event)` which returns a Game by having the respective player play its turn in the game. (In particular, AI-ul se uita doar la Game.State, iar 'Human' se uita la Game.State si Game.Selected pentru a-si da seama ce mutare a fost aleasa din GUI)
------------
### Game loop:

1. Capture events
2. Pass events to Game
3. Game will modify its state by the state returned by the brain of the current player in response to the given event and state of Game
4. Draw new Game state

-------------
Program lifetime:

1. Ask type of player 1: Human / MinMax / AlphaBeta
2. Ask type of player 2: ...
3. initialize Game
4. Run game loop
5. Display results / statistics