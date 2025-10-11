#include "pit.h"
#include "arch/x86_64/IRQ/port.h"
#include "HAL/console/print.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_FREQUENCY 1193182 // Hz

// inicjalizacja PIT na określoną częstotliwość
void pit_init(uint32_t frequency) {
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);
    outb(PIT_COMMAND, 0x36);            // channel 0, lo/hi byte, mode 3 (square wave)
    outb(PIT_CHANNEL0, divisor & 0xFF); // low byte
    outb(PIT_CHANNEL0, divisor >> 8);   // high byte

    print_str("[PIT] Initialized at ");
    print_int(frequency);
    print_str(" Hz\n");
}