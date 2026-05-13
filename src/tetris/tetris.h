#pragma once
#include "kernel/kernel.h"
#include "tetris/blocks.h"

typedef unsigned char       uint8_t;

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#define HELD_BLOCK_POSTION VIDEO_WIDTH - 15 
#define NEXT_BLOCK_POSITION VIDEO_WIDTH - 30

#define TETRIS_RENDER_TICKS 5
#define TETRIS_STEP_TICKS 10

#define POINTS_PER_LINE 100
#define POINTS_PER_TICK 1


#define BLOCK_CHAR '#'
#define FALLING_BLOCK_CHAR '+' 
#define PREVIEW_CHAR '.'  // char used to show where the block will land
#define VERTICAL_BORDER_CHAR '|'
#define HORIZONTAL_BORDER_CHAR '-'
#define EMPTY_CHAR ' '

static const uint8_t COLORS[8] = {RED_ON_BLACK, BLUE_ON_BLACK, CYAN_ON_BLACK, GREEN_ON_BLACK, YELLOW_ON_BLACK, MAGENTA_ON_BLACK, WHITE_ON_BLACK, BROWN_ON_BLACK};


#define CELL_EMPTY 0
#define CELL_FILLED_MASK 0x1 // 0001 <- last bit important
#define CELL_COLOR_MASK 0xE  // first 3 bits important -> 1110 

#define MAKE_CELL(filled, color) (((filled) & 0x1) | (((color) & 0x7) <<1))
#define IS_CELL_FILLED(cell) ((cell) & CELL_FILLED_MASK)
#define GET_CELL_COLOR(cell) (((cell) >> 1) & 0x7)

#define FIELD_CELL_COUNT (FIELD_WIDTH * FIELD_HEIGHT)
#define FIELD_BYTES ((FIELD_CELL_COUNT + 1) / 2)

extern uint8_t field[FIELD_BYTES];

static inline int field_index(int x, int y) {
    return y * FIELD_WIDTH + x;
}

static inline uint8_t field_get(int x, int y) {
    int idx = field_index(x, y);

    uint8_t cell_byte = field[idx / 2];

    if ((idx % 2) == 0) { // if first cell (lower 4 bitss)
        return cell_byte & 0x0F; // lower 4 bits
    } else {
        return (cell_byte >> 4) & 0x0F; // higher 4 bits
    }
}

static inline void field_set(int x, int y, uint8_t value) {
    int idx = field_index(x, y);

    uint8_t* cell_byte = &field[idx / 2];

    if ((idx % 2) == 0) { // if first cell (lower 4 bitss)
        *cell_byte = (*cell_byte & 0xF0) | (value & 0x0F); // lower 4 bits
    } else {
        *cell_byte = (*cell_byte & 0x0F) | ((value & 0x0F) << 4); // higher 4 bits
    }
}

void init_tetris();
void tetris_step();
void tetris_render();
void end_game();
void reset_field();