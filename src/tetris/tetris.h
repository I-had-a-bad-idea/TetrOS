#pragma once
#include "kernel/kernel.h"
#include "kernel/stdint.h"
#include "tetris/blocks.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#define TETRIS_RENDER_TICKS 5
#define TETRIS_STEP_TICKS 10

#define POINTS_PER_LINE 100
#define POINTS_PER_TICK 1


#define BLOCK_CHAR '#'
#define FALLING_BLOCK_CHAR '+' 
#define VERTICAL_BORDER_CHAR '|'
#define HORIZONTAL_BORDER_CHAR '-'
#define EMPTY_CHAR ' '

extern char field[FIELD_WIDTH][FIELD_HEIGHT];

void init_tetris();
void tetris_step();
void tetris_render();
void end_game();
void reset_field();