//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//
#ifndef INVISIBLEMAZE_TILE_H
#define INVISIBLEMAZE_TILE_H

#include "GameObject.h"

struct TileFlags {
  bool has_base = false;
  bool has_player = false;
  bool has_dragon = false;
  bool has_treasure = false;
};

//This class describes the only type of GameObject that can hold the player,
//the dragon, the base or the treasure.
//Example map:           Tile    V-Wall   Tile    V-Wall   Tile
//                       H-Wall  Blank    H-Wall  Blank    H-Wall
//                       Tile    V-Wall   Tile    V-Wall   Tile
//                       H-Wall  Blank    H-Wall  Blank    H-Wall
//                       Tile    V-Wall   Tile    V-Wall   Tile

class Tile : public GameObject {
 public:

  Tile();
  Tile(const TileFlags& tile_flags);
  virtual ~Tile() {};

  bool has_base();
  bool has_dragon();
  bool has_player();
  bool has_treasure();

  void set_has_base(bool has_base);
  void set_has_dragon(bool has_dragon);
  void set_has_player(bool has_player);
  void set_has_treasure(bool has_treasure);

 private:
  //sets all 4 connection variables to "this" (a non nullptr initialization)
  //because Tiles can have connections in any of the 4 directions.
  void InitializeConnections() override;

  bool has_base_;
  bool has_player_;
  bool has_treasure_;
  bool has_dragon_;
};

#endif // INVISIBLEMAZE_TILE_H

