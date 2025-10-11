#include "arch/x86_64/IDT/idt.h"
#include <stdint.h>

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

#define KERNEL_CS 0x08

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

void set_idt_gate(int n, uint64_t handler, uint8_t flags) {
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = KERNEL_CS;
    idt[n].ist         = 0;
    idt[n].type_attr   = flags;
    idt[n].offset_mid  = (handler >> 16) & 0xFFFF;
    idt[n].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].zero        = 0;
}

/* Extern stubs defined in isr.asm (we will create them explicitly there) */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq32();
extern void irq33();
extern void irq34();
extern void irq35();
extern void irq36();
extern void irq37();
extern void irq38();
extern void irq39();
extern void irq40();
extern void irq41();
extern void irq42();
extern void irq43();
extern void irq44();
extern void irq45();
extern void irq46();
extern void irq47();

void idt_init(void) {
    /* zero out IDT */
    for (int i = 0; i < IDT_ENTRIES; i++) {
        set_idt_gate(i, 0, 0);
    }

    /* set CPU exception handlers (vectors 0..31) with type_attr 0x8E (present, DPL=0, interrupt gate) */
    const uint8_t int_gate = 0x8E;
    set_idt_gate(0,  (uint64_t)isr0,  int_gate);
    set_idt_gate(1,  (uint64_t)isr1,  int_gate);
    set_idt_gate(2,  (uint64_t)isr2,  int_gate);
    set_idt_gate(3,  (uint64_t)isr3,  int_gate);
    set_idt_gate(4,  (uint64_t)isr4,  int_gate);
    set_idt_gate(5,  (uint64_t)isr5,  int_gate);
    set_idt_gate(6,  (uint64_t)isr6,  int_gate);
    set_idt_gate(7,  (uint64_t)isr7,  int_gate);
    set_idt_gate(8,  (uint64_t)isr8,  int_gate);
    set_idt_gate(9,  (uint64_t)isr9,  int_gate);
    set_idt_gate(10, (uint64_t)isr10, int_gate);
    set_idt_gate(11, (uint64_t)isr11, int_gate);
    set_idt_gate(12, (uint64_t)isr12, int_gate);
    set_idt_gate(13, (uint64_t)isr13, int_gate);
    set_idt_gate(14, (uint64_t)isr14, int_gate);
    set_idt_gate(15, (uint64_t)isr15, int_gate);
    set_idt_gate(16, (uint64_t)isr16, int_gate);
    set_idt_gate(17, (uint64_t)isr17, int_gate);
    set_idt_gate(18, (uint64_t)isr18, int_gate);
    set_idt_gate(19, (uint64_t)isr19, int_gate);
    set_idt_gate(20, (uint64_t)isr20, int_gate);
    set_idt_gate(21, (uint64_t)isr21, int_gate);
    set_idt_gate(22, (uint64_t)isr22, int_gate);
    set_idt_gate(23, (uint64_t)isr23, int_gate);
    set_idt_gate(24, (uint64_t)isr24, int_gate);
    set_idt_gate(25, (uint64_t)isr25, int_gate);
    set_idt_gate(26, (uint64_t)isr26, int_gate);
    set_idt_gate(27, (uint64_t)isr27, int_gate);
    set_idt_gate(28, (uint64_t)isr28, int_gate);
    set_idt_gate(29, (uint64_t)isr29, int_gate);
    set_idt_gate(30, (uint64_t)isr30, int_gate);
    set_idt_gate(31, (uint64_t)isr31, int_gate);

    /* IRQs — map to 32..47 */
    set_idt_gate(32, (uint64_t)irq32, int_gate);
    set_idt_gate(33, (uint64_t)irq33, int_gate);
    set_idt_gate(34, (uint64_t)irq34, int_gate);
    set_idt_gate(35, (uint64_t)irq35, int_gate);
    set_idt_gate(36, (uint64_t)irq36, int_gate);
    set_idt_gate(37, (uint64_t)irq37, int_gate);
    set_idt_gate(38, (uint64_t)irq38, int_gate);
    set_idt_gate(39, (uint64_t)irq39, int_gate);
    set_idt_gate(40, (uint64_t)irq40, int_gate);
    set_idt_gate(41, (uint64_t)irq41, int_gate);
    set_idt_gate(42, (uint64_t)irq42, int_gate);
    set_idt_gate(43, (uint64_t)irq43, int_gate);
    set_idt_gate(44, (uint64_t)irq44, int_gate);
    set_idt_gate(45, (uint64_t)irq45, int_gate);
    set_idt_gate(46, (uint64_t)irq46, int_gate);
    set_idt_gate(47, (uint64_t)irq47, int_gate);

    /* load IDT */
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint64_t)&idt;
    __asm__ volatile ("lidt %0" : : "m"(idtp));
}