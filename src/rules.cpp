/*
  Pikafish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
*/

#include "rules.h"

namespace Stockfish::Rules {

RepetitionRule repetitionRule = RepetitionRule::YITIAN;
DrawRule       drawRule       = DrawRule::NONE;
bool           sixtyMoveRule = false;
int            rule60MaxPly  = 134;
int            mateThreatDepth = 10;

}  // namespace Stockfish::Rules
