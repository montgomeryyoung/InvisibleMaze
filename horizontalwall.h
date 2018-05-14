//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//
#ifndef INVISIBLEMAZE_HORIZONTALWALL_H
#define INVISIBLEMAZE_HORIZONTALWALL_H

#include "Wall.h"

//This class defines a wall that is like a horizontal line.
//Connections are only possible in the up and down directions.
//Example map:           Tile    V-Wall   Tile    V-Wall   Tile
//                       H-Wall  Blank    H-Wall  Blank    H-Wall
//                       Tile    V-Wall   Tile    V-Wall   Tile
//                       H-Wall  Blank    H-Wall  Blank    H-Wall
//                       Tile    V-Wall   Tile    V-Wall   Tile

class HorizontalWall : public Wall {
 public:
  HorizontalWall();
  HorizontalWall(bool solid);
  virtual ~HorizontalWall() {};

 private:
  void InitializeConnections() override;
};

#endif // INVISIBLEMAZE_HORIZONTALWALL_H
