#pragma once
#include "stdint.h"
#include "isr.h"
#include "idt.h"
#include "irq.h"
#include "io.h"

// See https://wiki.osdev.org/VESA_Video_Modes INT 0x10, AX=0x4F01, CX=mode, ES:DI=256 byte buffer 
struct vbe_mode_info {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

#define VBE_INFO_ADDR 0x9400 // see also bootloader/boot_stage_2.asm

#define BLACK vbe_rgb(0, 0, 0)
#define RED vbe_rgb(50, 50, 50)

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


uint32_t vbe_rgb(uint8_t r, uint8_t g, uint8_t b);
void put_pixel(int x, int y, uint32_t color);
void fill_screen(uint32_t color);

int get_timer_ticks();
float get_time();
char get_pressed_key();

uint32_t rand32();
uint32_t rand_range(uint32_t min, uint32_t max);

void timer_register(func function, uint32_t interval);

void halt_cpu();