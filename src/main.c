#include <SDL2/SDL.h>
#include "game_logic.h"
#include "gui.h"

int main(void) {
    ChessState state;
    setup_board(&state);
    init_history(&state);

    if (!init_gui()) {
        return 1;
    }

    int running = 1;
    SDL_Event e; //event tracker
    int selected_x = -1;
    int selected_y = -1;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int action = handle_click(e.button.x, e.button.y);
                if (action >= 0 && check_game_status(&state) == 0) {
                    int x = action % 8;
                    int y = action / 8;
                    if (selected_x == -1) {
                        if (state.board[y][x].type != EMPTY && state.board[y][x].color == state.turn) {
                            selected_x = x;
                            selected_y = y;
                        }
                    } else {
                        if (apply_move(&state, selected_x, selected_y, x, y)) {
                            push_history(&state);
                        }
                        selected_x = -1;
                        selected_y = -1;
                    }
                } else if (action == BTN_NEW_GAME) {
                    setup_board(&state);
                    init_history(&state);
                    selected_x = -1;
                    selected_y = -1;
                } else if (action == BTN_UNDO) {
                    undo_move(&state);
                    selected_x = -1;
                    selected_y = -1;
                } else if (action == BTN_REDO) {
                    redo_move(&state);
                    selected_x = -1;
                    selected_y = -1;
                }
            }
        }
        render_game(&state, selected_x, selected_y);
        SDL_Delay(16);
    }

    cleanup_gui();
    free_history();
    return 0;
}
