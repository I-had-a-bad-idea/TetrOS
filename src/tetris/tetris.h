#pragma once
#include "kernel/kernel.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define FILED_SIZE FIELD_WIDTH * FIELD_HEIGHT

char field[FILED_SIZE] = {};

void init_tetris();
void tetris_step();
void tetris_render();