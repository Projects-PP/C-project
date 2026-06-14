#include "../include/game_logic.h"
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
}
static int is_move_valid_basic(ChessState* state, int from_x, int from_y, int to_x, int to_y) { // checks whether figure can physically make move on certain cell
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
    if (piece.type == KNIGHT) {
        if ((abs(dx) == 1 && abs(dy) == 2) || (abs(dx) == 2 && abs(dy) == 1)) {
            return 1;
        }
        return 0;
    }

    if (piece.type == BISHOP) {
        if (abs(dx) == abs(dy)) {
            int step_x = (dx > 0) ? 1 : -1;
            int step_y = (dy > 0) ? 1 : -1;
            int cur_x = from_x + step_x;
            int cur_y = from_y + step_y;
            while (cur_x != to_x && cur_y != to_y) {
                if (state->board[cur_y][cur_x].type != EMPTY) {
                    return 0;
                }
                cur_x += step_x;
                cur_y += step_y;
            }
            return 1;
        }
        return 0;
    }
    if (piece.type == ROOK) {
        if (dx == 0 || dy == 0) {
            int step_x = (dx == 0) ? 0 : ((dx > 0) ? 1 : -1);
            int step_y = (dy == 0) ? 0 : ((dy > 0) ? 1 : -1);
            int cur_x = from_x + step_x;
            int cur_y = from_y + step_y;
            while (cur_x != to_x || cur_y != to_y) {
                if (state->board[cur_y][cur_x].type != EMPTY) {
                    return 0;
                }
                cur_x += step_x;
                cur_y += step_y;
            }
            return 1;
        }
        return 0;
    }

    if (piece.type == QUEEN) {
        if (abs(dx) == abs(dy) || dx == 0 || dy == 0) {
            int step_x = (dx == 0) ? 0 : ((dx > 0) ? 1 : -1);
            int step_y = (dy == 0) ? 0 : ((dy > 0) ? 1 : -1);
            int cur_x = from_x + step_x;
            int cur_y = from_y + step_y;
            while (cur_x != to_x || cur_y != to_y) {
                if (state->board[cur_y][cur_x].type != EMPTY) {
                    return 0;
                }
                cur_x += step_x;
                cur_y += step_y;
            }
            return 1;
        }
        return 0;
    }

    if (piece.type == KING) {
        if (abs(dx) <= 1 && abs(dy) <= 1) {
            return 1;
        }
        return 0;
    }

    return 0;
}

int is_king_checked(ChessState* state, PieceColor king_color) {
    int kx = -1, ky = -1;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) { //searching king
            if (state->board[y][x].type == KING && state->board[y][x].color == king_color) {
                kx = x;
                ky = y;
                break;
            }
        }
        if (kx != -1) break;
    }
    if (kx == -1) return 0;
    PieceColor original_turn = state->turn;
    state->turn = (king_color == WHITE) ? BLACK : WHITE; //temporary changing turn
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (state->board[y][x].color == state->turn) {
                if (is_move_valid_basic(state, x, y, kx, ky)) {
                    state->turn = original_turn;
                    return 1;
                }
            }
        }
    }
    state->turn = original_turn;
    return 0;
}

int is_move_valid(ChessState* state, int from_x, int from_y, int to_x, int to_y) {
    if (!is_move_valid_basic(state, from_x, from_y, to_x, to_y)) {
        return 0;
    }
    ChessPiece src = state->board[from_y][from_x];
    ChessPiece dest = state->board[to_y][to_x];
    state->board[to_y][to_x] = src;
    state->board[from_y][from_x].type = EMPTY;
    state->board[from_y][from_x].color = COLOR_NONE;
    int checked = is_king_checked(state, src.color);
    state->board[from_y][from_x] = src;
    state->board[to_y][to_x] = dest;
    return !checked;
}

int apply_move(ChessState* state, int from_x, int from_y, int to_x, int to_y) {
    if (!is_move_valid(state, from_x, from_y, to_x, to_y)) {
        return 0;
    }

    ChessPiece piece = state->board[from_y][from_x];
    state->board[to_y][to_x] = piece;
    state->board[from_y][from_x].type = EMPTY;
    state->board[from_y][from_x].color = COLOR_NONE;

    if (piece.type == PAWN && (to_y == 0 || to_y == 7)) {
        state->board[to_y][to_x].type = QUEEN;
    }

    state->turn = (state->turn == WHITE) ? BLACK : WHITE;
    return 1;
}

static HistoryNode* history_head = NULL;
static HistoryNode* history_current = NULL;

void free_history(void) {
    HistoryNode* curr = history_head;
    while (curr) {
        HistoryNode* next = curr->next;
        free(curr);
        curr = next;
    }
    history_head = NULL;
    history_current = NULL;
}

void init_history(const ChessState* initial_state) {
    free_history();
    history_head = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (history_head) {
        history_head->state = *initial_state;
        history_head->prev = NULL;
        history_head->next = NULL;
        history_current = history_head;
    }
}

void push_history(const ChessState* state) {
    if (!history_current) return;
    HistoryNode* to_free = history_current->next;
    while (to_free) { //clears all next moves (in case history_current isn't last node in history)
        HistoryNode* next = to_free->next;
        free(to_free);
        to_free = next;
    }
    history_current->next = NULL;
    HistoryNode* new_node = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (new_node) {
        new_node->state = *state;
        new_node->prev = history_current;
        new_node->next = NULL;
        history_current->next = new_node;
        history_current = new_node;
    }
}

int undo_move(ChessState* state) {
    if (history_current && history_current->prev) {
        history_current = history_current->prev;
        *state = history_current->state;
        return 1;
    }
    return 0;
}

int redo_move(ChessState* state) {
    if (history_current && history_current->next) {
        history_current = history_current->next;
        *state = history_current->state;
        return 1;
    }
    return 0;
}
int check_game_status(ChessState* state) {
    int has_moves = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (state->board[y][x].color == state->turn) {
                for (int ty = 0; ty < 8; ty++) {
                    for (int tx = 0; tx < 8; tx++) {
                        if (is_move_valid(state, x, y, tx, ty)) {
                            has_moves = 1;
                            break;
                        }
                    }
                    if (has_moves) break;
                }
            }
            if (has_moves) break;
        }
    }
    if (has_moves) return 0;
    if (is_king_checked(state, state->turn)) return 1;
    return 2;
}
