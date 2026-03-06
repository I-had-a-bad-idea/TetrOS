#include "isr.h"

static struct registers* current_regs = 0;

ISR_Handler isr_handlers[IDT_ENTRY_COUNT];

static const char* const exceptions[] = {
    "Divide by zero error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception ",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};

void isr_initialize_gates(void);

void init_isr() {
    print_string_literal("Initializing ISRs and IDT gates...\n");
    isr_initialize_gates();
}

void __attribute__((cdecl)) isr_handler(Registers* regs) {
    if (isr_handlers[regs->interrupt_number] != 0) {
        isr_handlers[regs->interrupt_number](regs);
    }
    else {
        print_string_literal("Unhandled expection:  ");
        print_int(regs->interrupt_number);
        print_string_literal("\n");
        print_string(exceptions[regs->interrupt_number]);
        print_string_literal("\n");

        for (;;){
            asm volatile("hlt");
        }
    }
}

void isr_register_handler(int interrupt, ISR_Handler handler) {
    isr_handlers[interrupt] = handler;
    idt_enable_gate(interrupt);
}