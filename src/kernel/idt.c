#include "idt.h"

struct IDT idt_entries[IDT_ENTRY_COUNT];
struct IDT_PTR idt_ptr;

void load_idt(struct IDT_PTR* idt_ptr) {
    asm volatile("lidt (%0)" : : "r"(idt_ptr));
}

void set_idt_gate(int interrupt, void* base) {
    idt_entries[interrupt].base_low = ((uint32_t)base) & 0xffff;
    idt_entries[interrupt].segment_selector = 0x08;
    idt_entries[interrupt].zero = 0;
    idt_entries[interrupt].type = 0x8E;
    idt_entries[interrupt].base_high = ((uint32_t)base >> 16) & 0xffff;
}

void init_idt() {
    idt_ptr.limit_size = sizeof(idt_entries) - 1;
    idt_ptr.base_address = (uint32_t)&idt_entries;

    load_idt(&idt_ptr);
}