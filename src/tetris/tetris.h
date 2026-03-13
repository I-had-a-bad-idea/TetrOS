#pragma once
#include "kernel/kernel.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

extern char field[FIELD_HEIGHT][FIELD_WIDTH];

void init_tetris();
void tetris_step();
void tetris_render();