#pragma once
#include "kernel/kernel.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#define BLOCK_CHAR '#'
#define FALLING_BLOCK_CHAR '+' 
#define VERTICAL_BORDER_CHAR '|'
#define HORIZONTAL_BORDER_CHAR '-'
#define EMPTY_CHAR ' '

extern char field[FIELD_WIDTH][FIELD_HEIGHT];

void init_tetris();
void tetris_step();
void tetris_render();