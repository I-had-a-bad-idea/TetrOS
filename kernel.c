// #include "idt.h"
#include "kernel.h"

void print_char(char c) {

    unsigned short* video_memory = (unsigned short*)0xB8000;

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

void main(){
    // init_idt();
    
    print_string("Hello World,\nthis is the kernel!");
    return;
}