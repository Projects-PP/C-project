#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "../include/gui.h"
#include "../include/game_logic.h"
#include "../include/file_operations.h"

int main(void) {
    ChessState state;
    setup_board(&state);
    init_history(&state);

    if (!init_gui()) return 1;

    int running = 1;
    int selected_x = -1, selected_y = -1;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int action = handle_click(e.button.x, e.button.y);

                if (action >= 0) {
                    int x = action % 8;
                    int y = action / 8;

                    if (selected_x == -1)
                        selected_x = x, selected_y = y;
                    else {
                        if (apply_move(&state, selected_x, selected_y, x, y))
                            push_history(&state);

                        selected_x = selected_y = -1;
                    }
                }
                else if (action == BTN_NEW_GAME) {
                    setup_board(&state);
                    init_history(&state);
                }
                else if (action == BTN_UNDO) {
                    undo_move(&state);
                }
                else if (action == BTN_REDO) {
                    redo_move(&state);
                }
            }
        }

        render_game(&state, selected_x, selected_y);
        SDL_Delay(16);
    }

    cleanup_gui();
    return 0;
}
