#pragma once

#include "stdint.h"
#include "kernel.h"

struct registers
{
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pushed by pusha
  uint32_t int_no, err_code;  // interrupt number and error code
  uint32_t eip, cs, eflags, useresp, ss;  // pushed by the processor automatically
};


#define ISR(n) \
void isr_##n() { \
    asm volatile("\tcli"); \
    asm volatile("\tpush $" #n); \
    asm volatile("\tpush $" #n); \
    asm volatile("\tjmp common_isr_stub_handler"); \
}

extern void common_isr_stub_handler();
extern void isr_handler(struct registers* regs);

extern void generic_isr_handler();
