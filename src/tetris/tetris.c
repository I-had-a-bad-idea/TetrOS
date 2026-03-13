#include "tetris.h"


void init_tetris() {
    timer_register(tetris_step, 30);
    timer_register(tetris_render, 30);
}

void tetris_step() {
    return;
}

void tetris_render() {
    for(int i = 0; i < FIELD_SIZE; i++) {
        print_char(field[i]);
        if (i % FIELD_WIDTH == 0) {
            print_char('\n');
        } 
    }
}