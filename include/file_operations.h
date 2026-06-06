#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "game_logic.h"

int save_game(const char* filepath);
int load_game(const char* filepath, ChessState* out_state);
void log_event(const char* format, ...);

#endif // FILE_OPERATIONS_H
