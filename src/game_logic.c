#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>

void setup_board(ChessState* state) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            state->board[y][x].type = EMPTY;
            state->board[y][x].color = COLOR_NONE;
        }
    }

    int user_rows[3];
    user_rows[WHITE] = 7;
    user_rows[BLACK] = 0;

    PieceType layout[8] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };

    for (int c = WHITE; c <= BLACK; c++) {
        int y = user_rows[c];
        for (int x = 0; x < 8; x++) {
            state->board[y][x].type = layout[x];
            state->board[y][x].color = (PieceColor)c;
        }
    }

    for (int x = 0; x < 8; x++) {
        state->board[1][x].type = PAWN;
        state->board[1][x].color = BLACK;
        state->board[6][x].type = PAWN;
        state->board[6][x].color = WHITE;
    }
    
    state->turn = WHITE;
    for (int c = WHITE; c <= BLACK; c++) {
        for (int x = 0; x <= 1; x++) {
            state->castling_rights[c][x] = 1;
        }
    }
}

int is_move_valid(ChessState* state, int from_x, int from_y, int to_x, int to_y) {
    if (from_x < 0 || from_x >= 8 || from_y < 0 || from_y >= 8 ||
        to_x < 0 || to_x >= 8 || to_y < 0 || to_y >= 8) {
        return 0;
    }

    ChessPiece piece = state->board[from_y][from_x];
    if (piece.type == EMPTY || piece.color != state->turn) {
        return 0;
    }

    ChessPiece dest = state->board[to_y][to_x];
    if (dest.type != EMPTY && dest.color == piece.color) {
        return 0;
    }

    int dx = to_x - from_x;
    int dy = to_y - from_y;

    if (piece.type == PAWN) {
        int dir = (piece.color == WHITE) ? -1 : 1;
        int start_row = (piece.color == WHITE) ? 6 : 1;

        if (dx == 0 && dy == dir && dest.type == EMPTY) {
            return 1;
        }
        if (dx == 0 && dy == 2 * dir && from_y == start_row && dest.type == EMPTY) {
            if (state->board[from_y + dir][from_x].type == EMPTY) {
                return 1;
            }
        }
        if (abs(dx) == 1 && dy == dir && dest.type != EMPTY && dest.color != piece.color) {
            return 1;
        }
        return 0;
    }

    return 0;
}

int is_king_checked(ChessState* state, PieceColor king_color) {
    (void)state; (void)king_color;
    return 0;
}

int apply_move(ChessState* state, int from_x, int from_y, int to_x, int to_y) {
    (void)state; (void)from_x; (void)from_y; (void)to_x; (void)to_y;
    return 0;
}

void init_history(ChessState initial_state) { (void)initial_state; }
void push_history(ChessState state) { (void)state; }
int undo_move(ChessState* state) { (void)state; return 0; }
int redo_move(ChessState* state) { (void)state; return 0; }
void free_history(void) {}
