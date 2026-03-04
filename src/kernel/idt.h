#pragma once

#include "stdint.h"

// See https://wiki.osdev.org/Interrupt_Descriptor_Table

#define IDT_ENTRY_COUNT 256

struct IDT {
  uint16_t base_low;  // lower 16 bits 0-15 of the address to jump to when this interrupt fires
  uint16_t segment_selector;  // code segment selector in GDT
  uint8_t zero;  // unused, always be zero
  uint8_t type;  // types trap, interrupt gates
  uint16_t base_high;  // upper 16 bits 16-31 of the address to jump to
} __attribute__((packed));


struct IDT_PTR {
   uint16_t limit_size;  // limit size of all IDT segments
   uint32_t base_address;  // base address of the first IDT segment
} __attribute__((packed));


extern void init_idt();
void set_idt_gate(int interrupt, void* base);
void idt_enable_gate(int interrupt);
void idt_disable_gate(int interrupt);