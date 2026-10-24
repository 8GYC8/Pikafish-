/*
  Pikafish - a UCI xiangqi engine derived from Stockfish
  Copyright (C) 2026 The Pikafish developers

  Persistent experience-file learning. See experience.h for the file format.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
*/

#include "experience.h"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "position.h"
#include "uci.h"

namespace Stockfish {
namespace Experience {

namespace {
std::unordered_map<Key, Entry> table;
std::string                    path;
bool                           learning = true;
bool                           loaded   = false;

std::string trim(const std::string& s) {
    const auto b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos)
        return "";
    const auto e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

std::string bound_to_string(const Entry& e) {
    if (e.book)
        return "book";
    return e.bound == BOUND_EXACT ? "exact" : e.bound == BOUND_LOWER ? "lower" : "upper";
}

Bound bound_from_string(const std::string& s) {
    return s == "lower" ? BOUND_LOWER : s == "upper" ? BOUND_UPPER : BOUND_EXACT;
}
}  // namespace

void init(const std::string& p) {
    path = p;
    table.clear();
    loaded = false;

    std::ifstream in(path);
    if (!in)
        return;  // No experience file yet: start empty, create on first save.

    std::string line;
    while (std::getline(in, line))
    {
        line = trim(line);
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream ss(line);
        std::string        fen, moveStr, scoreStr, depthStr, boundStr;
        if (!std::getline(ss, fen, '|') || !std::getline(ss, moveStr, '|')
            || !std::getline(ss, scoreStr, '|') || !std::getline(ss, depthStr, '|')
            || !std::getline(ss, boundStr, '|'))
            continue;

        fen     = trim(fen);
        moveStr = trim(moveStr);

        StateInfo st;
        Position  pos;
        if (pos.set(fen, &st))
            continue;  // skip malformed FEN

        Move m = UCIEngine::to_move(pos, moveStr);
        if (m == Move::none())
            continue;

        // Parse score/depth without exceptions (the engine is built with
        // -fno-exceptions): strtol-based with validation.
        const std::string sScore = trim(scoreStr), sDepth = trim(depthStr);
        char*             end = nullptr;
        const long        score = std::strtol(sScore.c_str(), &end, 10);
        if (end == sScore.c_str() || *end != '\0')
            continue;
        const long depth = std::strtol(sDepth.c_str(), &end, 10);
        if (end == sDepth.c_str() || *end != '\0' || depth <= 0)
            continue;

        Entry e;
        e.move  = m;
        e.value = Value(score);
        e.depth = Depth(depth);
        e.bound = bound_from_string(trim(boundStr));
        e.book  = trim(boundStr) == "book";
        e.fen   = fen;
        table[pos.key()] = e;
    }

    loaded = true;
    sync_cout << "info string Experience: loaded " << table.size() << " entries from " << path
              << sync_endl;
}

void save() {
    if (path.empty() || (table.empty() && !loaded))
        return;

    namespace fs = std::filesystem;
    const fs::path fp = fs::path(path_from_utf8(path));
    if (fp.has_parent_path())
    {
        std::error_code ec;
        fs::create_directories(fp.parent_path(), ec);
    }

    const std::string tmp = path + ".tmp";
    {
        std::ofstream out(tmp, std::ios::trunc);
        if (!out)
            return;
        out << "# Pikafish experience file: <fen> | <move> | <score-cp> | <depth> | <bound>\n"
            << "# bound 'book' = play this move at the root (learned from studied games)\n";
        for (const auto& [key, e] : table)
            out << e.fen << " | " << UCIEngine::move(e.move) << " | " << int(e.value) << " | "
                << int(e.depth) << " | " << bound_to_string(e) << "\n";
    }
    std::remove(path.c_str());
    std::rename(tmp.c_str(), path.c_str());
}

void set_learning(bool b) { learning = b; }
bool learning_enabled() { return learning; }

const Entry* probe(Key key) {
    if (table.empty())
        return nullptr;
    const auto it = table.find(key);
    return it == table.end() ? nullptr : &it->second;
}

void store(const std::string& fen, Key key, Move m, Value v, Depth d, Bound b) {
    if (!learning || m == Move::none() || d <= 0)
        return;

    const auto it = table.find(key);
    if (it != table.end() && it->second.depth > d)
        return;  // keep deeper knowledge

    table[key] = Entry{m, v, d, b, false, fen};
}

std::size_t size() { return table.size(); }
std::string current_file() { return path; }

}  // namespace Experience
}  // namespace Stockfish
