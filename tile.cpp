//                      Invisible Maze
//  Montgomery Young          ProjectID: Final Project
//
#include "tile.h"

Tile::Tile() : GameObject() {
  has_base_ = false;
  has_player_ = false;
  has_dragon_ = false;
  has_treasure_ = false;
}

Tile::Tile(const TileFlags& tile_flags) : GameObject() {
  has_base_ = tile_flags.has_base;
  has_player_ = tile_flags.has_player;
  has_dragon_ = tile_flags.has_dragon;
  has_treasure_ = tile_flags.has_treasure;
}

void Tile::InitializeConnections() {
  left_connection_ = this;
  up_connection_ = this;
  right_connection_ = this;
  down_connection_ = this;
}

bool Tile::has_base() {
  return has_base_;
}

bool Tile::has_dragon() {
  return has_dragon_;
}

bool Tile::has_player() {
  return has_player_;
}

bool Tile::has_treasure() {
  return has_treasure_;
}
void Tile::set_has_base(bool has_base) {
  if(has_base_ != has_base) {
    has_base_ = has_base;
    NotifyListener();
  }
}

void Tile::set_has_dragon(bool has_dragon) {
  if(has_dragon_ != has_dragon) {
    has_dragon_ = has_dragon;
    NotifyListener();
  }
}

void Tile::set_has_player(bool has_player) {
  if(has_player_ != has_player) {
    has_player_ = has_player;
    NotifyListener();
  }
}

void Tile::set_has_treasure(bool has_treasure) {
  if(has_treasure_ != has_treasure) {
    has_treasure_ = has_treasure;
    NotifyListener();
  }
}

