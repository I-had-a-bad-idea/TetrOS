#include "kernel.h"
#include "tetris/tetris.h"

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

struct vbe_mode_info* vbe_info = (struct vbe_mode_info*)VBE_INFO_ADDR;

uint16_t timer_event_count = 0;
timer_event timer_events[MAX_TIMER_EVENTS] = {0};
static char pressed_key = 0;
uint32_t cursor_position = 0;

static uint32_t random_seed = 1234567890;

int timer_ticks = 0;
void timer_irq(Registers* regs) {
    timer_ticks++;
}

void keyboard_irq(Registers* regs) {
    uint8_t scancode = inb(0x60);
    if (scancode < 128) { // press
        char c = keyboard_scancodes[scancode];
        if (c) {
            pressed_key = c;
        }
    }
    else { // release
        char c = keyboard_scancodes[scancode - 128];
        if (c && pressed_key == c) {
            pressed_key = 0;
        }
    }
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



void put_pixel(int x, int y, uint32_t color) {
    if (vbe_info->framebuffer == 0) return;
    
    uint8_t* fb = (uint8_t*)vbe_info->framebuffer;
    int pitch = vbe_info->pitch;
    int bpp = vbe_info->bpp / 8; // bytes-per-pixel 
    
    uint8_t* pixel = fb + (y * pitch) + (x * bpp);
    
    if (bpp == 2) {
        *(uint16_t*)pixel = (uint16_t)color;
    } else if (bpp == 3) {
        pixel[0] = color & 0xFF;
        pixel[1] = (color >> 8) & 0xFF;
        pixel[2] = (color >> 16) & 0xFF;
    } else if (bpp == 4) {
        *(uint32_t*)pixel = color;
    }
}

// Convert RGB values to framebuffer color format
uint32_t vbe_rgb(uint8_t r, uint8_t g, uint8_t b) {
    if (vbe_info->bpp == 16) {
        // 16-bit RGB: 5-6-5 format
        return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
    } else {
        // 24/32-bit RGB: standard format
        return (r << 16) | (g << 8) | b;
    }
}

void fill_screen(uint32_t color) {
    for (int y = 0; y < vbe_info->height; y++) {
        for (int x = 0; x < vbe_info->width; x++) {
            put_pixel(x, y, color);
        }
    }
}

void print_char(char c) {
    return;
}

void print_string(const char* str) {
    while (*str) {
        print_char(*str++);
    }
}

void print_int(int n) {
    return;
    // if (n == 0) {
    //     print_char('0');
    //     return;
    // }

    // // Handle negative numbers
    // if (n < 0) {
    //     print_char('-');
    //     n = -n;
    // }

    // // Hanle actual number
    // char buffer[12];
    // int i = 0;

    // while (n > 0) {
    //     buffer[i++] = '0' + (n % 10);
    //     n /= 10;
    // }

    // for (int j = i - 1; j >= 0; j--){
    //     print_char(buffer[j]);
    // }
}

void clear_screen() {
    fill_screen(RED);
}

void reset_cursor() {
    cursor_position = 0;
}

void set_cursor(int x, int y) {
    return;
}

void write_char(int x, int y, char c) {
    return;
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


// void main() {
//     uint32_t* fb = (uint32_t*)0xE0000000;

//     while (1) {
//         for (int i = 0; i < 1024 * 768; i++) {
//             fb[i] = 0x00FF0000;
//         }
//     }
// }

void main(){
    random_seed = 1234567890; // Reset random seed on each boot for consistent behavior
    init_graphics();
    clear_screen();
    // for (int x = 0; x < fb->width / 2; x++) {
    //     for (int y = 0; y < fb->height / 2; y++) {
    //         put_pixel(x, y, RED); // fill bottom right corner red
    //     }
    // }

    while (1) {
        fill_screen(RED);
    }
    // print_string("Kernel started!\nSetting up the IDT...\n");
    // init_idt();
    // print_string("IDT-setup successful!\nSetting up the ISR...\n");
    // init_isr();
    // print_string("ISR-setup successful!\nSetting up the PICs...\n");
    // init_irq();
    // print_string("PICs setup successfull!\n");

    // // Register timer handler and enable interrupt 0
    // irq_register_handler_and_unmask(0, timer_irq);
    // // Register keyboard handler and enable interrupt 1
    // irq_register_handler_and_unmask(1, keyboard_irq);

    // print_string("Starting tetris");
    // init_tetris();

    // clear_screen();

    // int last_timer_ticks = 0;
    // while (1) {
    //     // if (cursor_position / VIDEO_WIDTH >= VIDEO_HEIGHT) {
    //         // reset_cursor();
    //     // }
    //     if (timer_ticks == last_timer_ticks) {
    //         continue;
    //     }
    //     last_timer_ticks = timer_ticks;
    //     for (int i = 0; i < timer_event_count; i++) {
    //         if (timer_ticks % timer_events[i].interval == 0) {
    //             timer_events[i].function();
    //         }
    //     }
    // }

    return;
}

void halt_cpu() {
    asm volatile("jmp .");
}