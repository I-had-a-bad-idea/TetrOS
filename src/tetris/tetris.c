#include "tetris.h"

char field[FIELD_WIDTH][FIELD_HEIGHT] = {};
bool block_active = false;
Block current_block;

void init_tetris() {
    timer_register(tetris_step, 30);
    timer_register(tetris_render, 30);

    // Init field
    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            field[x][y] = EMPTY_CHAR;
        }
    }
}

void tetris_step() {
    if (!block_active) {
        // Spawn new block
        int block_type = rand_range(0, 6);
        switch (block_type) {
            case 0: current_block = I; break;
            case 1: current_block = O; break;
            case 2: current_block = T; break;
            case 3: current_block = S; break;
            case 4: current_block = Z; break;
            case 5: current_block = J; break;
            case 6: current_block = L; break;
        }
        current_block.x = FIELD_WIDTH / 2 - 2; // Center top
        current_block.y = 0;
        block_active = true;
    }

    //// Physics step
    bool can_move_down = true;
    for (int bx = 0; bx < BLOCK_ARRAY_AXIS_SIZE; bx++) {
        for (int by = BLOCK_ARRAY_AXIS_SIZE-1; by >= 0; by--) { // check from bottom to top, so we can break earlier if we find a collision
            if (current_block.cells[bx][by]) {
                int field_x = current_block.x + bx;
                int field_y = current_block.y + by + 1; // Check one below

                // Check bounds
                if (field_y >= (FIELD_HEIGHT - 1) || field_x < 0 || field_x >= FIELD_WIDTH) {
                    can_move_down = false;
                    break;
                }
                // Check collision with existing blocks
                if (field[field_x][field_y] == BLOCK_CHAR) { // cant do != EMPTY_CHAR since we want to allow falling blocks to move through each other
                    can_move_down = false;
                    break;
                }
            }
        }
        if (!can_move_down) break;
    }
    if (can_move_down) {
        current_block.y++;
    } else {
        block_active = false;
        // Place block in field
        for (int bx = 0; bx < BLOCK_ARRAY_AXIS_SIZE; bx++) {
            for (int by = 0; by < BLOCK_ARRAY_AXIS_SIZE; by++) {
                if (current_block.cells[bx][by]) {
                    int field_x = current_block.x + bx;
                    int field_y = current_block.y + by;
                    if (field_x >= 0 && field_x < FIELD_WIDTH && field_y >= 0 && field_y < FIELD_HEIGHT) {
                        field[field_x][field_y] = BLOCK_CHAR;
                    }
                }
            }
        }
    }
}

void tetris_render() {
    reset_cursor();   // We dont need a full reset, since we overwrite it

    //// Render field

    for (int y = 0; y < FIELD_HEIGHT; y++) {    // rows
        for (int x = 0; x < FIELD_WIDTH; x++) {  // columns
            char c = field[x][y];

            int draw_x = x * 2 + 1;  // each x uses 2 chars; +1 for border
            write_char(draw_x, y, c); // first copy
            write_char(draw_x + 1, y, c);  // second copy
        }
    }

    // Render falling block
    if (block_active) {
        for (int bx = 0; bx < BLOCK_ARRAY_AXIS_SIZE; bx++) {
            for (int by = 0; by < BLOCK_ARRAY_AXIS_SIZE; by++) {
                if (current_block.cells[bx][by]) {
                    int field_x = current_block.x + bx;
                    int field_y = current_block.y + by;
                    if (field_x >= 0 && field_x < FIELD_WIDTH && field_y >= 0 && field_y < FIELD_HEIGHT) {
                        int draw_x = field_x * 2 + 1; // each x uses 2 chars; +1 for border
                        write_char(draw_x, field_y, FALLING_BLOCK_CHAR); // first copy
                        write_char(draw_x + 1, field_y, FALLING_BLOCK_CHAR); // second copy
                    }
                }
            }
        }
    }

    //// Render borders

    // Render left and right borders
    int x1 = 0;
    int x2 = (FIELD_WIDTH + 1) * 2 - 1;  // *2 since double chars
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        write_char(x1, y, VERTICAL_BORDER_CHAR);
        write_char(x2, y, VERTICAL_BORDER_CHAR);
    }
    // Render top and bottom borders   
    int y1 = 0;
    int y2 = FIELD_HEIGHT - 1;
    for (int x = 0; x < (FIELD_WIDTH + 1) * 2; x++) {
        write_char(x, y1, HORIZONTAL_BORDER_CHAR);
        write_char(x, y2, HORIZONTAL_BORDER_CHAR);
    }
}