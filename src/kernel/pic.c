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
void PIC_mask(int irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) { // master PIC
        port = PIC1_DATA;
    } else { // slave PIC
        port PIC2_DATA;
        irq -= 8; // PIC2 has its own 8 interrupts
    }
    
    value = inb(port) | (1 << irq);
    outb(port, value);
}
void PIC_unmask(int irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) { // master PIC
        port = PIC1_DATA;
    } else { // slave PIC
        port PIC2_DATA;
        irq -= 8; // PIC2 has its own 8 interrupts
    }
    
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}