//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//

#include "verticalwall.h"

VerticalWall::VerticalWall() : Wall(false){
}

VerticalWall::VerticalWall(bool solid) :Wall(solid){
}

void VerticalWall::InitializeConnections() {
  left_connection_ = this;
  up_connection_ = nullptr;
  right_connection_ = this;
  down_connection_ = nullptr;
}


