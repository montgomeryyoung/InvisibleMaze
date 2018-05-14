#ifndef INVISIBLE_MAZE_INVISIBLEMAZEVIEW_H
#define INVISIBLE_MAZE_INVISIBLEMAZEVIEW_H

#include "tile.h"
#include "wall.h"
#include "horizontalwall.h"
#include "verticalwall.h"
#include "gamedata.h"



class InvisibleMazeView
{
 public:
  InvisibleMazeView();
  InvisibleMazeView(GameData* model);
  virtual ~InvisibleMazeView() {};

  //Listens for the event that signals a tile in the maze has changed
  //
  //Param: (tile) Pointer to the tile that changed.
  virtual void OnTileUpdate(Tile* tile) = 0;

  //Listens for the event that signals a wall in the maze has changed
  //
  //Param: (wall) Pointer to the wall that changed.
  virtual void OnWallUpdate(Wall* wall) = 0;

  //Listens for the event that signals something in the game (not tile or wall)
  // has changed
  //
  //Param: (notify_from_game) The specific thing that changed in the game.
  //Param: (i) Some notification (like # of moves remaining) require an integer
  //       to identify the quantity. The value is -1 when it should be ignored.
  virtual void OnGameUpdate(GameNotifications notify_from_game, int i) = 0;

 protected:
  GameData* model_;
};

#endif // INVISIBLE_MAZE_INVISIBLEMAZEVIEW_H
