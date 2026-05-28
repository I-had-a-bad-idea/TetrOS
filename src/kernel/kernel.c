#include "kernel.h"
#include "tetris/tetris.h"

// List of keys ("key number" is index)
const char keyboard_scancodes[128] = {
0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
'\t',
'q','w','e','r','t','y','u','i','o','p','[',']','\n',
0,
'a','s','d','f','g','h','j','k','l',';','\'','`',
0,
'\\',
'z','x','c','v','b','n','m',',','.','/',
0,
'*',
0,
' ',
};

static char pressed_key = 0;

void keyboard_irq(Registers* regs) {
    uint8_t scancode = inb(0x60);  // Get "key number"
    if (scancode < 128) { // press
        char c = keyboard_scancodes[scancode]; // get key from list
        if (c) {
            pressed_key = c; // store key 
        }
    }
    else { // release
        char c = keyboard_scancodes[scancode - 128]; // get key from list
        if (c && pressed_key == c) { // if it is the current key
            pressed_key = 0;  // delete current key
        }
    }
}


uint16_t timer_event_count = 0;
timer_event timer_events[MAX_TIMER_EVENTS] = {0};
volatile uint32_t cursor_position = 0;

static uint32_t random_seed = 1234567890;

volatile uint8_t current_color = WHITE_ON_BLACK;

static uint16_t backbuffer[VIDEO_WIDTH * VIDEO_HEIGHT] = {0}; // for double buffering

volatile int timer_ticks = 0;
void timer_irq(Registers* regs) {
    timer_ticks++;
}


int get_timer_ticks() {
    return timer_ticks;
}

float get_time() {
    return timer_ticks * TIMER_SECONDS_PER_TICK;
}


char get_pressed_key() {
    return pressed_key;
}

void print_char(char c) {
    if (cursor_position >= VIDEO_WIDTH * VIDEO_HEIGHT) {return;}

    if (c == '\n') {
        // move to next line
        cursor_position += VIDEO_WIDTH - (cursor_position % VIDEO_WIDTH);
    } else {
        backbuffer[cursor_position++] = (current_color << 8) | c;
    }
}

void draw_string(int x, int y, const char* str, uint8_t color) {
    while (*str) {
        draw_char(x++, y, *str++, color);
    }
    
}

void print_string(const char* str) {
    while (*str) {
        print_char(*str++);
    }
}

void print_int(int n) {
    if (n == 0) {
        print_char('0');
        return;
    }

    // Handle negative numbers
    if (n < 0) {
        print_char('-');
        n = -n;
    }

    // Hanle actual number
    char buffer[20];
    int i = 0;

    while (n > 0 && i < 19) {
        buffer[i++] = '0' + (n % 10);
        n /= 10;
    }

    for (int j = i - 1; j >= 0; j--){
        print_char(buffer[j]);
    }
}

void clear_screen() {
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
        backbuffer[i] = BLACK_ON_BLACK << 8 | ' ';
    }
    cursor_position = 0;
}

void reset_cursor() {
    cursor_position = 0;
}

void set_cursor(int x, int y) {
    cursor_position = y * VIDEO_WIDTH + x;
}

void set_color(uint8_t color) {
    current_color = color;
}

void write_char(int x, int y, char c) {
    backbuffer[y * VIDEO_WIDTH + x] = (current_color << 8) | c;
}

void draw_char(int x, int y, char c, uint8_t color) {
    backbuffer[y * VIDEO_WIDTH + x] = (color << 8) | c;
}

void timer_register(func function, uint32_t interval) {
    if (timer_event_count >= MAX_TIMER_EVENTS) {
        return;
    }
    
    // Register the function
    timer_events[timer_event_count].function = function;
    timer_events[timer_event_count].interval = interval;
    timer_event_count++;
}

// Xorshift RNG
uint32_t rand32() {
    random_seed ^= random_seed << 13;
    random_seed ^= random_seed >> 17;
    random_seed ^= random_seed << 5;
    return random_seed;
}

uint32_t rand_range(uint32_t min, uint32_t max) {
    return min + (rand32() % (max - min + 1));
}

void iota(int n, char* buffer) {
    if (n == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    int i = 0;
    if (n < 0) {
        buffer[i++] = '-';
        n = -n;
    }

    char temp[20];
    int j = 0;

    while (n > 0 && j < 19) {
        temp[j++] = '0' + (n % 10);
        n /= 10;
    }

    for (int k = j - 1; k >= 0; k--) {
        buffer[i++] = temp[k];
    }
    buffer[i] = '\0';
}

void switch_buffers() {
    volatile unsigned short* video_memory = (unsigned short*)VIDEO_MEMORY;
    // Overwrite screen
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
        video_memory[i] = backbuffer[i];
    }
}

volatile void main(){
    random_seed = 1234567890; // Reset random seed on each boot for consistent behavior
    print_string("Kernel started!\nSetting up the IDT...\n");
    init_idt();
    print_string("IDT-setup successful!\nSetting up the ISR...\n");
    init_isr();
    print_string("ISR-setup successful!\nSetting up the PICs...\n");
    init_irq();
    print_string("PICs setup successfull!\n");
    switch_buffers();

    // Register timer handler and enable interrupt 0
    irq_register_handler_and_unmask(0, timer_irq);
    // Register keyboard handler and enable interrupt 1
    irq_register_handler_and_unmask(1, keyboard_irq);

    print_string("Starting tetris");
    init_tetris();

    switch_buffers();
    clear_screen();
    int last_timer_ticks = 0;
    while (1) {
        if (cursor_position / VIDEO_WIDTH >= VIDEO_HEIGHT) {
            reset_cursor();
        }
        if (timer_ticks == last_timer_ticks) {
            continue;
        }
        last_timer_ticks = timer_ticks;
        for (int i = 0; i < timer_event_count; i++) {
            if (timer_ticks % timer_events[i].interval == 0) {
                timer_events[i].function();
            }
        }
        switch_buffers(); // Update screen with backbuffer contents
    }

    return;
}