#pragma once
#include "kernel/kernel.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define FIELD_SIZE FIELD_WIDTH * FIELD_HEIGHT

extern char field[FIELD_SIZE];

void init_tetris();
void tetris_step();
void tetris_render();