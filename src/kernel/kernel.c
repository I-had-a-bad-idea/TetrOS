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
    print_string_literal("Int print");
    // TODO: Implement actual printing of ints
}

void main(){
    print_string_literal("Kernel started!\nSetting up the IDT...\n");
    init_idt();
    print_string_literal("IDT-setup successful!\nSetting up the ISR...\n");
    init_isr();
    print_string_literal("ISR-setup successful!\n");
    
    
    // print_string_literal("Triggering invalid opcode..\n");    
    // int x = 1 / 0;
    return;
}