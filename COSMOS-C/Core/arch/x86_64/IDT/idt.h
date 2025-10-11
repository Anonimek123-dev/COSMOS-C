#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_ENTRIES 256

void idt_init(void);
void set_idt_gate(int n, uint64_t handler, uint8_t flags);

#endif