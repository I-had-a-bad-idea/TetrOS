#include "tetris.h"

uint8_t field[FIELD_BYTES] = {0};
bool block_active = false;
bool block_held = false;

bool game_over = false;
volatile bool main_menu = true;

ActiveBlock current_block = {0};
Block rotated_block = {0}; // basically a temp for storing potential rotation
Block* held_block = {0};
Block* next_block = {0};

int block_land_y = 0;

int score = 0;
char score_buffer[20] = {0};

Block* get_random_block() {
    int block_type = rand_range(0, 6);
    switch (block_type) {
        case 0: return &I; break;
        case 1: return &O; break;
        case 2: return &T; break;
        case 3: return &S; break;
        case 4: return &Z; break;
        case 5: return &J; break;
        case 6: return &L; break;
    }

}

void init_tetris() {
    timer_register(tetris_step, TETRIS_STEP_TICKS);
    timer_register(tetris_render, TETRIS_RENDER_TICKS);
    game_over = false;
    main_menu = true;
    score = 0;
    // Init field
    next_block = get_random_block();
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
                if (IS_CELL_FILLED(field_get(field_x, field_y))) {
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
            next_block = get_random_block(); // Reset next block
        } else {
            return;
        }
    }

    if (game_over) {return;}

    if (!block_active) {
        // Spawn new block
        current_block.block = next_block;
        // apply color
        current_block.color = rand_range(0, 7); // 8 colors
        next_block = get_random_block();

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
    bool hard_drop = false;
    switch (key) {
        case 'a': desired_x--; break; // left
        case 'd': desired_x++; break; // right
        case 's': desired_y++; break; // down
        case 'q': desired_rotation = 1; break; // rotate left
        case 'e': desired_rotation = -1; break; // rotate right
        case 'c': switch_block_with_held = true; break; // switch blocks
        case ' ': hard_drop = true; break;
    }
    // "Hold" queue
    if (switch_block_with_held) {
        if (block_held) {
            // Switch the two
            Block* temp = held_block;
            held_block = current_block.block;
            current_block.block = temp;
        }
        else {
            block_held = true;
            // just put in buffer and then return (next step)
            held_block = current_block.block;
            block_active = false;
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
    // calculate where it will land
    block_land_y = current_block.y;
    while (can_move(current_block.block, current_block.x, block_land_y + 1)) {
        block_land_y++;
    }

    // Hard drop
    if (hard_drop) {
        current_block.y = block_land_y; // Move down as far as possible
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
                        field_set(field_x, field_y, MAKE_CELL(1, current_block.color)); // filled, color
                    }
                }
            }
        }
    }


    // Delete full lines
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        bool line_full = true;
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (!IS_CELL_FILLED(field_get(x, y))) { // also check for falling block chars, since they are not actually placed yet
                line_full = false;
                break;
            }
        }
        if (line_full) {
            score += POINTS_PER_LINE;
            // Move all lines above down
            for (int ty = y; ty > 0; ty--) {
                for (int x = 0; x < FIELD_WIDTH; x++) {
                    field_set(x, ty, field_get(x, ty - 1));
                }
            }
            // Clear top line
            for (int x = 0; x < FIELD_WIDTH; x++) {
                field_set(x, 0, CELL_EMPTY);
            }
        }
    }
}

void tetris_render() {
    if (main_menu) { // Render main menu
        if (game_over) {
            render_text_panel(VIDEO_WIDTH / 2 - 7, VIDEO_HEIGHT / 2 - 2, "Game Over!");
            render_text_panel(VIDEO_WIDTH / 2 - 12, VIDEO_HEIGHT / 2 - 1, "Press '1' to play");
            return;
        }
        render_main_menu();
        return;
    }

    if (game_over) {return;}
    score += POINTS_PER_TICK * TETRIS_RENDER_TICKS;
    clear_screen();
    render_playfield_background(); // Render background and borders
  
    // Render field
    for (int y = 0; y < FIELD_HEIGHT; y++) {    // rows
        for (int x = 0; x < FIELD_WIDTH; x++) {  // columns
            uint8_t cell = field_get(x, y);
            uint8_t color = GET_CELL_COLOR(cell);
            
            char c = EMPTY_CHAR;
            if (IS_CELL_FILLED(cell)) {
                c = BLOCK_CHAR;
            }

            int draw_x = FIELD_X + x * 2 + 1;  // each x uses 2 chars; +1 for border
            int draw_y = FIELD_Y + 1 + y;      // field rows start below the top border
            draw_char(draw_x, draw_y, c, COLORS[color]); // first copy
            draw_char(draw_x + 1, draw_y, c, COLORS[color]);  // second copy
            
        }
    }

    // Render land position preview
    if (block_active) {
        for (int bx = 0; bx < BLOCK_ARRAY_AXIS_SIZE; bx++) {
            for (int by = 0; by < BLOCK_ARRAY_AXIS_SIZE; by++) {
                if (current_block.block->cells[bx][by]) {
                    int field_x = current_block.x + bx;
                    int field_y = block_land_y + by;
                    if (field_x >= 0 && field_x < FIELD_WIDTH && field_y >= 0 && field_y < FIELD_HEIGHT) {
                        int draw_x = FIELD_X + field_x * 2 + 1; // each x uses 2 chars; +1 for border
                        int draw_y = FIELD_Y + 1 + field_y;
                        draw_char(draw_x, draw_y, PREVIEW_CHAR, LIGHT_GRAY_ON_BLACK); // first copy
                        draw_char(draw_x + 1, draw_y, PREVIEW_CHAR, LIGHT_GRAY_ON_BLACK); // second copy
                    }
                }
            }
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
                        int draw_x = FIELD_X + field_x * 2 + 1; // each x uses 2 chars; +1 for border
                        int draw_y = FIELD_Y + 1 + field_y;
                        draw_char(draw_x, draw_y, FALLING_BLOCK_CHAR, COLORS[current_block.color]); // first copy
                        draw_char(draw_x + 1, draw_y, FALLING_BLOCK_CHAR, COLORS[current_block.color]); // second copy
                    }
                }
            }
        }
    }

    // Render score 
    int score_x = FIELD_X + (FIELD_WIDTH + 1) * 2 + 14; // to the right of the field
    int score_y = FIELD_Y + FIELD_HEIGHT / 2 - 8;
    render_text_panel(score_x, score_y, "Score:");
    iota(score, score_buffer);
    render_text_panel(score_x, score_y + 3, score_buffer);

    // Render "held" block
    if (block_held) {
        render_text_panel(HELD_BLOCK_POSTION, 10, "Held block:");
        set_cursor(HELD_BLOCK_POSTION, 13);

        render_box(HELD_BLOCK_POSTION + 2, 13, BLOCK_ARRAY_AXIS_SIZE * 2 + 2, BLOCK_ARRAY_AXIS_SIZE + 2, WHITE_ON_BLACK); // box around held block
        for (int bx = 0; bx < BLOCK_ARRAY_AXIS_SIZE; bx++) {
            for (int by = 0; by < BLOCK_ARRAY_AXIS_SIZE; by++) {
                int screen_x = (HELD_BLOCK_POSTION + 9) + bx;
                int screen_y = 13 + by;
                char block_char = EMPTY_CHAR;

                if (held_block->cells[bx][by]) {
                    block_char = FALLING_BLOCK_CHAR;
                }
                int draw_x = screen_x * 2 + 1; // each x uses 2 chars; +1 for border
                write_char(draw_x, screen_y, block_char); // first copy
                write_char(draw_x + 1, screen_y, block_char); // second copy
            }
        }
    }
    // Render next block
    if (next_block) {
        render_text_panel(NEXT_BLOCK_POSITION, 10, "Next block:");
        set_cursor(NEXT_BLOCK_POSITION, 13);

        render_box(NEXT_BLOCK_POSITION + 1, 13, BLOCK_ARRAY_AXIS_SIZE * 2 + 4, BLOCK_ARRAY_AXIS_SIZE + 2, WHITE_ON_BLACK); // box around next block
        for (int bx = 0; bx < BLOCK_ARRAY_AXIS_SIZE; bx++) {
            for (int by = 0; by < BLOCK_ARRAY_AXIS_SIZE; by++) {
                int screen_x = (NEXT_BLOCK_POSITION + 17) + bx;
                int screen_y = 13 + by;
                char block_char = EMPTY_CHAR;

                if (next_block->cells[bx][by]) {
                    block_char = FALLING_BLOCK_CHAR;
                }
                int draw_x = screen_x * 2 + 1; // each x uses 2 chars; +1 for border
                write_char(draw_x, screen_y, block_char); // first copy
                write_char(draw_x + 1, screen_y, block_char); // second copy
            }
        }
    }
}

void render_box(int x, int y, int width, int height, uint8_t color) {
    // Top and bottom borders
    for (int i = 0; i < width; i++) {
        write_char(x + i, y, HORIZONTAL_BORDER_CHAR);
        write_char(x + i, y + height - 1, HORIZONTAL_BORDER_CHAR);
    }
    // Left and right borders
    for (int j = 0; j < height; j++) {
        write_char(x, y + j, VERTICAL_BORDER_CHAR);
        write_char(x + width - 1, y + j, VERTICAL_BORDER_CHAR);
    }
    // Corners
    write_char(x, y, '+');
    write_char(x + width - 1, y, '+');
    write_char(x, y + height - 1, '+');
    write_char(x + width - 1, y + height - 1, '+');
}

void render_text_panel(int x, int y, const char* text) {
    render_box(x, y, 15, 3, WHITE_ON_BLACK);
    set_cursor(x + 2, y + 1);
    print_string(text);
}

void render_borders() {
    set_color(BLUE_ON_BLACK);

    // Render left and right borders
    int x1 = FIELD_X;
    int x2 = FIELD_X + (FIELD_WIDTH + 1) * 2 - 1;  // *2 since double chars
    for (int y = FIELD_Y + 1; y <= FIELD_Y + FIELD_HEIGHT; y++) {
        write_char(x1, y, VERTICAL_BORDER_CHAR);
        write_char(x2, y, VERTICAL_BORDER_CHAR);
    }
    // Render top and bottom borders   
    int y1 = FIELD_Y;
    int y2 = FIELD_Y + FIELD_HEIGHT + 1;
    for (int x = FIELD_X; x < FIELD_X + (FIELD_WIDTH + 1) * 2; x++) {
        write_char(x, y1, HORIZONTAL_BORDER_CHAR);
        write_char(x, y2, HORIZONTAL_BORDER_CHAR);
    }

    // Render corners
    write_char(x1, y1, '+');
    write_char(x2, y1, '+');
    write_char(x1, y2, '+');
    write_char(x2, y2, '+');
}

void render_background() {
    set_color(DARK_GRAY_ON_BLACK);
    for (int y = 0; y < VIDEO_HEIGHT; y++) {
        for (int x = 0; x < VIDEO_WIDTH; x++) {
            if (rand_range(0, 100) < 2) { // ~2% chance to render a dot for subtle starry background
                write_char(x, y, '.');
            }
        }
    }
}

void render_playfield_background() {
    render_background();
    render_borders();

    set_color(WHITE_ON_BLACK); // Reset color for next render
}

void render_main_menu_stars() {
    // Render some random stars in the background of the main menu
    set_color(DARK_GRAY_ON_BLACK);
    for (int i = 0; i < MAIN_MENU_STAR_COUNT; i++) {
        int x = rand_range(0, VIDEO_WIDTH - 1);
        int y = rand_range(0, VIDEO_HEIGHT - 1);

        write_char(x, y, '.');
    }
}

void render_screen_borders() {
    set_color(BLUE_ON_BLACK);

    // Top border
    set_cursor(0, 0);
    print_string("################################################################################");

    // Bottom border
    set_cursor(0, 24);
    print_string("################################################################################");

    // Side borders
    for (int y = 1; y < 24; y++) {
        set_cursor(0, y);
        print_string("#");

        set_cursor(79, y);
        print_string("#");
    }
}

void render_title() {
    // Title shadow
    set_color(DARK_GRAY_ON_BLACK);
    set_cursor(TITLE_POSITION_X, 4);
    print_string("######## ######## ######## ########   #######   ######");

    set_cursor(TITLE_POSITION_X, 5);
    print_string("   ##    ##          ##    ##     ## ##     ## ##    ##");

    set_cursor(TITLE_POSITION_X, 6);
    print_string("   ##    ##          ##    ##     ## ##     ## ##");

    set_cursor(TITLE_POSITION_X, 7);
    print_string("   ##    ######      ##    ########  ##     ##  ######");

    set_cursor(TITLE_POSITION_X, 8);
    print_string("   ##    ##          ##    ##   ##   ##     ##       ##");

    set_cursor(TITLE_POSITION_X, 9);
    print_string("   ##    ##          ##    ##    ##  ##     ## ##    ##");

    set_cursor(TITLE_POSITION_X, 10);
    print_string("   ##    ########    ##    ##     ##  #######   ######");

    // Main title
    set_color(CYAN_ON_BLACK);
    set_cursor(TITLE_POSITION_X - 1, 3);
    print_string("######## ######## ######## ########   #######   ######");

    set_cursor(TITLE_POSITION_X - 1, 4);
    print_string("   ##    ##          ##    ##     ## ##     ## ##    ##");

    set_cursor(TITLE_POSITION_X - 1, 5);
    print_string("   ##    ##          ##    ##     ## ##     ## ##");

    set_cursor(TITLE_POSITION_X - 1, 6);
    print_string("   ##    ######      ##    ########  ##     ##  ######");

    set_cursor(TITLE_POSITION_X - 1, 7);
    print_string("   ##    ##          ##    ##   ##   ##     ##       ##");

    set_cursor(TITLE_POSITION_X - 1, 8);
    print_string("   ##    ##          ##    ##    ##  ##     ## ##    ##");

    set_cursor(TITLE_POSITION_X - 1, 9);
    print_string("   ##    ########    ##    ##     ##  #######   ######");
}

void render_main_menu() {
    clear_screen(); // Reset screen

    render_main_menu_stars();   // Stars around the screen
    render_screen_borders();    // Borders around the screen

    render_title();    // Big Title in the middle of the screen

    // Menu options
    set_cursor(28, 15); // 40 - len("Press 1 to start") / 2
    set_color(YELLOW_ON_BLACK);
    print_string("+----------------------+");

    set_cursor(28, 16);
    print_string("|  [1] Start Game      |");
    set_cursor(28, 17);
    print_string("+----------------------+");

    set_color(WHITE_ON_BLACK); // Reset color for next render
}

void end_game() {
    game_over = true;
    main_menu = true;  // Display main menu again
    // Render field with all filled blocks
    for (int y = 0; y < FIELD_HEIGHT; y++) {    // rows
        for (int x = 0; x < FIELD_WIDTH; x++) {  // columns

            // Keep existing blocks
            if (!IS_CELL_FILLED(field_get(x, y))) {
                
                int draw_x = FIELD_X + x * 2 + 1;  // each x uses 2 chars; +1 for border
                int draw_y = FIELD_Y + 1 + y;
                draw_char(draw_x, draw_y, BLOCK_CHAR, BLACK_ON_WHITE); // first copy
                draw_char(draw_x + 1, draw_y, BLOCK_CHAR, BLACK_ON_WHITE);  // second copy
            }
        }
    }
}

void reset_field() {
    // Clear field
    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            field_set(x, y, CELL_EMPTY);
        }
    }
}