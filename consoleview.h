//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//
#ifndef INVISIBLEMAZE_CONSOLEVIEW_H
#define INVISIBLEMAZE_CONSOLEVIEW_H

#include <utility>
#include <iostream>
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>


#include "invisiblemazeview.h"

using namespace std;
using ConsoleCoordinates = pair<short, short>;

//This class displays the invisible maze game on a windows console.
class ConsoleView : public InvisibleMazeView {
 public:
  ConsoleView();
  ConsoleView(GameData* model);
  virtual ~ConsoleView() {};

  //Listen for the 3 events that are raised by the model.
  void OnTileUpdate(Tile* tile) override;
  void OnWallUpdate(Wall* wall) override;
  void OnGameUpdate(GameNotifications notify_from_game, int i) override;

  //Draws the maze border, title and everything else except the maze contents,
  //The maze border needs the maze dimensions, so they are sent in as parameters
  void InitializeDisplay();

 private:
  //Border edge coordinates for the maze.
  const short kStartingXValue = 0;
  const short kStartingYValue = 14;

  //Starting coordinates where the status area is drawn.
  const short kStartingXStatusValue = 36;
  const short kStartingYStatusValue = 5;

  //Colors for drawing on the system console
  const short kWhite = 7;
  const short kGreen = 2;
  const short kLightGreen = 10;
  const short kRed = 12;

  //To determine the size of each tile.
  const int kHorizontalSpacesForOneTile = 4;
  const int kVerticalSpacesForOneTile = 2;
  const int kSpaceForWall = 1;
  const int kSingleSpace = 1;


  //The ASCII codes used to draw the maze
  enum AsciiCodesForMaze {
    kVerticalLine=179,      kRightFacingT=195,
    kBlankSpace=32,         kLeftFacingT=180,
    kTopLeftCorner=218,     kHorizontalLine=196,
    kDownFacingT=194,       kTopRightCorner=191,
    kBottomLeftCorner=192,  kUpFacingT=193,
    kBorromRightCorner=217, kBlock=219
  };

  //The kinds of tiles and walls that can be shown in the view.
  enum PiecesForDisplay {
    kHorizontalVisibleWall,       kHorizontalInvisibleWall,
    kVerticalVisibleWall,         kVerticalInvisibleWall,
    kTileWithBase,                kTileWithPlayerAndTreasure,
    kTileWithPlayer,              kTileWithDragon,
    kTileThatIsEmpty
  };

  //Moves the cursor to the specified coordinates. Uses conio.h and requires
  //windows operating system.
  //
  //Param:  (console_x_coordinate) horizontal coordinate in the console screen.
  //        (console_y_coordinate) vertical coordinate in the console screen.
  void GotoXY( short console_x_coordinate, short console_y_coordinate );

  //Writes a character on the console screen.
  //
  //Param:  (char_to_write) The character that is written to the screen.
  //        (write_length) The number of times the character is written.
  //        If this parameter is not provided, the character is written once.
  void WriteCharacterToConsole(char char_to_write, int write_length);
  void WriteCharacterToConsole(char char_to_write);

  //Draws the outline of the game board on the console screen.
  void DrawGameBoard();

  //Draws the game border for one row of tiles. (left and right sides)
  //
  //Param:  (row_number) This is used to calculate the y coordinate.
  //        (use_t) Indicates that the sides should be the sideways T ascii value.
  void DrawTileRow(int row_number, bool use_t);

  //Draws the top border of the maze
  void DrawTopBorder();

  //Draws the bottom border of the maze
  void DrawBottomBorder();

  //Draws the title graphic for the maze
  void DrawTitle();

  //Draws a game piece
  //
  //Param:  (game_piece) identifies what kind of piece to draw.
  //        (coords) the x and y coordinates for drawing the piece.
  void DrawGamePiece(PiecesForDisplay game_piece, ConsoleCoordinates coords);

  //Calculates the coordinates on the console where a piece should be drawn.
  //The calculation is done based on the id of the piece to draw and the
  //number of pieces per row in the display.
  //Param:  (maze_piece_id) The id of the piece whose coordinates are being calculated.
  ConsoleCoordinates GetConsoleCoordinates(int maze_piece_id);

  //Updates the status area of the display with the current number of turns left
  //for the player or the dragon.
  void ShowNumberOfTurnsLeft();

  //Erases the player or dragon status icon where it is, and redraws it in a new location
  //
  //Param:  (delta_x) The change in the x direction for the new location.
  //        (delta_y) The change in the y direction for the new location.
  //        (wait_time_before_move) Milliseconds of wait before the icon is moved.
  void MovePlayerStatusIcon(int delta_x, int delta_y, int wait_time_before_move);
  void MoveDragonStatusIcon(int delta_x, int delta_y, int wait_time_before_move);

  //Sets the color that the console will draw in. This uses conio.h and windows.h
  //so it limits the program to work only in windows.
  //
  //Param:  (color) A numerical representation of color as defined in windows.h
  void SetConsoleColor(WORD color);

  //Updates the status area of the screen
  void DrawPlayerStatus();

  //Moves the console input position to a particular place that is out of the way.
  void GotoDefaultConsoleCursorPosition();

  //Draws the green bar that indicates if it is the player's turn or the dragon's turn
  void DrawTurnStatus(bool player_has_next_turn);

  //Draws the animation that is shown when the player or the dragon win.
  void DrawWinAnimation(bool player_has_won);

  bool player_has_treasure_;
  int player_moves_remaining_;
  bool dragon_is_awake_;

  short maze_left_edge_;
  short maze_right_edge_;
  short maze_top_edge_;
  short maze_bottom_edge_;
};

#endif // INVISIBLEMAZE_CONSOLEVIEW_H
