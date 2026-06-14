#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "game_logic.h"

void log_event(const char* format, ...);
int save_game(const ChessState* state, const char* filepath);
int load_game(ChessState* state, const char* filepath);

#endif // FILE_OPERATIONS_H
