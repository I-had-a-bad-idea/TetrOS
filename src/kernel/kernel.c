#include "kernel.h"

uint32_t cursor_position = 0;

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
    print_string_literal("ISR-setup successful!\n");

    print_int(-12345);
    print_char(' ');
    print_int(54321);

    return;
}