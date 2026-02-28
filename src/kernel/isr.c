#include "isr.h"

static struct registers* current_regs = 0;

#define ISR(n) \
void isr_##n() { \
    asm volatile("\tcli"); \
    asm volatile("\tpush $" #n); \
    asm volatile("\tpush $" #n); \
    asm volatile("\tjmp common_isr_stub_handler"); \
}

ISR(0);
ISR(1);
ISR(2);
ISR(3);
ISR(4);
ISR(5);
ISR(6);
ISR(7);
ISR(8);
ISR(9);
ISR(10);
ISR(11);
ISR(12);
ISR(13);
ISR(14);
ISR(15);

void common_isr_stub_handler()
{
  // save all registers
  asm volatile("\tpusha");
  // save ds segment
  asm volatile("\tmov %ds, %eax");
  asm volatile("\tpush %eax");
  // load new data segment for handling interrupt
  asm volatile("\tmov $0x10, %eax");
  asm volatile("\tmov %eax, %ds");
  asm volatile("\tmov %eax, %es");
  asm volatile("\tmov %eax, %fs");
  asm volatile("\tmov %eax, %gs");
  // call common interrupt handler
  asm volatile("push %esp");
  asm volatile("\tcall isr_handler");
  asm volatile("add $4, %esp");
  // get original ds segment
  asm volatile("\tpop %eax");
  // set its value to other segments
  asm volatile("\tmov %eax, %ds");
  asm volatile("\tmov %eax, %es");
  asm volatile("\tmov %eax, %fs");
  asm volatile("\tmov %eax, %gs");
  // restore all registers
  asm volatile("\tpopa");
  // restore stack space pushed by isr_n() routines
  // by pushing its isr number into stack
  asm volatile("\tadd $8, %esp");
  // store interrupt which was cleared by isr_n()
  // asm volatile("\tsti");
  // return after handling interrupt
  asm volatile("\tiret");
}

void isr_handler(struct registers* regs) {
    current_regs = regs;
    generic_isr_handler();
    current_regs = 0;
}

void generic_isr_handler() {
    if (!current_regs) return;

    print_string_literal("ISR called: ");
    print_int(current_regs->int_no);
    print_string_literal("\n");

    // Special handling for specifigc interrupts
    switch (current_regs->int_no) {
        case 0:
            print_string_literal("Division by zero error! Value of eax: ");
            print_int(current_regs->eax);
            print_string_literal("\n");
            break;
        default:
            break;
    }
    print_string_literal("Halting system.\n");
    asm volatile("hlt");
}