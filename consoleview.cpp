//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//

#include "consoleview.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


ConsoleView::ConsoleView() : InvisibleMazeView() {

}
ConsoleView::ConsoleView(GameData* model) : InvisibleMazeView(model) {
  player_moves_remaining_ = 0;
}

void ConsoleView::GotoXY(short console_x_coordinate, short console_y_coordinate) {
  COORD coord = { console_x_coordinate, console_y_coordinate };
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

ConsoleCoordinates ConsoleView::GetConsoleCoordinates(int maze_piece_id) {
  short piece_row, piece_column;
  int pieces_per_row = model_->pieces_per_row();

  //To get the coordinates, the row and column of the identified piece have to
  //be calculated.
  if (maze_piece_id % pieces_per_row == 0) {
    piece_column = pieces_per_row;
    piece_row = maze_piece_id / pieces_per_row;
  } else {
    piece_column = maze_piece_id % pieces_per_row;
    piece_row = (maze_piece_id / pieces_per_row) + 1;
  }

  ConsoleCoordinates coord (0,0);

  int number_of_tiles_to_consider;
  int number_of_walls_to_consider;
  int spaces_for_previous_tiles;
  int spaces_for_previous_walls;

  //Prepare to calculate x coordinate
  number_of_tiles_to_consider  = piece_column / 2;
  number_of_walls_to_consider = (piece_column - 1) / 2;
  spaces_for_previous_tiles = number_of_tiles_to_consider * kHorizontalSpacesForOneTile;
  spaces_for_previous_walls = number_of_walls_to_consider * kSpaceForWall;

  //calculate x coordinate. Add the starting value, the space for the maze border,
  //the spaces for previous walls and the spaces for previous tiles.
  coord.first = maze_left_edge_ + kSpaceForWall + spaces_for_previous_tiles + spaces_for_previous_walls;

  //Prepare to calculate y coordinate
  number_of_tiles_to_consider  = piece_row / 2;
  number_of_walls_to_consider = (piece_row - 1) / 2;
  spaces_for_previous_tiles = number_of_tiles_to_consider * kVerticalSpacesForOneTile;
  spaces_for_previous_walls = number_of_walls_to_consider * kSpaceForWall;

  //calculate y coordinate. Add the starting value, the space for the maze border,
  //the spaces for previous walls and the spaces for previous tiles.
  coord.second = maze_top_edge_ + kSpaceForWall + spaces_for_previous_tiles + spaces_for_previous_walls;

  return coord;
}



void ConsoleView::InitializeDisplay() {
  player_has_treasure_ = false;
  dragon_is_awake_ = false;
  maze_left_edge_ = kStartingXValue;
  maze_right_edge_ = maze_left_edge_ + ((kHorizontalSpacesForOneTile + kSpaceForWall) * model_->tiles_per_row());
  maze_top_edge_ = kStartingYValue;
  maze_bottom_edge_ = maze_top_edge_ + ((kVerticalSpacesForOneTile + kSpaceForWall) * model_->tiles_per_column());

  system("cls");
  DrawTitle();
  DrawGameBoard();
  DrawPlayerStatus();
}

void ConsoleView::MovePlayerStatusIcon(int delta_x, int delta_y, int wait_time_before_move = 0) {
  static ConsoleCoordinates old_coords(kStartingXStatusValue, kStartingYStatusValue);

  this_thread::sleep_for(chrono::milliseconds(wait_time_before_move));

  ConsoleCoordinates coords (old_coords);
  DrawGamePiece(PiecesForDisplay::kTileThatIsEmpty, coords);

  coords = make_pair(old_coords.first + delta_x, old_coords.second + delta_y);
  DrawGamePiece(PiecesForDisplay::kTileWithPlayerAndTreasure, coords);

  old_coords = coords;
}

void ConsoleView::MoveDragonStatusIcon(int delta_x, int delta_y, int wait_time_before_move = 0) {
  static ConsoleCoordinates old_coords(kStartingXStatusValue, kStartingYStatusValue + 3);

  this_thread::sleep_for(chrono::milliseconds(wait_time_before_move));

  ConsoleCoordinates coords (old_coords);
  DrawGamePiece(PiecesForDisplay::kTileThatIsEmpty, coords);

  coords = make_pair(old_coords.first + delta_x, old_coords.second + delta_y);
  DrawGamePiece(PiecesForDisplay::kTileWithDragon, coords);

  old_coords = coords;
}


void ConsoleView::DrawWinAnimation(bool player_has_won) {
  const int kSmallWait = 100;
  const int kMicroWait = 20;
  const int kMediumWait = 400;

  //erase the text showing how many turns are left.
  GotoXY(kStartingXStatusValue + 5, kStartingYStatusValue);
  cout<< "                        ";
  GotoXY(kStartingXStatusValue + 5, kStartingYStatusValue + 3);
  cout<< "                        ";


    if (player_has_won) {
      MovePlayerStatusIcon(0,-1,0);
      MovePlayerStatusIcon(0,1,kSmallWait);
      MovePlayerStatusIcon(0,-1,kMediumWait);
      MovePlayerStatusIcon(0,7,kSmallWait);
      for (int i = 0; i < 30; i++)
        MovePlayerStatusIcon(-1,0,kMicroWait);
      MovePlayerStatusIcon(-1,0,kMicroWait);
      GotoXY(kStartingXStatusValue-25, kStartingYStatusValue+7);
      cout << "Player wins!";

    } else {
      MoveDragonStatusIcon(0,-1,0);
      MoveDragonStatusIcon(0,1,kSmallWait);
      MoveDragonStatusIcon(0,-1,kMediumWait);
      MoveDragonStatusIcon(0,5,kSmallWait);
      for (int i = 0; i < 30; i++)
        MoveDragonStatusIcon(-1,0,kMicroWait);
      MoveDragonStatusIcon(-1,0,kMicroWait);
      GotoXY(kStartingXStatusValue-25, kStartingYStatusValue+7);
      cout << "Dragon wins!";

     }
}

void ConsoleView::DrawPlayerStatus() {
  ConsoleCoordinates coords (kStartingXStatusValue, kStartingYStatusValue);

  //Show the player icon
  if (player_has_treasure_)
    DrawGamePiece(PiecesForDisplay::kTileWithPlayerAndTreasure, coords);
  else
    DrawGamePiece(PiecesForDisplay::kTileWithPlayer, coords);

  //show the player moves remaining
  GotoXY(kStartingXStatusValue + 5, kStartingYStatusValue);
  if (player_moves_remaining_ == 1)
    cout<< player_moves_remaining_ << " move remaining  ";
  else
    cout << player_moves_remaining_ << " moves remaining  ";

  //show or hide the dragon sleeping zzzz's
  coords.second = coords.second + 3;
  DrawGamePiece(PiecesForDisplay::kTileWithDragon, coords);
  if (dragon_is_awake_) {
    GotoXY(kStartingXStatusValue -2, kStartingYStatusValue + 2);
    cout << "  ";
    GotoXY(kStartingXStatusValue -3, kStartingYStatusValue + 3);
    cout << " ";
  } else {
    GotoXY(kStartingXStatusValue -2, kStartingYStatusValue + 2);
    cout << "zz";
    GotoXY(kStartingXStatusValue -3, kStartingYStatusValue + 3);
    cout << "z";
  }

  //Show dragon moves remaining
  GotoXY(kStartingXStatusValue + 5, kStartingYStatusValue + 3);
  if (player_moves_remaining_ == 0)
    cout << "1 move remaining  ";
  else
    cout << "0 moves remaining  ";


  //show a green block on the player or the dragon to indicate whose turn it is.
  if(player_moves_remaining_ != 0) {
    //put the green blocks beside the player icon
    SetConsoleColor(kGreen);
    GotoXY(kStartingXStatusValue - 2, kStartingYStatusValue);
    WriteCharacterToConsole(AsciiCodesForMaze::kBlock);
    GotoXY(kStartingXStatusValue - 2, kStartingYStatusValue+1);
    WriteCharacterToConsole(AsciiCodesForMaze::kBlock);
    SetConsoleColor(kWhite);

    //remove any green blocks beside the dragon icon
    GotoXY(kStartingXStatusValue - 2, kStartingYStatusValue+3);
    cout << " ";
    GotoXY(kStartingXStatusValue - 2, kStartingYStatusValue+4);
    cout << " ";

  } else {
    //remove any green blocks beside the player icon
    GotoXY(kStartingXStatusValue - 2, kStartingYStatusValue);
    cout << " ";
    GotoXY(kStartingXStatusValue - 2, kStartingYStatusValue+1);
    cout << " ";

    //put the green blocks beside the dragon icon
    SetConsoleColor(kGreen);
    GotoXY(kStartingXStatusValue - 2, kStartingYStatusValue+3);
    WriteCharacterToConsole(AsciiCodesForMaze::kBlock);
    GotoXY(kStartingXStatusValue - 2, kStartingYStatusValue+4);
    WriteCharacterToConsole(AsciiCodesForMaze::kBlock);
    SetConsoleColor(kWhite);
  }

  GotoDefaultConsoleCursorPosition();
}

void ConsoleView::GotoDefaultConsoleCursorPosition() {
  GotoXY(maze_left_edge_, maze_top_edge_ - 2);
}





void ConsoleView::DrawGameBoard() {
  GotoXY(maze_left_edge_,maze_top_edge_);

  DrawTopBorder();
  for (int i = 0; i < model_->tiles_per_column() - 1; i++)
    DrawTileRow(i, true);

  DrawTileRow(model_->tiles_per_column() - 1, false);
  DrawBottomBorder();
  GotoDefaultConsoleCursorPosition();
}



void ConsoleView::DrawGamePiece(PiecesForDisplay piece_to_draw, ConsoleCoordinates coords) {
  switch (piece_to_draw) {
    case PiecesForDisplay::kHorizontalInvisibleWall:
      GotoXY(coords.first, coords.second);
      WriteCharacterToConsole(AsciiCodesForMaze::kBlankSpace, kHorizontalSpacesForOneTile);
      break;

    case PiecesForDisplay::kHorizontalVisibleWall:
      GotoXY(coords.first, coords.second);
      WriteCharacterToConsole(AsciiCodesForMaze::kHorizontalLine, kHorizontalSpacesForOneTile);
      break;

    case PiecesForDisplay::kVerticalInvisibleWall:
      for (int i = 0; i < kVerticalSpacesForOneTile; i++) {
        GotoXY(coords.first, coords.second+i);
        WriteCharacterToConsole(AsciiCodesForMaze::kBlankSpace);
      }
      break;

    case PiecesForDisplay::kVerticalVisibleWall:
      for (int i = 0; i < kVerticalSpacesForOneTile; i++) {
        GotoXY(coords.first, coords.second+i);
        WriteCharacterToConsole(AsciiCodesForMaze::kVerticalLine);
      }
      break;

    case PiecesForDisplay::kTileThatIsEmpty:
      GotoXY(coords.first, coords.second);
      WriteCharacterToConsole(AsciiCodesForMaze::kBlankSpace,kHorizontalSpacesForOneTile);
      GotoXY(coords.first, coords.second+1);
      WriteCharacterToConsole(AsciiCodesForMaze::kBlankSpace,kHorizontalSpacesForOneTile);
      break;

    case PiecesForDisplay::kTileWithBase:
      GotoXY(coords.first, coords.second);
      cout << " /_\\";
      GotoXY(coords.first, coords.second+1);
      cout << " |_|";
      break;

    case PiecesForDisplay::kTileWithDragon:
      if(dragon_is_awake_) {
        GotoXY(coords.first, coords.second);
        cout << "<";
        SetConsoleColor(kRed);
        cout << "oo";
        SetConsoleColor(kWhite);
        cout << ">";
      } else {
        GotoXY(coords.first, coords.second);
        cout << "<";
        SetConsoleColor(kRed);
        cout << "..";
        SetConsoleColor(kWhite);
        cout << ">";
      }
      GotoXY(coords.first, coords.second+1);
      cout << "`\\/'";
      break;

    case PiecesForDisplay::kTileWithPlayer:
      GotoXY(coords.first, coords.second);
      cout << " oo ";
      GotoXY(coords.first, coords.second+1);
      cout << " -- ";
      break;

    case PiecesForDisplay::kTileWithPlayerAndTreasure:
      SetConsoleColor(kLightGreen);
      GotoXY(coords.first, coords.second);
      cout << "oo $";
      GotoXY(coords.first, coords.second+1);
      cout << "~~ $";
      SetConsoleColor(kWhite);
      break;

    default:
      //There is another piece, the blank piece. It is not drawn.
      break;
  }
  GotoDefaultConsoleCursorPosition();
}

void ConsoleView::DrawTitle() {
  //These letters are built using a font found on http://www.ascii-art.com
  cout << "   _   _  _   _ _   _   ___   _   ___   _     ___ " << endl;
  cout << "  | | | \\| | | | | | | / __> | | | - > | |   | __>" << endl;
  cout << "  | | |  \\ | | | | | | \\__ \\ | | | _ \\ | |_  | __>" << endl;
  cout << "  |_| |_\\__|  \\_/  |_| <___/ |_| |___/ |___| |___>" <<endl;
  cout << endl;
  cout << "     _____   ___   ____   ___ " << endl;
  cout << "    /     \\ / - \\ <_  /  | __>" << endl;
  cout << "    | | | | | | |  / /_  | __>" << endl;
  cout << "    |_|_|_| |_|_| /____> |___>" << endl;
}

void ConsoleView::WriteCharacterToConsole(char c, int repeat) {
  string str (repeat, c);
  cout << str;
}

void ConsoleView::WriteCharacterToConsole(char c) {
  WriteCharacterToConsole(c, 1);
}

void ConsoleView::DrawTileRow(int row_number, bool use_t) {
  //                          starting         + top border +   ((spaces per row + space for wall) * number of rows)
  int starting_y_coordinate = maze_top_edge_ + kSpaceForWall + ( (kVerticalSpacesForOneTile +kSpaceForWall) * row_number);

  for (int i = 0; i < kVerticalSpacesForOneTile; i++) {
    GotoXY(maze_left_edge_, starting_y_coordinate + i);
    WriteCharacterToConsole(AsciiCodesForMaze::kVerticalLine);

    GotoXY(maze_right_edge_ , starting_y_coordinate + i ) ;
    WriteCharacterToConsole(AsciiCodesForMaze::kVerticalLine);
  }
  if (use_t) {
    GotoXY(maze_left_edge_, starting_y_coordinate + kVerticalSpacesForOneTile ) ;
    WriteCharacterToConsole(AsciiCodesForMaze::kRightFacingT);

    GotoXY(maze_right_edge_, starting_y_coordinate + kVerticalSpacesForOneTile ) ;
    WriteCharacterToConsole(AsciiCodesForMaze::kLeftFacingT);
  }
}


void ConsoleView::DrawTopBorder() {
  GotoXY(maze_left_edge_,maze_top_edge_);

  WriteCharacterToConsole (AsciiCodesForMaze::kTopLeftCorner);
  for (int i = 0; i < model_->tiles_per_row() - 1; i++) {
    WriteCharacterToConsole (AsciiCodesForMaze::kHorizontalLine,kHorizontalSpacesForOneTile);
    WriteCharacterToConsole (AsciiCodesForMaze::kDownFacingT);
  }
  WriteCharacterToConsole (AsciiCodesForMaze::kHorizontalLine,kHorizontalSpacesForOneTile);
  WriteCharacterToConsole (AsciiCodesForMaze::kTopRightCorner);
}

void ConsoleView::DrawBottomBorder() {
  GotoXY(maze_left_edge_,maze_bottom_edge_);
  WriteCharacterToConsole (AsciiCodesForMaze::kBottomLeftCorner);
  for (int i = 0; i < model_->tiles_per_row() - 1; i++) {
    WriteCharacterToConsole (AsciiCodesForMaze::kHorizontalLine,kHorizontalSpacesForOneTile);
    WriteCharacterToConsole (AsciiCodesForMaze::kUpFacingT);
  }
  WriteCharacterToConsole (AsciiCodesForMaze::kHorizontalLine,kHorizontalSpacesForOneTile);
  WriteCharacterToConsole (AsciiCodesForMaze::kBorromRightCorner);
}


void ConsoleView::OnTileUpdate(Tile* t) {
  bool has_player = t->has_player();
  bool has_base = t->has_base();
  bool has_treasure = t->has_treasure();
  bool has_dragon = t->has_dragon();

  ConsoleCoordinates coords (GetConsoleCoordinates(t->id()));

  if (has_dragon)  //dragon case  (if dragon, show dragon even if it has player, treasure or base)
    DrawGamePiece(PiecesForDisplay::kTileWithDragon, coords);

  else if (has_player && has_treasure)  //player with treasure case.
    DrawGamePiece(PiecesForDisplay::kTileWithPlayerAndTreasure, coords);

  else if (has_player)  // player case
    DrawGamePiece(PiecesForDisplay::kTileWithPlayer, coords);

  else if (has_base)  //base case
    DrawGamePiece(PiecesForDisplay::kTileWithBase, coords);

  else  //plain tile case
    DrawGamePiece(PiecesForDisplay::kTileThatIsEmpty, coords);

}
void ConsoleView::OnWallUpdate(Wall* w) {
  ConsoleCoordinates coords (GetConsoleCoordinates(w->id()));

  if (dynamic_cast<HorizontalWall*> (w) != nullptr) {
    if (w->visible())
      DrawGamePiece(PiecesForDisplay::kHorizontalVisibleWall, coords);
    else
      DrawGamePiece(PiecesForDisplay::kHorizontalInvisibleWall, coords);

  } else { //A vertical wall
    if (w->visible())
      DrawGamePiece(PiecesForDisplay::kVerticalVisibleWall, coords);
    else
      DrawGamePiece(PiecesForDisplay::kVerticalInvisibleWall, coords);
  }

}
void ConsoleView::OnGameUpdate(GameNotifications notify_from_game, int i)
{
  switch (notify_from_game) {
    case GameNotifications::kDragonAwakened:
      dragon_is_awake_ = true;
      break;

    case GameNotifications::kPlayerFoundTreasure:
      for (int i = 0; i < 5; i++) {
        SetConsoleColor(kGreen);
        DrawGameBoard();
        Sleep(50);
        SetConsoleColor(kLightGreen);
        DrawGameBoard();
        Sleep(50);
      }
      SetConsoleColor(kWhite);
      DrawGameBoard();
      player_has_treasure_ = true;
      DrawPlayerStatus();
      break;

    case GameNotifications::kRemainingMoves:
      player_moves_remaining_ = i;
      DrawPlayerStatus();
      break;

    case GameNotifications::kPlayerLost:
      DrawWinAnimation(false);
      break;
    case GameNotifications::kPlayerWon:
      DrawWinAnimation(true);
      break;
  }

}


void ConsoleView::SetConsoleColor(WORD color){
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}


