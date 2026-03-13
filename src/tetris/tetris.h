#pragma once
#include "kernel/kernel.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

extern char field[FIELD_WIDTH][FIELD_HEIGHT];

void init_tetris();
void tetris_step();
void tetris_render();