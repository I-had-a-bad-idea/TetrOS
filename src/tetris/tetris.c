#include "tetris.h"

char field[FIELD_WIDTH][FIELD_HEIGHT] = {};

void init_tetris() {
    timer_register(tetris_step, 30);
    timer_register(tetris_render, 30);
}

void tetris_step() {
    return;
}

void tetris_render() {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        print_string(field[i]); // Print row
    }
    print_char('\n');
}