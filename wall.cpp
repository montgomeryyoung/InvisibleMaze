//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//
#include "wall.h"

Wall::Wall() : GameObject() {
  solid_ = false;
  visible_ = false;  //All walls start out invisible.
}

Wall::Wall(bool solid) : GameObject() {
  solid_ = solid;
  visible_ = false;  //All walls start out invisible.
}

void Wall::set_visible(bool visible) {
  if(visible != visible_) {
    visible_ = visible;
    NotifyListener();
  }
}

bool Wall::visible() {
  return visible_;
}

bool Wall::solid() {
  return solid_;
}
