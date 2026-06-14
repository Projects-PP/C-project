#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include "game_logic.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BOARD_SIZE 512
#define CELL_SIZE (BOARD_SIZE / 8)
#define BTN_NEW_GAME -2
#define BTN_UNDO     -3
#define BTN_REDO     -4
#define CLICK_NONE   -1
#define BTN_LOAD     -5

int init_gui(void);
void cleanup_gui(void);
void render_game(ChessState* state, int selected_x, int selected_y);
int handle_click(int mouse_x, int mouse_y);
char* open_file_dialog(void);

#endif // GUI_H
