//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//
#ifndef INVISIBLEMAZE_BLANKSPACE_H
#define INVISIBLEMAZE_BLANKSPACE_H

#include "GameObject.h"

//This class defines the blank space that is between wall objects such that
//there are no connections possible.
//
//Example map:           Tile    V-Wall   Tile    V-Wall   Tile
//                       H-Wall  Blank    H-Wall  Blank    H-Wall
//                       Tile    V-Wall   Tile    V-Wall   Tile
//                       H-Wall  Blank    H-Wall  Blank    H-Wall
//                       Tile    V-Wall   Tile    V-Wall   Tile

class BlankSpace : public GameObject {
 public:
  BlankSpace();
  virtual ~BlankSpace() {};

 private:
  void InitializeConnections() override;
};

#endif // INVISIBLEMAZE_BLANKSPACE_H
