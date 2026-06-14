#include <SDL2/SDL.h>
#include "game_logic.h"
#include "gui.h"
#include "file_operations.h"
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#define make_dir(d) _mkdir(d)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define make_dir(d) mkdir(d, 0777)
#endif

int main(void) {
    make_dir("saves");
    make_dir("logs");

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
                time_t t = time(NULL);
                struct tm* tm_info = localtime(&t);
                char filename[64];
                strftime(filename, sizeof(filename), "saves/game_%Y%m%d_%H%M%S.sav", tm_info);
                save_game(&state, filename);
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
                        log_event("Move: %c%d -> %c%d", 'a' + selected_x, 8 - selected_y, 'a' + x, 8 - y);
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
                } else if (action == BTN_LOAD) {
                    char* path = open_file_dialog();
                    if (path && load_game(&state, path)) {
                        init_history(&state);
                    }
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
