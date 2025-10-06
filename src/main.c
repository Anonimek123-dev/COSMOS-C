#include "arch/x86_64/IDT/idt.h"
#include "arch/x86_64/PIC/pic.h"
#include "Drivers/PS2/keyboard/ps2.h"
#include "HAL/console/print.h"
#include <stdint.h>

void kernel_init(void);
void hardwaresetup(void);
void kernel_update(void);
void enable_irq(void);
void kb_update(void);

// Main kernel function
void kernel_main() {
    kernel_init();   // Init
    hardwaresetup(); // set IDT, remap PIC, init keyboard, enable interrupts    
    while (1) {
        kernel_update();
    }
}

// Init
void kernel_init() {    
    print_clear();
    print_set_color(LIGHT_GREEN, BLACK);    
    print_str("COSMOS-C booted successfully!\n");
}

// Called once to set up interrupts + devices
void hardwaresetup(void) {    
    idt_init();            // 1) initialize IDT (sets up interrupt gates)    
    pic_remap(0x20, 0x28); // 2) remap PIC so IRQs 0..15 map to vectors 0x20..0x2F   
    keyboard_init();       // 3) initialize keyboard driver (buffers, state)    
    enable_irq();          // 4) enable interrupts globally
}

void kernel_update(void) {
    kb_update();
}
