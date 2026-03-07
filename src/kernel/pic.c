#include "pic.h"

void PIC_configure(uint8_t pic1_offset, uint8_t pic2_offset) {
    return;
}

void PIC_send_end_of_interrupt(int irq) {
    if(irq >= 8) {     // if slave PIC
        outb(PIC2_COMMAND, PIC_EOI); 
    }

    outb(PIC1_COMMAND, PIC_EOI); // master PIC has to recieve no matter what
}

void PIC_disable() {
    outb(PIC1_DATA, 0xff); // Mask all master PIC interrupts
    outb(PIC2_DATA, 0xff); // Mask all slave PIC interrupt
}
void PIC_mask(int irq);
void PIC_unmask(int irq);