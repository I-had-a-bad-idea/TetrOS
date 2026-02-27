#include "idt.h"
#include "isr.h"

extern void load_idt(struct  IDT*);

void set_idt_gate(int n, uint32_t isr){
    idt_entries[n].base_low = isr & 0xffff;
    idt_entries[n].segment_selector = 0x08;
    idt_entries[n].zero = 0;
    idt_entries[n].type = 0x8E;
    idt_entries[n].base_high = (isr >> 16) & 0xffff;
}


void init_idt() {
    set_idt_gate(0, (uint32_t)isr_0);
    set_idt_gate(1, (uint32_t)isr_1);
    set_idt_gate(2, (uint32_t)isr_2);
    set_idt_gate(3, (uint32_t)isr_3);
    set_idt_gate(4, (uint32_t)isr_4);
    set_idt_gate(5, (uint32_t)isr_5);
    set_idt_gate(6, (uint32_t)isr_6);
    set_idt_gate(7, (uint32_t)isr_7);
    set_idt_gate(8, (uint32_t)isr_8);
    set_idt_gate(9, (uint32_t)isr_9);
    set_idt_gate(10, (uint32_t)isr_10);
    set_idt_gate(11, (uint32_t)isr_11);
    set_idt_gate(12, (uint32_t)isr_12);
    set_idt_gate(13, (uint32_t)isr_13);
    set_idt_gate(14, (uint32_t)isr_14);
    set_idt_gate(15, (uint32_t)isr_15);

    idt_ptr.limit_size = sizeof(idt_entries) - 1;
    idt_ptr.base_address = (uint32_t)&idt_entries;

    load_idt((uint32_t)&idt_ptr);
}