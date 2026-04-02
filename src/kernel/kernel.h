#pragma once
#include "stdint.h"
#include "isr.h"
#include "idt.h"
#include "irq.h"
#include "io.h"


typedef struct __attribute__((packed)){
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint32_t address;
    uint8_t bpp;
} framebuffer_t;

#define FRAMEBUFFER_INFO_LOCATION 0x9000 // see also bootloader/boot.asm


extern framebuffer_t* fb;
extern uint32_t* framebuffer;


#define BLACK 0x000000
#define RED 0xFF0000

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

void print_char(char c);
void print_string(const char* str);
void print_int(int n);

void clear_screen();
void reset_cursor();
void set_cursor(int x, int y);
void write_char(int x, int y, char c);

int get_timer_ticks();
float get_time();
char get_pressed_key();

uint32_t rand32();
uint32_t rand_range(uint32_t min, uint32_t max);

void timer_register(func function, uint32_t interval);