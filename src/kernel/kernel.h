#pragma once
#include "stdint.h"
#include "isr.h"
#include "idt.h"
#include "irq.h"
#include "io.h"

#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x0F
#define VIDEO_WIDTH 80

#define TIMER_SECONDS_PER_TICK 1

extern uint32_t cursor_position;

#define print_string_literal(str) do { \
    char _tmp[] = str; \
    print_string(_tmp); \
} while(0)

void print_char(char c);
void print_string(const char* str);
void print_int(int n);
int get_timer_ticks();
float get_time();