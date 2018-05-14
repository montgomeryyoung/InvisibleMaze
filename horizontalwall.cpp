//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//

#include "horizontalwall.h"

HorizontalWall::HorizontalWall() :Wall(false) {}
HorizontalWall::HorizontalWall(bool solid) : Wall(solid) {}

void HorizontalWall::InitializeConnections() {
  left_connection_ = nullptr;
  up_connection_ = this;
  right_connection_ = nullptr;
  down_connection_ = this;
}



