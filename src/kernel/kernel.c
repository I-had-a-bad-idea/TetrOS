#include "kernel.h"

char keyboard_scancodes[128] = {
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


uint32_t cursor_position = 0;

int timer_ticks = 0;
void timer_irq(Registers* regs) {
    timer_ticks++;
}

void keyboard_irq(Registers* regs) {
    uint8_t scancode = inb(0x60);
    if (scancode < 128) { // press
        print_int(scancode);
        char c = keyboard_scancodes[scancode];
        if (c) {
            print_char(c);
        }
    }
}

int get_timer_ticks() {
    return timer_ticks;
}

float get_time() {
    return timer_ticks * TIMER_SECONDS_PER_TICK;
}

void print_char(char c) {

    unsigned short* video_memory = (unsigned short*)VIDEO_MEMORY;

    if (c == '\n') {
        // move to next line
        cursor_position += VIDEO_WIDTH - (cursor_position % VIDEO_WIDTH);
    } else {
        video_memory[cursor_position++] = (WHITE_ON_BLACK << 8) | c;
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
    char buffer[10];
    int i = 0;

    while (n > 0) {
        buffer[i++] = '0' + (n % 10);
        n /= 10;
    }

    for (int j = i - 1; j >= 0; j--){
        print_char(buffer[j]);
    }
}


void main(){
    print_string_literal("Kernel started!\nSetting up the IDT...\n");
    init_idt();
    print_string_literal("IDT-setup successful!\nSetting up the ISR...\n");
    init_isr();
    print_string_literal("ISR-setup successful!\nSetting up the PICs...\n");
    init_irq();
    print_string_literal("PICs setup successfull!\n");

    // Register timer handler and enable interrupt 0
    irq_register_handler_and_unmask(0, timer_irq);
    // Register keyboard handler and enable interrupt 1
    irq_register_handler_and_unmask(1, keyboard_irq);

    print_int(-12345);
    print_char(' ');
    print_int(54321);

    return;
}