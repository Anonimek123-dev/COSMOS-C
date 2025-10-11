#include <stdint.h>
#include "HAL/console/print.h"
#include "arch/x86_64/PIC/pic.h"
#include "Drivers/PS2/keyboard/ps2.h"
#include "Core/arch/x86_64/TIMER/PIT/pit.h"
#include "Core/arch/x86_64/TIMER/timer.h"

// Forward declare PIC send EOI and keyboard handler
extern void pic_send_eoi(unsigned char irq);
extern void keyboard_irq_handler(void);

// C handler called from assembly with vector in rdi
void isr_handler(uint64_t vector) {
    if (vector <= 31) {
        // CPU exception
        print_str("Exception vector: ");
        print_int((int)vector);
        print_str("\n");
        // For now: just return (or halt) - in dev you might halt
        // __asm__ volatile ("cli; hlt");
        return;
    }

    if (vector >= 32 && vector <= 47) {
        unsigned char irq = (unsigned char)(vector - 32);
        // dispatch common IRQs here
        switch (irq) {
            case 0:  // PIT timer IRQ0
                timer_tick();
                break;
            case 1: // keyboard
                keyboard_irq_handler();
                break;
            default:
                // other IRQs we don't handle yet
                break;
        }

        // send EOI to PICs
        pic_send_eoi(irq);
        return;
    }

    // unexpected vector
    print_str("Unhandled vector: ");
    print_int((int)vector);
    print_str("\n");
}

// enable interrupts (wrapper)
void enable_irq(void) {
    __asm__ volatile ("sti");
}