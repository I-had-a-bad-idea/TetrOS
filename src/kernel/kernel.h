#pragma once
#include "stdint.h"
#include "isr.h"
#include "idt.h"
#include "irq.h"
#include "io.h"

extern uint32_t random_seed;

#define VIDEO_MEMORY 0xB8000

#define BLACK_ON_BLACK      0x00
#define WHITE_ON_BLACK      0x0F
#define BLACK_ON_WHITE      0xF0
#define RED_ON_BLACK        0x04
#define GREEN_ON_BLACK      0x02
#define BLUE_ON_BLACK       0x01
#define YELLOW_ON_BLACK     0x0E
#define CYAN_ON_BLACK       0x0B
#define MAGENTA_ON_BLACK    0x0D
#define LIGHT_GRAY_ON_BLACK 0x07
#define DARK_GRAY_ON_BLACK  0x08
#define BLACK_ON_RED        0x40
#define BLACK_ON_GREEN      0x20
#define BLACK_ON_BLUE       0x10
#define WHITE_ON_BLUE       0x1F
#define YELLOW_ON_BLUE      0x1E
#define WHITE_ON_WHITE      0xFF

#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25

#define TIMER_SECONDS_PER_TICK 1

typedef void (*func)(void);

typedef struct {
    func function;
    uint32_t interval;
} timer_event;

#define MAX_TIMER_EVENTS 16

extern timer_event timer_events[MAX_TIMER_EVENTS];
extern uint16_t timer_event_count;

extern uint32_t cursor_position;

#define print_string_literal(str) do { \
    char _tmp[] = str; \
    print_string(_tmp); \
} while(0)

void print_char(char c);
void print_string(const char* str);
void print_int(int n);

void clear_screen();
void reset_cursor();
void write_char(int x, int y, char c);

int get_timer_ticks();
float get_time();
char get_pressed_key();

uint32_t rand32();
uint32_t rand_range(uint32_t min, uint32_t max);

void timer_register(func function, uint32_t interval);