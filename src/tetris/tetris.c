#include "tetris.h"

char field[FIELD_WIDTH][FIELD_HEIGHT] = {};

void init_tetris() {
    timer_register(tetris_step, 30);
    timer_register(tetris_render, 30);

    // Init field
    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            field[x][y] = FALLING_BLOCK_CHAR;
        }
    }
}

void tetris_step() {
    return;
}

void tetris_render() {
    reset_cursor();   // We dont need a full reset, since we overwrite it

    //// Render field

    for (int y = 0; y < FIELD_HEIGHT; y++) {    // rows
        for (int x = 0; x < FIELD_WIDTH; x++) {  // columns
            char c = field[x][y];

            int draw_x = x * 2 + 1;  // each x uses 2 chars; +1 for border
            write_char(draw_x, y, c); // first copy
            write_char(draw_x + 1, y, c);  // second copy
        }
    }

    //// Render borders


    // Render left and right borders
    int x1 = 0;
    int x2 = (FIELD_WIDTH + 1) * 2 - 1;  // *2 since double chars
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        write_char(x1, y, VERTICAL_BORDER_CHAR);
        write_char(x2, y, VERTICAL_BORDER_CHAR);
    }
    // Render top and bottom borders   
    int y1 = 0;
    int y2 = FIELD_HEIGHT - 1;
    for (int x = 0; x < (FIELD_WIDTH + 1) * 2; x++) {
        write_char(x, y1, HORIZONTAL_BORDER_CHAR);
        write_char(x, y2, HORIZONTAL_BORDER_CHAR);
    }
}