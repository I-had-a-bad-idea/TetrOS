#include "tetris.h"


void init_tetris() {
    timer_register(tetris_step, 30);
    timer_register(tetris_render, 30);
}

void tetris_step() {
    return;
}

void tetris_render() {
    return;
}