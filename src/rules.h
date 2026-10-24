/*
  Pikafish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
*/

#ifndef RULES_H_INCLUDED
#define RULES_H_INCLUDED

namespace Stockfish::Rules {

enum class RepetitionRule {
    ASIAN,
    CHINESE,
    SKY,
    COMPUTER,
    YITIAN,
    ALLOW_CHASE,
    NO_JUDGEMENT
};

enum class DrawRule {
    NONE,
    DRAW_AS_BLACK_WIN,
    DRAW_AS_RED_WIN,
    DRAW_REP_AS_BLACK_WIN,
    DRAW_REP_AS_RED_WIN
};

extern RepetitionRule repetitionRule;
extern DrawRule       drawRule;
extern bool           sixtyMoveRule;
extern int            rule60MaxPly;
extern int            mateThreatDepth;

inline bool chinese_rule() { return repetitionRule == RepetitionRule::CHINESE; }
inline bool sky_rule() { return repetitionRule == RepetitionRule::SKY; }
inline bool computer_rule() { return repetitionRule == RepetitionRule::COMPUTER; }
inline bool yitian_rule() { return repetitionRule == RepetitionRule::YITIAN; }
inline bool allow_chase() { return repetitionRule == RepetitionRule::ALLOW_CHASE; }
inline bool no_judgement() { return repetitionRule == RepetitionRule::NO_JUDGEMENT; }
inline bool chinese_or_sky() { return chinese_rule() || sky_rule(); }

}  // namespace Stockfish::Rules

#endif  // #ifndef RULES_H_INCLUDED
