#include <stdio.h>
#include <assert.h>
#include "../include/gui.h"

void test_mouse_click_mapping(void) {
    // Test clicking within board cell (0, 0)
    int action1 = handle_click(10, 10);
    assert(action1 == 0);

    // Test clicking within board cell (1, 1)
    int action2 = handle_click(CELL_SIZE + 10, CELL_SIZE + 10);
    assert(action2 == 9);

    int action3 = handle_click(BOARD_SIZE + 50, 75);
    assert(action3 == BTN_NEW_GAME);

    int action4 = handle_click(BOARD_SIZE + 50, 145);
    assert(action4 == BTN_UNDO);

    int action5 = handle_click(BOARD_SIZE + 50, 215);
    assert(action5 == BTN_REDO);

    int action6 = handle_click(BOARD_SIZE + 50, 285);
    assert(action6 == BTN_LOAD);

    // Test clicking outside active areas
    int action7 = handle_click(BOARD_SIZE + 5, 5);
    assert(action7 == CLICK_NONE);

    printf("Test GUI Mouse Click Mapping: PASSED\n");
}

int main(void) {
    printf("Running Chess GUI Tests...\n");
    test_mouse_click_mapping();
    printf("All Chess GUI Tests PASSED successfully!\n");
    return 0;
}
