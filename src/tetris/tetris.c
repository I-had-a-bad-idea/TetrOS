#include "tetris.h"

char field[FIELD_WIDTH][FIELD_HEIGHT] = {0};
bool block_active = false;
bool game_over = false;
ActiveBlock current_block = {0};

void init_tetris() {
    timer_register(tetris_step, 10);
    timer_register(tetris_render, 5);

    // Init field
    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            field[x][y] = EMPTY_CHAR;
        }
    }
}

bool can_move(Block* block, int desired_x, int desired_y) {
    for (int bx = 0; bx < BLOCK_ARRAY_AXIS_SIZE; bx++) {
        for (int by = 0; by < BLOCK_ARRAY_AXIS_SIZE; by++) {
            if (block->cells[bx][by]) {
                int field_x = desired_x + bx;
                int field_y = desired_y + by;

                // Check bounds
                if (field_y >= FIELD_HEIGHT || field_y < 0|| field_x < 0 || field_x >= FIELD_WIDTH) {
                    return false;
                }
                // Check collision with existing blocks
                if (field[field_x][field_y] == BLOCK_CHAR) {
                    return false;
                }
            }
        }
    }
    return true;
}

void tetris_step() {
    if (game_over) {return;}

    if (!block_active) {
        // Spawn new block
        int block_type = rand_range(0, 6);
        switch (block_type) {
            case 0: current_block.block = &I; break;
            case 1: current_block.block = &O; break;
            case 2: current_block.block = &T; break;
            case 3: current_block.block = &S; break;
            case 4: current_block.block = &Z; break;
            case 5: current_block.block = &J; break;
            case 6: current_block.block = &L; break;
        }
        current_block.x = FIELD_WIDTH / 2 - 2; // Center top
        current_block.y = 0;
        block_active = true;
        if (!can_move(current_block.block, current_block.x, current_block.y)) { // If block cannot be spawned
            end_game();
        }
    }

    //// Inputs
    char key = get_pressed_key();
    int desired_x = current_block.x;
    int desired_y = current_block.y;
    int desired_rotation = 0;
    switch (key) {
        case 'a': desired_x--; break; // left
        case 'd': desired_x++; break; // right
        case 's': desired_y++; break; // down
        case 'q': desired_rotation = -1; break; // rotate left
        case 'e': desired_rotation = 1; break; // rotate right
    }

    if (can_move(current_block.block, desired_x, desired_y)) {
        current_block.x = desired_x;
        current_block.y = desired_y;
    }

    //// Physics step
    bool can_move_down = can_move(current_block.block, current_block.x, current_block.y + 1); 

    // Move block down or place it if it cant move down
    if (can_move_down) {
        current_block.y++;
    } else {
        block_active = false;
        // Place block in field
        for (int bx = 0; bx < BLOCK_ARRAY_AXIS_SIZE; bx++) {
            for (int by = 0; by < BLOCK_ARRAY_AXIS_SIZE; by++) {
                if (current_block.block->cells[bx][by]) {
                    int field_x = current_block.x + bx;
                    int field_y = current_block.y + by;
                    if (field_x >= 0 && field_x < FIELD_WIDTH && field_y >= 0 && field_y < FIELD_HEIGHT) {
                        field[field_x][field_y] = BLOCK_CHAR;
                    }
                }
            }
        }
    }


    // Delete full lines
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        bool line_full = true;
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (field[x][y] == EMPTY_CHAR || field[x][y] == FALLING_BLOCK_CHAR) { // also check for falling block chars, since they are not actually placed yet
                line_full = false;
                break;
            }
        }
        if (line_full) {
            // Move all lines above down
            for (int ty = y; ty > 0; ty--) {
                for (int x = 0; x < FIELD_WIDTH; x++) {
                    field[x][ty] = field[x][ty - 1];
                }
            }
            // Clear top line
            for (int x = 0; x < FIELD_WIDTH; x++) {
                field[x][0] = EMPTY_CHAR;
            }
        }
    }
}

void tetris_render() {
    if (game_over) {return;}

    // Render field
    for (int y = 1; y <= FIELD_HEIGHT; y++) {    // rows
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
                if (current_block.block->cells[bx][by]) {
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


    // Render borders

    // Render left and right borders
    int x1 = 0;
    int x2 = (FIELD_WIDTH + 1) * 2 - 1;  // *2 since double chars
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        write_char(x1, y, VERTICAL_BORDER_CHAR);
        write_char(x2, y, VERTICAL_BORDER_CHAR);
    }
    // Render top and bottom borders   
    int y1 = 0;
    int y2 = FIELD_HEIGHT;
    for (int x = 0; x < (FIELD_WIDTH + 1) * 2; x++) {
        write_char(x, y1, HORIZONTAL_BORDER_CHAR);
        write_char(x, y2, HORIZONTAL_BORDER_CHAR);
    }
}

void end_game() {
    game_over = true;
    // Render field with all filled blocks
    for (int y = 1; y < FIELD_HEIGHT; y++) {    // rows
        for (int x = 0; x < FIELD_WIDTH; x++) {  // columns

            int draw_x = x * 2 + 1;  // each x uses 2 chars; +1 for border
            write_char(draw_x, y, BLOCK_CHAR); // first copy
            write_char(draw_x + 1, y, BLOCK_CHAR);  // second copy
        }
    }
}