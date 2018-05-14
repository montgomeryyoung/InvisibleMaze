#include "blankspace.h"

BlankSpace::BlankSpace() :GameObject() {
}

void BlankSpace::InitializeConnections() {
  left_connection_ = nullptr;
  up_connection_ = nullptr;
  right_connection_ = nullptr;
  down_connection_ = nullptr;
}
