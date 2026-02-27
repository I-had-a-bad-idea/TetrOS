#pragma once
#include "stdint.h"

#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x0F
#define VIDEO_WIDTH 80

uint32_t cursor_position = 0;

void print_char(char c);
void print_string(const char* str);
void print_int(int n);