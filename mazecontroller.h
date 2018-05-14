//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//

#ifndef INVISIBLEMAZE_MAZECONTROLLER_H
#define INVISIBLEMAZE_MAZECONTROLLER_H

#include <string>

#include "gamedata.h"
#include "consoleview.h"

//Controls the inputs for the maze. This includes the initial data input to
//build the maze and the user's keyboard inputs to play the game.
class MazeController {
 public:
  MazeController();
  MazeController(GameData* model, ConsoleView* view);
  virtual ~MazeController() {};

  //Begins a loop where the maze is loaded and the game is started. When the game
  //is over, this function, clears the gameboard and asks the player which maze they
  //want to play or none. Choosing "none" allows the program to end.
  void PlayAgainLoop();

 private:
  //Builds a piece for the game maze using the 3 character code in its triplet.
  //
  //Param:  (triplet) a 3 character code. This code must be one of the ones
  //in the triplets[] array.
  void BuildMazePieceFromTriplet(string triplet);

  //Identifies if the row of pieces is a row of tiles and walls or
  //a row of blanks and walls. If a row of tiles and walls, then updates the
  //member variables for the tile_row_ and tile_columns_.
  //
  //Param:  (row_of_pieces) a series of triplets. (3 character codes) each
  //separated by a space.
  void ReadRowOfGamePieces(string row_of_pieces);

  //Builds a maze one line of pieces at a time. Does not identify a treasure
  //tile, a base tile or a dragon tile. So the function to randomly assign them
  //must be called if these build functions are used
  void BuildSmallMaze();
  void BuildMediumMaze();
  void BuildLargeMaze();

  //Begins a loop where the player can use the arrow keys to play the game. This
  //function uses conio.h to collect keypress, and therefore limits it to run
  //on Windows machines.
  void GameLoop();

  //These triplets are the text codes for identifying a game piece.
  //Their index in the array is the same as the value of the GamePieces enumerator
  //from gamedata.h.
  string triplets_[9] = {"TPL", "TWB", "TWT", "TWD", "WHS", "WHO", "WVS", "WVO", "__B"};
  GameData* model_;
  ConsoleView* view_;
  int tiles_per_row_;
  int tiles_per_column_;

};

#endif // INVISIBLEMAZE_MAZECONTROLLER_H
