//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//

#ifndef INVISIBLEMAZE_WALL_H
#define INVISIBLEMAZE_WALL_H

#include "gameobject.h"

//This class describes all walls. It is abstract, because, the only real walls
//in a maze are horizontal or vertical walls.
class Wall : public GameObject {
 public:
  Wall();
  Wall(bool solid);
  virtual ~Wall() {};
  void set_visible(bool visible);
  bool solid();
  bool visible();

 private:
  void InitializeConnections() override = 0;

  bool visible_;
  bool solid_;

};

#endif // INVISIBLEMAZE_WALL_H
