#include "tetris.h"

char field[FIELD_WIDTH][FIELD_HEIGHT] = {0};
bool block_active = false;
bool game_over = false;
ActiveBlock current_block = {0};
Block rotated_block = {0};
Block held_block = {0};
int score = 0;

bool main_menu = true;

void init_tetris() {
    timer_register(tetris_step, TETRIS_STEP_TICKS);
    timer_register(tetris_render, TETRIS_RENDER_TICKS);
    game_over = false;
    score = 0;
    // Init field
    reset_field();
}

void rotate_block(Block* src, Block* dst, int direction) {
    // direction = 1 for clockwise, -1 for counter-clockwise
    for (int x = 0; x < BLOCK_ARRAY_AXIS_SIZE; x++) {
        for (int y = 0; y < BLOCK_ARRAY_AXIS_SIZE; y++) {
            
            if (direction == 1) { //clockwise
                dst->cells[x][y] = src->cells[BLOCK_ARRAY_AXIS_SIZE - 1 - y][x];
            } else { // counter-clockwise
                dst->cells[x][y] = src->cells[y][BLOCK_ARRAY_AXIS_SIZE - 1 - x];
            }
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
    if (main_menu) {
        if (get_pressed_key() == '1') {
            main_menu = false;
            game_over = false;
            clear_screen();
            reset_field();
            score = 0; // Reset score
        } else {
            return;
        }
    }

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
    bool switch_block_with_held = false;
    switch (key) {
        case 'a': desired_x--; break; // left
        case 'd': desired_x++; break; // right
        case 's': desired_y++; break; // down
        case 'q': desired_rotation = 1; break; // rotate left
        case 'e': desired_rotation = -1; break; // rotate right
        case 'c': switch_block_with_held = true; break; // switch blocks
    }
    // "Hold" queue
    if (switch_block_with_held) {
        if (held_block) {
            // Switch the two
            Block temp = held_block;
            held_block = ActiveBlock.block;
            ActiveBlock.block = temp;
        }
        else {
            // just put in buffer and then return (next step)
            held_block = ActiveBlock.block;
            return;
        }
    }


    // Rotation
    if (desired_rotation != 0) {
        rotate_block(current_block.block, &rotated_block, desired_rotation);
        // Check if block can be rotated
        if (can_move(&rotated_block, current_block.x, current_block.y)) {
            // Apply rotation
            *current_block.block = rotated_block;
        }
    }

    // Movement
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
            score += POINTS_PER_LINE;
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
    if (main_menu) { // Render main menu
        set_cursor(40, 5); // Middle of screen at the top
        print_string("TetrOS");
        set_cursor(40, 8);
        print_string("1) Start game");
        return;
    }

    if (game_over) {return;}
    score += POINTS_PER_TICK * TETRIS_RENDER_TICKS;
    reset_cursor();   // We dont need a full reset, since we overwrite it
  
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

    // Render score 
    int score_x = (FIELD_WIDTH + 1) * 2 + 10; // to the right of the field
    int score_y = 20;
    set_cursor(score_x, score_y);
    print_string("Score:");
    print_int(score);
    
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

    // Render "held" block
    if (held_block) {
        set_cursor(VIDEO_WIDTH - 15, 10);
        print_string("Held block:");
        set_cursor(VIDEO_WIDTH - 15, 12);

        for (int bx = 0; bx < BLOCK_ARRAY_AXIS_SIZE; bx++) {
            for (int by = 0; by < BLOCK_ARRAY_AXIS_SIZE; by++) {
                if (held_block.block->cells[bx][by]) {
                    int screen_x = (VIDEO_WIDTH - 15) + bx;
                    int screen_y = 12 + by;
                    write_char(screen_x, screen_y, FALLING_BLOCK_CHAR); // first copy
                    write_char(screen_x + 1, screen_y, FALLING_BLOCK_CHAR); // second copy
                }
            }
        }
    }
}

void end_game() {
    game_over = true;
    main_menu = true;  // Display main menu again
    // Render field with all filled blocks
    for (int y = 1; y < FIELD_HEIGHT; y++) {    // rows
        for (int x = 0; x < FIELD_WIDTH; x++) {  // columns

            int draw_x = x * 2 + 1;  // each x uses 2 chars; +1 for border
            write_char(draw_x, y, BLOCK_CHAR); // first copy
            write_char(draw_x + 1, y, BLOCK_CHAR);  // second copy
        }
    }
    reset_field();
}

void reset_field() {
    // Clear field
    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            field[x][y] = EMPTY_CHAR;
        }
    }
}