//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//
#ifndef INVISIBLEMAZE_GAMEDATA_H
#define INVISIBLEMAZE_GAMEDATA_H

#include "gameobject.h"
#include "tile.h"
#include "wall.h"
#include <vector>
#include <memory>  //for unique_ptr
#include <list>

using std::vector;
using std::unique_ptr;
using std::list;
using std::function;

//These are the events that can be raised by the game. (Not counting events
//raised by changing GameObjects)
enum GameNotifications {
  kDragonAwakened,
  kPlayerFoundTreasure,
  kPlayerWon,
  kPlayerLost,
  kRemainingMoves
};

//The various specific GameObjects that can be instantiated.
enum GamePieces {
  kPlainTile = 0,
  kTileWithBase = 1,
  kTileWithTreasure = 2,
  kTileWithDragon = 3,
  kHorizontalSolidWall = 4,
  kHorizontalOpenWall = 5,
  kVerticalSolidWall = 6,
  kVerticalOpenWall = 7,
  kBlank = 8
};

//Used to identify movement directions
enum MoveDirections {
  kUp,
  kDown,
  kLeft,
  kRight
};

//Used to identify the state of the GameData object.
enum GameState {
  kSetup,
  kPlay,
  kInitialization,
  kGameOver
};


//These types are for raising events
//The listeners are pointers to functions that will be called when an event is
//raised. The Connections are iterators in the linked list of function pointers
//so a function can be removed from the list.
using TileChangedListener = function<void(Tile*)>;
using WallChangedListener = function<void(Wall*)>;
using GameNotificationListener = function<void(GameNotifications, int)>;
using TileChangedConnection = std::list<TileChangedListener>::iterator;
using WallChangedConnection = std::list<WallChangedListener>::iterator;
using GameNotificationConnection = std::list<GameNotificationListener>::iterator;

//This class is the entire model for the game. It instantiates a vector of GameObjects
//and maintains the game rules. This class has events to notify any listeners
//about changes to the game status or the GameObjects.
class GameData {
 public:
  GameData();
  virtual ~GameData();

  //Attempts to move the player in the indicated direction. If a wall is in the
  //indicated direction, then figure out if the wall is solid or not. If it is
  //solid, move through to the tile beyond. If not, the wall becomes visible and
  //the player stays on the same tile.
  //
  //Param: (direction_to_move) The direction to try to move the player.
  //        Must be member of the enum.
  void MovePlayer(MoveDirections direction_to_move);

  // Initializes the game state for a new game.
  void SetupGame();

  // Ads the identified piece to the gameboard vector. Piece is added to the end of the
  // vector and the piece id should be one more than the vector index since id starts at 1
  //
  //Param: (game_piece_to_add) Identifies which kind of piece to add to the gameboard
  //       Must be a value in the enum.
  void AddPieceToGameboard(GamePieces game_piece_to_add);

  //If the base, treasure and dragon are not setup when the maze was created
  //(through AddPieceToGameboard) then they need to be set before the game begins.
  //This method randomly selects tiles. One is assigned the base, another the treasure
  //and to a third, the dragon.
  void RandomlyAssignBaseTreasureAndDragon();

  // Sets the connection pointers for each GameObject so they point to each other
  // according to the maze structure. Tiles point to nullptr for sides against a
  // maze edge, or they point to adjacent walls. Walls point to adjacent tiles.
  void ConnectGameboardPiecesTogether();

  // Changes the game state to kPlay. This ends the setup mode
  //when the gameboard is being built, connected and initialized.
  void PlayGame();

  //Makes all the walls visible.
  void ShowAllWalls();

  //Destructs all the GameObjects in the gameboard, and resets the game.
  void ClearGameboard();

  //Connects an event handler and returns an iterator to the new connection.
  //
  //Param:  A function pointer that conforms to the type definition (called a listener)
  //Return: A defined type that is an iterator for the linked list of listeners.
  //        This iterator can be used to remove the listener. It is the client's
  //        responsibility to delete the iterator
  TileChangedConnection ConnectTileChangeHandler(TileChangedListener l);
  WallChangedConnection ConnectWallChangeHandler(WallChangedListener l);
  GameNotificationConnection ConnectGameChangeHandler(GameNotificationListener l);

  //removes the identified event handler connection.
  //
  //Param:  An iterator for the appropriate linked list of listeners.
  void DisconnectTileChangeHandler(TileChangedConnection c);
  void DisconnectWallChangeHandler(WallChangedConnection c);
  void DisconnectGameChangeHandler(GameNotificationConnection c);

  //Get & Set
  //The dragon pauses before moving. This is to allow the view to show something
  //before and after the dragon move.
  int pause_before_dragon_move();
  void set_pause_before_dragon_move(int pause_in_milliseconds);

  //Get only
  //The game will only accept particular inputs when it is in a particular state.
  //The state changes are controlled internally, but the current state can be accessed.
  GameState game_state();

  //Get & Set
  int tiles_per_row();
  void set_tiles_per_row(int tiles_per_row);
  int tiles_per_column();
  void set_tiles_per_column(int tiles_per_column);

  //Get only
  int pieces_per_row();
  int pieces_per_column();

 private:
  //The game notification event has 2 parameters. The first identifies the
  //notification. The second is an integer. Some notifications need the
  //integer, others don't. When
  const int kIgnoreNumber = -1;

  const int kMaxPlayerMoves = 6;
  const int kMaxPlayerAndTreasureMoves = 4;
  const int kDefaultPauseBeforeDragonMove = 100;


  //Randomly selects and returns a tile from the gameboard_.
  Tile* SelectRandomTile();

  //If player moves to within 1 tile of the dragon (including diagonals),
  //then dragon should wake. 1 tile away is actually 2 pieces away, since there
  //are walls between tiles.
  void CheckIfDragonShouldWake();

  //The class maintains a variable for move remaining in this turn for the player.
  //This needs to be maintained, because, if the dragon is awake, it moves after the
  //player's moves are used up for this turn. This function raises an event.
  void DecrementPlayerMovesRemaining();

  //The dragon moves 1 space either horizontally, vertically, or diagonally.
  //This function moves the dragon and updates the affected tiles.
  void MoveDragon();

  //This function is called to reset the player's turn. If the player has already
  //found the treasure, they have a different number of moves available.
  void ResetPlayerMovesRemaining();

  //Selects and returns a piece of the gameboard.
  //
  //Param:  (id) the id value for a piece in the gameboard.
  //Return: If there is a piece in the gameboard with that id, then a pointer
  //to the piece is returned. If not, a nullptr is returned.
  GameObject* GetPieceFromGameboard(int id);

  //This function is an event handler and is called whenever a piece on the gameboard changes
  //
  //Param:  (updated_piece) A pointer to the piece that has changed
  void OnUpdatesFromPieces(GameObject* updated_piece);

  //These functions raise the Game Notification event to any listeners.
  //
  //Param:  (notification) This value is bound to the enumeration.
  //        (num) This value is either the kIgnoreNumber constant or it is a useful number
  //         depending on which notification is in the parameter.
  void NotifyListener(GameNotifications notification);
  void NotifyListener(GameNotifications notification, int num);

  //Updates the tile flags (member variables) of the two tiles that are sent in as
  //parameters to reflect the successful movement of the player.
  //
  //Param:  (move_from) This is a GameObject*, but it is expected to be a Tile*
  //        (move_to) This is another GameObject* that is expected to be a Tile*
  void UpdateGameboardWithMove(GameObject* move_from, GameObject* move_to);

  //Following the connection in the indicated direction, this function returns the next gameboard piece.
  //
  //Param:  (connection_direction) The indicated direction. The GameObject has pointers in 4 directions.
  //         and this indicates which pointer to follow.
  //         (current_piece) This is the GameObject piece to start with.
  //Return: A pointer to the GameObject that is in the indicated direction.
  GameObject* GetConnectedPiece(MoveDirections move_direction, GameObject* current_piece);

  //Sets the tile flag indicating that the dragon is on the tile. The tile that
  //is changed is the one pointed to by the dragon_pointer_. A sleeping dragon is
  //not indicated on the tile. Only an awake one.
  void WakeDragon();

  GameObject* player_pointer_;
  GameObject* treasure_pointer_;
  GameObject* base_pointer_;
  GameObject* dragon_pointer_;

  int player_moves_remaining_;
  bool dragon_awake_;
  int pause_before_dragon_move_;
  GameState game_state_;
  int tiles_per_row_;
  int tiles_per_column_;
  int pieces_per_row_;
  int pieces_per_column_;

  vector<GameObject*> gameboard_;

  list<TileChangedListener> tile_changed_listeners_;
  list<WallChangedListener> wall_changed_listeners_;
  list<GameNotificationListener> game_notification_listeners_;



};

#endif // INVISIBLEMAZE_GAMEDATA_H
