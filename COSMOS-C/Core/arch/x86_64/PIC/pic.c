#include "arch/x86_64/PIC/pic.h"
#include "arch/x86_64/IRQ/port.h"

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1

void pic_remap(int offset1, int offset2) {
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_CMD, 0x11);
    outb(PIC2_CMD, 0x11);
    outb(PIC1_DATA, offset1);
    outb(PIC2_DATA, offset2);
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Restore the old mask, but unmask IRQ0 (timer) and IRQ1 (keyboard).
    a1 &= ~(1 << 0); // unmask IRQ0
    a1 &= ~(1 << 1); // unmask IRQ1
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}


void pic_send_eoi(unsigned char irq) {
    if (irq >= 8) outb(PIC2_CMD, 0x20);
    outb(PIC1_CMD, 0x20);
}