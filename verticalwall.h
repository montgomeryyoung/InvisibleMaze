//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//

#ifndef INVISIBLEMAZE_VERTICALWALL_H
#define INVISIBLEMAZE_VERTICALWALL_H

#include "Wall.h"

//This class defines a wall that is like a vertical line.
//Connections are only possible in the left and right directions.
//Example map:           Tile    V-Wall   Tile    V-Wall   Tile
//                       H-Wall  Blank    H-Wall  Blank    H-Wall
//                       Tile    V-Wall   Tile    V-Wall   Tile
//                       H-Wall  Blank    H-Wall  Blank    H-Wall
//                       Tile    V-Wall   Tile    V-Wall   Tile

class VerticalWall : public Wall {
 public:
  VerticalWall();
  VerticalWall(bool solid);
  virtual ~VerticalWall() {};

 private:
  virtual void InitializeConnections() override;
};

#endif // INVISIBLEMAZE_VERTICALWALL_H


