#include "irq.h"

IRQ_Handler irq_handlers[16]; // 2 PICs * 8 interrupts each

void __attribute__((cdecl)) irq_handler(Registers* regs) {
    int irq = regs->interrupt_number - PIC_REMAP_OFFSET;


    if (irq_handlers[irq] != 0) {
        irq_handlers[irq](regs); // Handle IRQ
    } else {
        print_string_literal("Unhandled Irq:  ");
        print_int(irq);
        print_string_literal("\n");
    }

    // Send EOI
    PIC_send_end_of_interrupt(irq);
}

void init_irq() {
    print_string_literal("Initializing PIC ISR handlers...\n");
    PIC_remap(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

    for (int i = 0; i < 16; i++) {
        isr_register_handler(PIC_REMAP_OFFSET + i, irq_handler);
    }
    
    asm volatile("sti"); // Enable interrupts
}

void irq_register_handler(int irq, IRQ_Handler handler) {
    irq_handlers[irq] = handler;
}

void irq_register_handler_and_unmask(int irq, IRQ_Handler handler) {
    irq_register_handler(irq, handler);
    PIC_unmask(irq);
}