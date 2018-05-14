//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project      4/30/2018
//
//This program creates an invisible maze. The player needs to find the treasure
//and return to the home base before the dragon catches the player.
//

#include "gamedata.h"
#include "consoleview.h"
#include "mazecontroller.h"
#include "tile.h"
#include "wall.h"

int main() {
  GameData* model = new GameData();
  ConsoleView* view = new ConsoleView(model);
  MazeController controller(model, view);

  model->ConnectTileChangeHandler([&](Tile* tile) { view->OnTileUpdate(tile); });
  model->ConnectWallChangeHandler([&](Wall* wall) { view->OnWallUpdate(wall); });
  model->ConnectGameChangeHandler([&](GameNotifications notification, int i) { view->OnGameUpdate(notification, i); });

  controller.PlayAgainLoop();
  delete model;
  delete view;

  return 0;
}




