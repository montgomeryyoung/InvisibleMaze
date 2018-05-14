//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//

#include "mazecontroller.h"
#include <string>
#include <conio.h>
//#include <iostream>

#include "gamedata.h"
#include "consoleview.h"


MazeController::MazeController() {
}

MazeController::MazeController(GameData* game, ConsoleView* view) {
  model_ = game;
  view_ = view;
  tiles_per_row_ = 0;
  tiles_per_column_ = 0;
}

void MazeController::BuildMazePieceFromTriplet(string triplet) {
  if (triplet == triplets_[GamePieces::kBlank])
    model_->AddPieceToGameboard(GamePieces::kBlank);

  else if  (triplet == triplets_[GamePieces::kHorizontalOpenWall])
    model_->AddPieceToGameboard(GamePieces::kHorizontalOpenWall);

  else if  (triplet == triplets_[GamePieces::kHorizontalSolidWall])
    model_->AddPieceToGameboard(GamePieces::kHorizontalSolidWall);

  else if  (triplet == triplets_[GamePieces::kPlainTile])
    model_->AddPieceToGameboard(GamePieces::kPlainTile);

  else if  (triplet == triplets_[GamePieces::kTileWithBase])
    model_->AddPieceToGameboard(GamePieces::kTileWithBase);

  else if  (triplet == triplets_[GamePieces::kTileWithDragon])
    model_->AddPieceToGameboard(GamePieces::kTileWithDragon);

  else if  (triplet == triplets_[GamePieces::kTileWithTreasure])
    model_->AddPieceToGameboard(GamePieces::kTileWithTreasure);

  else if  (triplet == triplets_[GamePieces::kVerticalOpenWall])
    model_->AddPieceToGameboard(GamePieces::kVerticalOpenWall);

  else  //assume (triplet == triplets_[GamePieces::kVerticalSolidWall])
    model_->AddPieceToGameboard(GamePieces::kVerticalSolidWall);
}

void MazeController::ReadRowOfGamePieces(string row_of_pieces) {
  //If the row starts with a 'T' then this is a tile row. Therefore,
  //each column of the maze must have one more tile than previously known.
  //(that is why tiles_per_column is incremented)
  //Also, because this is a tile row, the number of tiles in the row is recalculated.
  //This means the number of tiles in a row is recalculated for every tile row.
  //But that doesn't really hurt performance much.
  if(row_of_pieces[0] == 'T') {
    tiles_per_column_++;
    tiles_per_row_ = 0;
  }

  unsigned int i = 0;
  //i+2 is the index for the end of a triplet
  while (i+2 < row_of_pieces.length()) {
      string triplet = row_of_pieces.substr(i,3);
      BuildMazePieceFromTriplet(triplet);
      if (row_of_pieces[i] == 'T')
          tiles_per_row_++;
      i +=4;
  }
}

void MazeController::BuildSmallMaze() {
  ReadRowOfGamePieces("TPL WVO TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVS TPL WVO TPL ");
  ReadRowOfGamePieces("WHS __B WHO __B WHS ");
  ReadRowOfGamePieces("TPL WVO TPL WVO TPL ");
}


void MazeController::BuildMediumMaze() {
  ReadRowOfGamePieces("TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL ");
  ReadRowOfGamePieces("WHO __B WHS __B WHO __B WHS __B WHS __B WHS __B WHS __B WHS ");
  ReadRowOfGamePieces("TPL WVS TPL WVS TPL WVS TPL WVO TPL WVO TPL WVO TPL WVO TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHO __B WHO __B WHS __B WHS __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVO TPL WVS TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL ");
  ReadRowOfGamePieces("WHS __B WHS __B WHO __B WHS __B WHS __B WHS __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHS __B WHS __B WHS __B WHS __B WHS __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVS TPL WVO TPL WVO TPL WVO TPL WVO TPL WVS TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHS __B WHS __B WHS __B WHO __B WHO __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL WVS TPL WVO TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHS __B WHO __B WHO __B WHO __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHO __B WHO __B WHO __B WHO __B WHS __B WHO ");
  ReadRowOfGamePieces("TPL WVO TPL WVO TPL WVS TPL WVS TPL WVS TPL WVO TPL WVO TPL ");
}

void MazeController::BuildLargeMaze() {
  ReadRowOfGamePieces("TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVS TPL WVO TPL WVO TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHS __B WHO __B WHS __B WHS __B WHS __B WHS __B WHS __B WHS __B WHS __B WHO __B WHO __B WHS __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVS TPL WVS TPL WVS TPL WVO TPL WVO TPL WVO TPL WVO TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL WVO TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHO __B WHO __B WHS __B WHS __B WHO __B WHO __B WHO __B WHS __B WHO __B WHO __B WHO __B WHS __B WHO ");
  ReadRowOfGamePieces("TPL WVO TPL WVS TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVO TPL ");
  ReadRowOfGamePieces("WHS __B WHS __B WHO __B WHS __B WHS __B WHS __B WHO __B WHO __B WHO __B WHO __B WHS __B WHO __B WHO __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVS TPL WVS TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHS __B WHS __B WHS __B WHS __B WHS __B WHO __B WHO __B WHO __B WHO __B WHO __B WHO __B WHO __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVS TPL WVO TPL WVO TPL WVO TPL WVO TPL WVS TPL WVS TPL WVO TPL WVS TPL WVO TPL WVS TPL WVS TPL WVS TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHS __B WHS __B WHS __B WHO __B WHO __B WHO __B WHO __B WHO __B WHS __B WHS __B WHO __B WHO __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL WVS TPL WVO TPL WVS TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHS __B WHO __B WHO __B WHO __B WHO __B WHO __B WHO __B WHS __B WHS __B WHS __B WHS __B WHS __B WHO ");
  ReadRowOfGamePieces("TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHO __B WHO __B WHO __B WHO __B WHS __B WHO __B WHO __B WHS __B WHS __B WHS __B WHS __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVO TPL WVS TPL WVS TPL WVS TPL WVS TPL WVO TPL WVO TPL WVS TPL WVO TPL WVO TPL WVO TPL WVO TPL WVS TPL WVS TPL ");
  ReadRowOfGamePieces("WHS __B WHS __B WHO __B WHO __B WHO __B WHS __B WHO __B WHO __B WHS __B WHS __B WHS __B WHS __B WHS __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVO TPL WVO TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL WVO TPL WVO TPL WVO TPL WVS TPL WVO TPL WVO TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHS __B WHS __B WHS __B WHS __B WHO __B WHO __B WHS __B WHS __B WHO __B WHO __B WHS __B WHS __B WHO ");
  ReadRowOfGamePieces("TPL WVS TPL WVS TPL WVO TPL WVO TPL WVS TPL WVO TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL WVO TPL WVO TPL WVO TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHO __B WHO __B WHO __B WHO __B WHO __B WHS __B WHS __B WHO __B WHO __B WHO __B WHS __B WHS __B WHO ");
  ReadRowOfGamePieces("TPL WVS TPL WVO TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL WVS TPL WVO TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHS __B WHO __B WHO __B WHO __B WHO __B WHO __B WHO __B WHS __B WHO __B WHO __B WHO __B WHO __B WHO __B WHO ");
  ReadRowOfGamePieces("TPL WVO TPL WVO TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL WVS TPL WVS TPL WVO TPL ");
  ReadRowOfGamePieces("WHS __B WHS __B WHS __B WHO __B WHO __B WHS __B WHO __B WHO __B WHS __B WHS __B WHO __B WHO __B WHO __B WHS __B WHO ");
  ReadRowOfGamePieces("TPL WVO TPL WVO TPL WVO TPL WVS TPL WVS TPL WVO TPL WVO TPL WVS TPL WVO TPL WVO TPL WVS TPL WVS TPL WVO TPL WVS TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHS __B WHS __B WHO __B WHO __B WHS __B WHO __B WHS __B WHO __B WHO __B WHO __B WHS __B WHS __B WHS ");
  ReadRowOfGamePieces("TPL WVS TPL WVS TPL WVO TPL WVO TPL WVO TPL WVS TPL WVS TPL WVS TPL WVS TPL WVS TPL WVO TPL WVO TPL WVO TPL WVO TPL ");
  ReadRowOfGamePieces("WHO __B WHO __B WHS __B WHS __B WHS __B WHS __B WHO __B WHS __B WHO __B WHO __B WHO __B WHS __B WHS __B WHS __B WHO ");
  ReadRowOfGamePieces("TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVO TPL WVS TPL WVS TPL WVO TPL WVO TPL WVO TPL ");
}

void MazeController::PlayAgainLoop() {
  string input_for_keep_playing = "Medium";

  while (toupper(input_for_keep_playing[0]) != 'N') {
    tiles_per_column_ = 0;
    model_->SetupGame();

    switch (toupper(input_for_keep_playing[0])) {
      case 'S':
        BuildSmallMaze();
        break;
      case 'M':
        BuildMediumMaze();
        break;
      case 'L':
      default:
        BuildLargeMaze();
        break;
    }
    model_->set_tiles_per_row(tiles_per_row_);
    model_->set_tiles_per_column(tiles_per_column_);
    model_->RandomlyAssignBaseTreasureAndDragon();
    model_->ConnectGameboardPiecesTogether();
    view_->InitializeDisplay();
    model_->set_pause_before_dragon_move(100);
    model_->PlayGame();

    GameLoop();

    cout << " Play Again? (Small, Medium, Large, No) ";
    cin >> input_for_keep_playing;
    model_->ClearGameboard();
  }
}

void MazeController::GameLoop()
{
  const int kLeftArrow=75, kRightArrow = 77, kUpArrow=72, kDownArrow=80, kArrowIsComing=224, kEscapeKey = 27;
  char next_direction_to_move = kUpArrow;

  do {
    if (kbhit()) {
      next_direction_to_move = getch();

      if (next_direction_to_move == kArrowIsComing) next_direction_to_move = getch();
      switch (next_direction_to_move) {
        case 'w':
        case kUpArrow:
          model_->MovePlayer(MoveDirections::kUp);
          break;
        case 'a':
        case kLeftArrow:
          model_->MovePlayer(MoveDirections::kLeft);
          break;
        case 's':
        case kDownArrow:
          model_->MovePlayer(MoveDirections::kDown);
          break;
        case 'd':
        case kRightArrow:
          model_->MovePlayer(MoveDirections::kRight);
          break;
        case '.':
          model_->ShowAllWalls();
          break;
      }
    }
  } while ((next_direction_to_move!= kEscapeKey) && (model_->game_state() == GameState::kPlay));
}
