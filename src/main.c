#include "arch/x86_64/TIMER/timer.h"
#include "arch/x86_64/IDT/idt.h"
#include "arch/x86_64/PIC/pic.h"
#include "Drivers/PS2/keyboard/ps2.h"
#include "HAL/console/print.h"
#include "Core/arch/x86_64/TIMER/callback/callback.h"
#include <stdint.h>

void kernel_init(void);
void hardwaresetup(void);
void kernel_update(void);
void enable_irq(void);
void kb_update(void);
void load_logs(void);

// This function loads logs
void load_logs(void){
    /* IDT */        print_set_color(LIGHT_GREEN, BLACK); print_str("[1/6] [IDT] ");          print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(500);   print_str("\n");
    /* */            print_set_color(LIGHT_GREEN, BLACK); print_str("[2/6] [PIT] ");          print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(700);   print_str("\n");
    /* PIC */        print_set_color(LIGHT_GREEN, BLACK); print_str("[3/6] [PIC] ");          print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(900);   print_str("\n");
    /* KEYBOARD */   print_set_color(LIGHT_GREEN, BLACK); print_str("[4/6] [PS/2 DRIVER] ");  print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(1000);  print_str("\n");
    /* TIMER */      print_set_color(LIGHT_GREEN, BLACK); print_str("[5/6] [PIT] ");          print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(1500);  print_str("\n");
    /* INTERRUPTS */ print_set_color(LIGHT_GREEN, BLACK); print_str("[6/6] [INTERRUPTS] ");   print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(2000);  print_str("\n");
}
// Main kernel function
void kernel_main() {    
    hardwaresetup(); // set IDT, remap PIC, init keyboard, init timer, enable interrupts   
    kernel_init();   // Init 
    load_logs();
    while (1) {
        kernel_update();
    }
}

// Init
void kernel_init() {
    print_clear();
    print_set_color(WHITE, BLACK);
    print_str("COSMOS-C booted successfully!\n");
}

// Called once to set up interrupts + devices
void hardwaresetup(void) {    
    idt_init();            // 1) initialize IDT (sets up interrupt gates)    
    pic_remap(0x20, 0x28); // 2) remap PIC so IRQs 0..15 map to vectors 0x20..0x2F   
    keyboard_init();       // 3) initialize keyboard driver (buffers, state)
    timer_init();          // 4) initialize timer
    enable_irq();          // 5) enable interrupts globally   
}

void kernel_update(void) {
    kb_update();
}