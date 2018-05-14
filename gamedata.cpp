#include <vector>
#include <iterator>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
//#include <iostream>
#include <algorithm>


#include "gamedata.h"
#include "tile.h"
#include "horizontalwall.h"
#include "verticalwall.h"
#include "blankspace.h"

using std::vector;
using namespace std;

GameData::GameData() {
  player_moves_remaining_ = kMaxPlayerMoves;
  dragon_awake_ = false;
  game_state_ = GameState::kInitialization;
  pause_before_dragon_move_ = kDefaultPauseBeforeDragonMove;
  srand(time(NULL));
}

GameData::~GameData() {
  tile_changed_listeners_.clear();
  wall_changed_listeners_.clear();
  game_notification_listeners_.clear();
  ClearGameboard();
}

void GameData::AddPieceToGameboard(GamePieces game_piece_to_add) {
  if (game_state_ == GameState::kPlay) return;

  GameObject* game_object;
  TileFlags tile_flags;

  switch (game_piece_to_add) {
    case GamePieces::kPlainTile:
      game_object = new Tile(tile_flags); // Default flags are fine.
    break;
    case GamePieces::kTileWithDragon:
      game_object = new Tile(tile_flags); //default flags are fine. Dragon not set until it is awake.
      dragon_pointer_ = game_object;
    break;
    case GamePieces::kTileWithBase:
      tile_flags.has_base = true;
      game_object = new Tile(tile_flags);
      base_pointer_ = game_object;
      player_pointer_ = game_object;
    break;
    case GamePieces::kTileWithTreasure:
      tile_flags.has_treasure = true;
      game_object = new Tile(tile_flags);
      treasure_pointer_ = game_object;
    break;
    case GamePieces::kHorizontalOpenWall:
      game_object = new HorizontalWall(false);
    break;
    case GamePieces::kHorizontalSolidWall:
      game_object = new HorizontalWall(true);
    break;
    case GamePieces::kVerticalOpenWall:
      game_object = new VerticalWall(false);
    break;
    case GamePieces::kVerticalSolidWall:
      game_object = new VerticalWall(true);
    break;
    case GamePieces::kBlank:
    default:
      game_object = new BlankSpace();
    break;
  }

  game_object->InitializeConnections();
  game_object->listener_to_notify_ = ([&](GameObject* go) { OnUpdatesFromPieces(go); });
  gameboard_.emplace_back(game_object);
}


void GameData::ConnectGameboardPiecesTogether() {
  if (game_state_ == GameState::kPlay) return;

  for (vector<GameObject*>::iterator piece = gameboard_.begin(); piece != gameboard_.end(); ++piece) {
    //set the UP connection
    if ((*piece)->up_connection_ != nullptr) {
      if ((*piece)->id_ > pieces_per_row_)                    //not first row
        (*piece)->up_connection_ = *(piece - pieces_per_row_);
      else                                                //first row
        (*piece)->up_connection_ = nullptr;
    }

    //set the DOWN connection
    if ((*piece)->down_connection_ != nullptr) {
      if ((*piece)->id_ > pieces_per_row_ * (pieces_per_column_ - 1) )  //last row
        (*piece)->down_connection_ = nullptr;
      else                                                    //not last row
        (*piece)->down_connection_ = *(piece + pieces_per_row_);
    }

    //set the LEFT connection
    if ((*piece)->left_connection_ != nullptr) {
      if ((*piece)->id_ % pieces_per_row_ == 1 )                 //first column
        (*piece)->left_connection_ = nullptr;
       else                                                   //not first column
        (*piece)->left_connection_ = *(piece - 1);
    }

    //set the RIGHT connection
    if ((*piece)->right_connection_ != nullptr) {
      if ((*piece)->id_ % pieces_per_row_ == 0 )                 //last column
        (*piece)->right_connection_ = nullptr;
      else                                                   //not last column
        (*piece)->right_connection_ = *(piece + 1);
    }
  } //end for
}

void GameData::set_tiles_per_row(int tiles_per_row) {
  tiles_per_row_ = tiles_per_row;
  pieces_per_row_ = (tiles_per_row * 2) - 1;
}

void GameData::set_tiles_per_column(int tiles_per_column) {
  tiles_per_column_ = tiles_per_column;
  pieces_per_column_ = (tiles_per_column * 2) - 1;
}

void GameData::SetupGame() {
  ClearGameboard();

  game_state_ = GameState::kSetup;
  player_moves_remaining_ = kMaxPlayerMoves;
  dragon_awake_ = false;
}

void GameData::PlayGame() {
  game_state_ = GameState::kPlay;

  //Call the event handler as if the piece pointed to by player_pointer_ changed.
  //This will allow any connected view to know where the player is when the game begins.
  OnUpdatesFromPieces(player_pointer_);

  NotifyListener(GameNotifications::kRemainingMoves, player_moves_remaining_);
}


void GameData::ClearGameboard() {
  player_pointer_ = nullptr;
  treasure_pointer_ = nullptr;
  base_pointer_ = nullptr;
  dragon_pointer_ = nullptr;

  for (auto itr = gameboard_.begin(); itr != gameboard_.end(); ++itr) {
     delete *itr;
  }
  gameboard_.erase(gameboard_.begin(), gameboard_.end());
  gameboard_.clear();
  game_state_ = GameState::kInitialization;

  //reset the next_id_ so it starts at 1 just like when the class is first instantiated
  GameObject::next_id_ = 1;
}

GameObject* GameData::GetPieceFromGameboard(int id) {
  auto found_piece = find_if(gameboard_.begin(), gameboard_.end(),  [id](const GameObject* c) { return c->id_ == id; });
  return *found_piece;
}

//Tiles and Walls will call this function when they are updated.
//Raises an event specific to changed tiles or walls.
void GameData::OnUpdatesFromPieces(GameObject* updated_piece) {
  if (dynamic_cast<Tile*> (updated_piece) != nullptr) {        //A tile updated
    for (auto listener : tile_changed_listeners_)
      listener(dynamic_cast<Tile*>(updated_piece));
  } else if (dynamic_cast<Wall*> (updated_piece) != nullptr) { //A wall updated
      for ( auto listener : wall_changed_listeners_)
        listener(dynamic_cast<Wall*>(updated_piece));
  }
}

void GameData::NotifyListener(GameNotifications notification) {
    NotifyListener(notification, kIgnoreNumber);
}

void GameData::NotifyListener(GameNotifications notification, int num) {
  for ( auto listener : game_notification_listeners_)
    listener(notification, num);
}

TileChangedConnection GameData::ConnectTileChangeHandler(TileChangedListener l) {
  return tile_changed_listeners_.insert(tile_changed_listeners_.end(), l);
}
WallChangedConnection GameData::ConnectWallChangeHandler(WallChangedListener l) {
  return wall_changed_listeners_.insert(wall_changed_listeners_.end(), l);
}
GameNotificationConnection GameData::ConnectGameChangeHandler(GameNotificationListener l) {
  return game_notification_listeners_.insert(game_notification_listeners_.end(), l);
}
void GameData::DisconnectTileChangeHandler(TileChangedConnection c) {
	tile_changed_listeners_.erase(c);
}
void GameData::DisconnectWallChangeHandler(WallChangedConnection c) {
	wall_changed_listeners_.erase(c);
}
void GameData::DisconnectGameChangeHandler(GameNotificationConnection c) {
	game_notification_listeners_.erase(c);
}

void GameData::DecrementPlayerMovesRemaining() {
  player_moves_remaining_--;
  NotifyListener(GameNotifications::kRemainingMoves, player_moves_remaining_);
}

void GameData::UpdateGameboardWithMove(GameObject* move_from, GameObject* move_to) {
  Tile* current_tile = dynamic_cast<Tile*>(move_from);
  Tile* next_tile = dynamic_cast<Tile*>(move_to);

  //move player
  current_tile->set_has_player(false);
  next_tile->set_has_player(true);
  player_pointer_ = next_tile;

  //Move treasure, or raise event if treasure was found
  if (current_tile->has_treasure()) { //player has treasure so move it with the player
    current_tile->set_has_treasure(false);
    next_tile->set_has_treasure(true);
    treasure_pointer_ = next_tile;
  } else {
    if (next_tile->has_treasure()) { //Treasure was just found.
      NotifyListener(GameNotifications::kPlayerFoundTreasure);
      treasure_pointer_ = next_tile;

      //When the treasure is found, the dragon wakes, and the player's turn is over.
      WakeDragon();
      player_moves_remaining_ = 0;
      NotifyListener(GameNotifications::kRemainingMoves, player_moves_remaining_);
    }
  }
  //Player movement can cause the dragon to wake if the player comes too close
  //to the dragon.
  CheckIfDragonShouldWake();
}

Tile* GameData::SelectRandomTile() {
  //Number of pieces per row is 2 * pieces - 1. ex. Tile Wall Tile (2*2 -1 = 3)
  //Same with pieces per column. Rows and column lists always begin and end with a tile.
  int total_pieces_in_gameboard = ((2 * tiles_per_column_) -1) * ((2 * tiles_per_row_) - 1);
  int random_number;
  Tile* tile;
  GameObject* piece;

  do {
    random_number = (rand() % total_pieces_in_gameboard) + 1;
    piece = GetPieceFromGameboard(random_number);
  } while (dynamic_cast<Tile*> (piece) == nullptr);  //keep only tile pieces

  tile = dynamic_cast<Tile*> (piece);
  return tile;
}

void GameData::RandomlyAssignBaseTreasureAndDragon() {
  if (game_state_ == GameState::kPlay)
    return;

  Tile* tile;

  tile = SelectRandomTile();
  // The tile doesn't get the flag that it has a dragon until the dragon is awake.
  //But the dragon_pointer_ is still set to keep track of the sleeping dragon.
  dragon_pointer_ = tile;

  tile = SelectRandomTile();
  tile->set_has_treasure(true);
  treasure_pointer_ = tile;

  tile = SelectRandomTile();
  tile->set_has_base(true);
  tile->set_has_player(true);
  player_pointer_ = tile;
  base_pointer_ = tile;

  CheckIfDragonShouldWake();
}

void GameData::CheckIfDragonShouldWake() {
  if (dragon_awake_ == false) {
    if(dragon_pointer_->up_connection_ != nullptr)
      if(dragon_pointer_->up_connection_->up_connection_  == player_pointer_)
        WakeDragon();

    if(dragon_pointer_->down_connection_ != nullptr)
      if(dragon_pointer_->down_connection_->down_connection_  == player_pointer_)
        WakeDragon();

    if(dragon_pointer_->left_connection_ != nullptr)
      if(dragon_pointer_->left_connection_->left_connection_  == player_pointer_)
        WakeDragon();

    if(dragon_pointer_->right_connection_ != nullptr)
      if(dragon_pointer_->right_connection_->right_connection_  == player_pointer_)
        WakeDragon();

  }

}

void GameData::WakeDragon() {
  dragon_awake_=true;
  Tile* tile = dynamic_cast<Tile*> (dragon_pointer_);
  tile->set_has_dragon(true);
  NotifyListener(GameNotifications::kDragonAwakened);
}

GameObject* GameData::GetConnectedPiece(MoveDirections connection_direction, GameObject* current_piece) {
  GameObject* connected_piece = nullptr;
  switch (connection_direction) {
    case MoveDirections::kUp:
      connected_piece = current_piece->up_connection_;
      break;
    case MoveDirections::kDown:
      connected_piece = current_piece->down_connection_;
      break;
    case MoveDirections::kLeft:
      connected_piece = current_piece->left_connection_;
      break;
    case MoveDirections::kRight:
    connected_piece = current_piece->right_connection_;
      break;
  }
  return connected_piece;
}

void GameData::ShowAllWalls() {
  Wall* wallb;
  for (vector<GameObject*>::iterator piece = gameboard_.begin(); piece != gameboard_.end(); ++piece) {
    if (dynamic_cast<Wall*> (*piece) != nullptr) {
      wallb = dynamic_cast<Wall*> (*piece);
      if (wallb->solid()) wallb->set_visible(true);
    }
  }
}

void GameData::MovePlayer(MoveDirections direction_to_move) {
  if (game_state_ != GameState::kPlay) return;

  GameObject* next_gameboard_piece = nullptr;

  DecrementPlayerMovesRemaining();

  //starting with the tile pointed to by player_pointer_, find the connected wall
  next_gameboard_piece = GetConnectedPiece(direction_to_move, player_pointer_);

  //A wall should be connected to the tile. If it is a passthrough wall,
  //then a tile should be on the other side.
  if (dynamic_cast<Wall*> (next_gameboard_piece) != nullptr) {
    Wall* wall = dynamic_cast<Wall*> (next_gameboard_piece);
    if (wall->solid())  {
      //When the player runs into a solid wall, they cannot continue through.
      //If the solid wall is invisible, it needs to be set to visible.
      if (wall->visible() == false) wall->set_visible(true);
      if (player_moves_remaining_ <=0) {
        MoveDragon();
        ResetPlayerMovesRemaining();
      }
    } else {
      //look past the passthrough wall to the tile on the other side.
      next_gameboard_piece = GetConnectedPiece(direction_to_move, next_gameboard_piece);
      if (dynamic_cast<Tile*> (next_gameboard_piece) != nullptr)
        UpdateGameboardWithMove(player_pointer_, next_gameboard_piece);

      if ((player_pointer_ == treasure_pointer_) && (player_pointer_ == base_pointer_)) {
        NotifyListener(GameNotifications::kPlayerWon);
        game_state_ = GameState::kGameOver;
      }
      else if((player_pointer_ == dragon_pointer_) && (player_pointer_ != base_pointer_)) {
        NotifyListener(GameNotifications::kPlayerLost);
        game_state_ = GameState::kGameOver;
      }
      else if (player_moves_remaining_ <=0) {
        MoveDragon();
        if (game_state_ == GameState::kPlay) ResetPlayerMovesRemaining();
      }
    }
  }
}

GameState GameData::game_state() {
  return game_state_;
}

void GameData::MoveDragon() {
  if (dragon_awake_ == false) return;

  //The game waits for a specific amount of time before moving the dragon, to
  //give the impression to the player that the dragon is thinking about where to move
  std::this_thread::sleep_for(std::chrono::milliseconds(pause_before_dragon_move_));

  //The current row and column for the dragon
  int dragon_row, dragon_column;

  //The player's row and column
  int player_row, player_column;

  //The row and column where the dragon will move
  int dragon_new_row, dragon_new_column;

  int pieces_per_row = (2 * tiles_per_row_) - 1;

  //Find the dragon's row and column based on the ID of the dragon_pointer_
  if (dragon_pointer_->id_ % pieces_per_row == 0) {
    dragon_column = pieces_per_row;
    dragon_row = dragon_pointer_->id_ / pieces_per_row;
  } else {
    dragon_column = dragon_pointer_->id_ % pieces_per_row;
    dragon_row = (dragon_pointer_->id_ / pieces_per_row) + 1;
  }

  //Find the player's row and column based on the ID of the player_pointer_
  if (player_pointer_->id_ % pieces_per_row == 0) {
    player_column = pieces_per_row;
    player_row = player_pointer_->id_ / pieces_per_row;
  } else {
    player_column = player_pointer_->id_ % pieces_per_row;
    player_row = (player_pointer_->id_ / pieces_per_row) + 1;
  }

  //Find the row and col the dragon should move to.
  //Between each row or column of tiles, is a row or column of walls.
  //
  //Therefore, for the dragon to move to the next tile, the row or column must be
  //incremented or decremented by 2
  if (dragon_row > player_row)
    dragon_new_row = dragon_row - 2;
  else if (dragon_row < player_row)
    dragon_new_row = dragon_row + 2;
  else
    dragon_new_row = dragon_row;

  if (dragon_column > player_column)
    dragon_new_column = dragon_column - 2;
  else if (dragon_column < player_column)
    dragon_new_column = dragon_column + 2;
  else
    dragon_new_column = dragon_column;

  //turn the new row and col numbers back into an ID for an item in the game.
  int new_dragon_id = ((dragon_new_row - 1)*pieces_per_row) + dragon_new_column;

  //get the item from the gameboard that cooresponds with the new_dragon_id
  GameObject* next_dragon_pointer = GetPieceFromGameboard(new_dragon_id);

  Tile* current_dragon_tile = dynamic_cast<Tile*>(dragon_pointer_);
  current_dragon_tile->set_has_dragon(false);

  Tile* next_dragon_tile = dynamic_cast<Tile*>(next_dragon_pointer);
  next_dragon_tile->set_has_dragon(true);
  dragon_pointer_ = next_dragon_pointer;

  //After the move, if the dragon has found the player...then game over.
  if(player_pointer_ == dragon_pointer_) {
    NotifyListener(GameNotifications::kPlayerLost);
    game_state_ = GameState::kGameOver;
  }
}

void GameData::set_pause_before_dragon_move(int pause_in_milliseconds) {
  pause_before_dragon_move_ = pause_in_milliseconds;
}

int GameData::pause_before_dragon_move() {
  return pause_before_dragon_move_;
}

void GameData::ResetPlayerMovesRemaining() {
  if (player_pointer_ == treasure_pointer_)
    player_moves_remaining_ = kMaxPlayerAndTreasureMoves;
  else
    player_moves_remaining_ = kMaxPlayerMoves;
  NotifyListener(GameNotifications::kRemainingMoves, player_moves_remaining_);
}

int GameData::tiles_per_row() {
  return tiles_per_row_;
}

int GameData::tiles_per_column() {
  return tiles_per_column_;
}

int GameData::pieces_per_row() {
  return pieces_per_row_;
}

int GameData::pieces_per_column() {
  return pieces_per_column_;
}
