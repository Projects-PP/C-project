#include "../include/gui.h"
#include "../include/game_logic.h"
#include <SDL2/SDL.h>
#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

int init_gui(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow(
        "Chess GUI",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0
    );

    if (!window) {
        printf("Window error: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    return 1;
}

void cleanup_gui(void) {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

static void draw_board(ChessState* state, int selected_x, int selected_y) {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            SDL_Rect r = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };

            if ((x + y) % 2 == 0)
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
            else
                SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);

            SDL_RenderFillRect(renderer, &r);

            if (x == selected_x && y == selected_y) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 120);
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            ChessPiece p = state->board[y][x];
            if (p.type == EMPTY) continue;

            SDL_Rect r = {
                x * CELL_SIZE + CELL_SIZE/4,
                y * CELL_SIZE + CELL_SIZE/4,
                CELL_SIZE/2,
                CELL_SIZE/2
            };

            if (p.color == WHITE)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

            SDL_RenderFillRect(renderer, &r);
        }
    }

    SDL_Rect btn_new  = { BOARD_SIZE + 20, 50, 200, 50 };
    SDL_Rect btn_undo = { BOARD_SIZE + 20, 120, 200, 50 };
    SDL_Rect btn_redo = { BOARD_SIZE + 20, 190, 200, 50 };

    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_RenderFillRect(renderer, &btn_new);

    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
    SDL_RenderFillRect(renderer, &btn_undo);

    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_RenderFillRect(renderer, &btn_redo);
}

void render_game(ChessState* state, int selected_x, int selected_y) {
    draw_board(state, selected_x, selected_y);
    SDL_RenderPresent(renderer);
}

int handle_click(int mouse_x, int mouse_y) {
    if (mouse_x < BOARD_SIZE && mouse_y < BOARD_SIZE) {
        int x = mouse_x / CELL_SIZE;
        int y = mouse_y / CELL_SIZE;
        return y * 8 + x;
    }

    if (mouse_x > BOARD_SIZE + 20 && mouse_x < BOARD_SIZE + 220) {
        if (mouse_y >= 50 && mouse_y <= 100) return BTN_NEW_GAME;
        if (mouse_y >= 120 && mouse_y <= 170) return BTN_UNDO;
        if (mouse_y >= 190 && mouse_y <= 240) return BTN_REDO;
    }

    return CLICK_NONE;
}
