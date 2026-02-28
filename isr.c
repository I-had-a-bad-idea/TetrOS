#include "isr.h"

static struct registers* current_regs = NULL;

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
    current_regs = NULL;
}

void generic_isr_handler() {
    if (!current_regs) return;

    print_string("ISR called: ");
    print_int(current_regs->int_no);
    print_string("\n");

    // Special handling for specifigc interrupts
    switch (current_regs->int_no) {
        case 0:
            print_string("Division by zero error! Value of eax: ");
            print_int(current_regs->eax);
            print_string("\n");
            break;
        default:
            break;
    }
    print_string("Halting system.\n");
    asm volatile("hlt");
}