/*
  Pikafish - a UCI xiangqi engine derived from Stockfish
  Copyright (C) 2026 The Pikafish developers

  Persistent experience-file learning ("learn from played games").

  The module keeps a small persistent table mapping position keys to
  {best move, score, depth, bound} entries. Entries are loaded from a
  plain-text file at startup, are consulted during search when the
  transposition table has no data for a position, and are updated with
  the concluded best move after every completed root search. The file
  is saved on "ucinewgame" and on "quit".

  File format (one entry per line, '|' separated):
    <fen> | <move-uci> | <score-cp> | <depth> | <bound: exact|lower|upper>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
*/

#ifndef EXPERIENCE_H_INCLUDED
#define EXPERIENCE_H_INCLUDED

#include <cstddef>
#include <string>

#include "types.h"

namespace Stockfish {

class Position;

namespace Experience {

struct Entry {
    Move        move;
    Value       value;  // centipawn score, side-to-move perspective, never a mate score
    Depth       depth;
    Bound       bound;
    std::string fen;  // kept to be able to save the table back to disk
};

void init(const std::string& path);  // (re)load the experience file
void save();                         // persist the table to the current file

void set_learning(bool b);
bool learning_enabled();

const Entry* probe(Key key);
void         store(const std::string& fen, Key key, Move m, Value v, Depth d, Bound b);

std::size_t size();
std::string current_file();

}  // namespace Experience
}  // namespace Stockfish

#endif  // EXPERIENCE_H_INCLUDED
