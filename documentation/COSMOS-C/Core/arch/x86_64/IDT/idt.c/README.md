# ⚙️ `idt.c` — Interrupt Descriptor Table Implementation

## 📄 Overview

This file defines the **Interrupt Descriptor Table (IDT)** initialization logic for the kernel’s **x86_64 architecture**.  
The IDT maps CPU exceptions and hardware interrupts to specific handler routines (ISRs and IRQs).

The implementation sets up the IDT structure, populates each vector, and loads it into the CPU using the `lidt` instruction.

---

## 🧠 Key Structures
---
### `struct idt_entry`

Defines a single **64-bit IDT entry**.  
Each entry describes the address and attributes of one interrupt handler.

```c
struct idt_entry {
    uint16_t offset_low;   // Lower 16 bits of handler address
    uint16_t selector;     // Code segment selector (usually kernel segment)
    uint8_t  ist;          // Interrupt Stack Table index
    uint8_t  type_attr;    // Type and attributes (e.g., 0x8E = present, interrupt gate)
    uint16_t offset_mid;   // Middle 16 bits of handler address
    uint32_t offset_high;  // Upper 32 bits of handler address
    uint32_t zero;         // Reserved (must be 0)
} __attribute__((packed));
```
---
### `struct idt_ptr`

**The IDT pointer** structure passed to **the CPU** with the `lidt` instruction.

```c
struct idt_ptr {
    uint16_t limit;  // Size of IDT - 1
    uint64_t base;   // Base address of the IDT
} __attribute__((packed));
```

---
## 🧩 Constants and Globals

```c
#define KERNEL_CS 0x08
```

- `KERNEL_CS` → Segment selector for the kernel code segment (defined in GDT). Two static global variables are used:

```c
static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;
```

---
## 🛠️ Function: `set_idt_gate`

Configures a single IDT entry with a handler address and flags.

```c
void set_idt_gate(int n, uint64_t handler, uint8_t flags) {
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = KERNEL_CS;
    idt[n].ist         = 0;
    idt[n].type_attr   = flags;
    idt[n].offset_mid  = (handler >> 16) & 0xFFFF;
    idt[n].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].zero        = 0;
}
```

- `n` → Interrupt vector (0–255)
- `handler` → Address of ISR/IRQ handler
- `flags` → Attributes (e.g., `0x8E` = present, interrupt gate, ring 0)

---
## ⚡ ISR and IRQ Declarations
The file declares **external handler stubs** implemented in **assembly** (isr.asm):

- **ISRs (0–31)** — CPU exceptions
- **IRQs (32–47)** — Hardware interrupts (e.g., timer, keyboard)

```c
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
```

These stubs will later call a common interrupt handler in C.

---
## 🚀 Function: `idt_init()`

Initializes and loads the IDT into the CPU.

```c
void idt_init(void) {
    // 1. Zero out the IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        set_idt_gate(i, 0, 0);
    }

    // 2. Define the standard interrupt gate attributes
    const uint8_t int_gate = 0x8E; // Present, ring 0, interrupt gate

    // 3. Register CPU exception handlers (vectors 0–31)
    set_idt_gate(0,  (uint64_t)isr0,  int_gate);
    set_idt_gate(1,  (uint64_t)isr1,  int_gate);
    /* ... */
    set_idt_gate(31, (uint64_t)isr31, int_gate);

    // 4. Register hardware IRQ handlers (vectors 32–47)
    set_idt_gate(32, (uint64_t)irq32, int_gate);
    /* ... */
    set_idt_gate(47, (uint64_t)irq47, int_gate);

    // 5. Load the IDT into the CPU
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint64_t)&idt;
    __asm__ volatile ("lidt %0" : : "m"(idtp));
}
```
## 🧩 Summary Table

| Step | Description                                         |
| ---- | --------------------------------------------------- |
| 1️⃣  | Zero out all IDT entries                            |
| 2️⃣  | Set up ISR handlers for CPU exceptions (0–31)       |
| 3️⃣  | Set up IRQ handlers for hardware interrupts (32–47) |
| 4️⃣  | Fill in the IDT pointer structure                   |
| 5️⃣  | Load IDT with the `lidt` instruction                |
---

## 🧠 Notes

- `0x8E` flag means:
    - Bit 7: Present
    - Bits 6–5: Descriptor privilege level (0 = kernel)
    - Bit 4: 0 (always)
    - Bits 3–0: Type (1110 = interrupt gate)
- The IDT must be reloaded after switching to long mode.
- In isr.asm, each ISR should save registers, call a C-level handler, and restore state.
---

## ✅ Result

After `idt_init()`:

- The CPU knows where to jump when an interrupt or exception occurs.
- System exceptions (like divide-by-zero or page fault) are properly handled.
- Hardware interrupts (like keyboard, timer, etc.) are routed safely to their handlers.

**💡 The IDT is the CPU’s “phonebook” for interrupts —** each entry connects a unique interrupt number to the function that handles it.

---