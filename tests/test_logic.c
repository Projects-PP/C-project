#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../include/game_logic.h"
#include "../include/file_operations.h"

void test_setup_board(void) {
    ChessState state;
    setup_board(&state);
    assert(state.turn == WHITE);
    assert(state.board[7][0].type == ROOK);
    assert(state.board[7][0].color == WHITE);
    assert(state.board[0][4].type == KING);
    assert(state.board[0][4].color == BLACK);
    assert(state.board[4][4].type == EMPTY);
    printf("Test Setup Board: PASSED\n");
}

void test_check_detection(void) {
    ChessState state;
    memset(&state, 0, sizeof(ChessState));
    state.turn = WHITE;
    state.board[7][4].type = KING;
    state.board[7][4].color = WHITE;
    state.board[0][4].type = ROOK;
    state.board[0][4].color = BLACK;
    assert(is_king_checked(&state, WHITE) == 1);
    assert(is_king_checked(&state, BLACK) == 0);
    printf("Test Check Detection: PASSED\n");
}

void test_checkmate_detection(void) {
    ChessState state;
    memset(&state, 0, sizeof(ChessState));
    state.turn = WHITE;
    state.board[7][4].type = KING;
    state.board[7][4].color = WHITE;
    state.board[6][4].type = QUEEN;
    state.board[6][4].color = BLACK;
    state.board[5][4].type = ROOK;
    state.board[5][4].color = BLACK;
    assert(check_game_status(&state) == 1);
    printf("Test Checkmate Detection: PASSED\n");
}

void test_stalemate_detection(void) {
    ChessState state;
    memset(&state, 0, sizeof(ChessState));
    state.turn = BLACK;
    state.board[0][7].type = KING;
    state.board[0][7].color = BLACK;
    state.board[1][5].type = QUEEN;
    state.board[1][5].color = WHITE;
    state.board[2][6].type = KING;
    state.board[2][6].color = WHITE;
    assert(check_game_status(&state) == 2);
    printf("Test Stalemate Detection: PASSED\n");
}

void test_save_load(void) {
    ChessState state;
    setup_board(&state);
#ifdef _WIN32
    _mkdir("saves");
#else
    mkdir("saves", 0777);
#endif
    assert(save_game(&state, "saves/test_save.sav") == 1);
    
    ChessState loaded_state;
    assert(load_game(&loaded_state, "saves/test_save.sav") == 1);
    assert(loaded_state.turn == state.turn);
    assert(loaded_state.board[7][0].type == state.board[7][0].type);
    printf("Test Save & Load: PASSED\n");
}

int main(void) {
    printf("Running Chess Logic Tests...\n");
    test_setup_board();
    test_check_detection();
    test_checkmate_detection();
    test_stalemate_detection();
    test_save_load();
    printf("All Chess Logic Tests PASSED successfully!\n");
    return 0;
}
