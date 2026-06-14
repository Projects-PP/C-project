#include "../include/gui.h"
#include "../include/game_logic.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

static SDL_Window* window = NULL;
static TTF_Font* font = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* piece_textures[3][7];

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

    if (TTF_Init() != 0) {
        printf("TTF init error: %s\n", TTF_GetError());
        cleanup_gui();
        return 0;
    }
    font = TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", 20);
    if (!font) {
        printf("Font error: %s\n", TTF_GetError());
        cleanup_gui();
        return 0;
    }

    const char* file_paths[3][7] = {
        [WHITE] = { //idx 1
            [PAWN] = "assets/images/w_pawn.png",
            [KNIGHT] = "assets/images/w_knight.png",
            [BISHOP] = "assets/images/w_bishop.png",
            [ROOK] = "assets/images/w_rook.png",
            [QUEEN] = "assets/images/w_queen.png",
            [KING] = "assets/images/w_king.png"
        },
        [BLACK] = { //idx 2
            [PAWN] = "assets/images/b_pawn.png",
            [KNIGHT] = "assets/images/b_knight.png",
            [BISHOP] = "assets/images/b_bishop.png",
            [ROOK] = "assets/images/b_rook.png",
            [QUEEN] = "assets/images/b_queen.png",
            [KING] = "assets/images/b_king.png"
        }
    };

    for (int c = WHITE; c <= BLACK; c++) {
        for (int t = PAWN; t <= KING; t++) {
            piece_textures[c][t] = IMG_LoadTexture(renderer, file_paths[c][t]);
        }
    }

    return 1;
}

void cleanup_gui(void) {
    for (int c = WHITE; c <= BLACK; c++) {
        for (int t = PAWN; t <= KING; t++) {
            if (piece_textures[c][t]) {
                SDL_DestroyTexture(piece_textures[c][t]);
                piece_textures[c][t] = NULL;
            }
        }
    }
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    TTF_Quit();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

static void draw_btn_text(const char* text, SDL_Rect* btn_rect) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture) {
            int text_w = 0;
            int text_h = 0;
            SDL_QueryTexture(texture, NULL, NULL, &text_w, &text_h);
            SDL_Rect dst_rect = {
                btn_rect->x + (btn_rect->w - text_w) / 2,
                btn_rect->y + (btn_rect->h - text_h) / 2,
                text_w,
                text_h
            };
            SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
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
                x * CELL_SIZE + CELL_SIZE/8,
                y * CELL_SIZE + CELL_SIZE/8,
                3*CELL_SIZE/4,
                3*CELL_SIZE/4
            };

            if (piece_textures[p.color][p.type]) {
                SDL_RenderCopy(renderer, piece_textures[p.color][p.type], NULL, &r);
            }
        }
    }

    SDL_Rect btn_new  = { BOARD_SIZE + 20, 50, 200, 50 };
    SDL_Rect btn_undo = { BOARD_SIZE + 20, 120, 200, 50 };
    SDL_Rect btn_redo = { BOARD_SIZE + 20, 190, 200, 50 };

    SDL_Color color_new = { 74, 105, 150, 255 };
    SDL_Color color_undo = { 108, 163, 114, 255 };
    SDL_Color color_redo = { 196, 96, 96, 255 };

    SDL_SetRenderDrawColor(renderer, color_new.r, color_new.g, color_new.b, color_new.a);
    SDL_RenderFillRect(renderer, &btn_new);
    draw_btn_text("New Game", &btn_new);

    SDL_SetRenderDrawColor(renderer, color_undo.r, color_undo.g, color_undo.b, color_undo.a);
    SDL_RenderFillRect(renderer, &btn_undo);
    draw_btn_text("Undo", &btn_undo);

    SDL_SetRenderDrawColor(renderer, color_redo.r, color_redo.g, color_redo.b, color_redo.a);
    SDL_RenderFillRect(renderer, &btn_redo);
    draw_btn_text("Redo", &btn_redo);

    int status = check_game_status(state);
    if (status != 0) {
        SDL_Rect overlay = { 0, 0, BOARD_SIZE, BOARD_SIZE };
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(renderer, &overlay);
        if (status == 1) {
            if (state->turn == WHITE) {
                draw_btn_text("Checkmate! Black Wins", &overlay);
            } else {
                draw_btn_text("Checkmate! White Wins", &overlay);
            }
        } else {
            draw_btn_text("Stalemate! Draw", &overlay);
        }
    }
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
