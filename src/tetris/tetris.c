#include "tetris.h"

char field[FIELD_HEIGHT][FIELD_WIDTH] = {};

void init_tetris() {
    timer_register(tetris_step, 30);
    timer_register(tetris_render, 30);
}

void tetris_step() {
    return;
}

void tetris_render() {
    reset_cursor();   // We dont need a full reset, since we overwrite it
    for (int x = 0; x < FIELD_HEIGHT; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            write_char(x, y, field[x][y]);         // Render the coordinates
        }
    }
}