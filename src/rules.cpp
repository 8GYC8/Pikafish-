/*
  Pikafish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
*/

#include "rules.h"

namespace Stockfish::Rules {

// Defaults: AsianRule with rule120, Sixty Move Rule on.
// SkyRule also couples to rule120 (adjustable 90-150); YitianRule couples to
// rule140 (also adjustable 90-150) with Sixty Move Rule on. See engine.cpp
// for the active RuleConfig couplings (this namespace mirrors the defaults).
RepetitionRule repetitionRule = RepetitionRule::ASIAN;
DrawRule       drawRule       = DrawRule::NONE;
bool           sixtyMoveRule = true;
int            rule60MaxPly  = 120;
int            mateThreatDepth = 10;

}  // namespace Stockfish::Rules
