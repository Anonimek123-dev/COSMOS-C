# 🧩 `idt.h` — Interrupt Descriptor Table (Header)

## 📄 Overview

This header file defines the **interface** for the Interrupt Descriptor Table (IDT) module used by the x86_64 kernel.  
It provides essential constants and function prototypes that allow other parts of the kernel to configure and initialize the IDT.

---

## 🧠 Purpose

The **IDT (Interrupt Descriptor Table)** maps interrupt and exception vectors to their respective handler functions.  
Every interrupt (e.g., divide-by-zero, keyboard input, system timer) has a unique vector number, and the IDT defines where control should jump when that interrupt occurs.

---

## 📦 Header Contents

```c
#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_ENTRIES 256

void idt_init(void);
void set_idt_gate(int n, uint64_t handler, uint8_t flags);

#endif
```
---
## ⚙️ Macro Definitions

```c
#define IDT_ENTRIES 256
```
Defines the total number of IDT entries — **256** possible interrupt vectors (0–255). This includes:

- **0–31** → CPU exceptions
- **32–47** → Hardware IRQs
- **48–255** → User-defined or system interrupts (e.g., syscalls)

---
## 🧩 Function Declarations
```c
void idt_init(void);
```
Initializes the **Interrupt Descriptor Table** and loads it into the CPU using the `lidt` instruction.
This function is typically called early during kernel initialization (after entering long mode).

**Responsibilities:**

- Clears all IDT entries
- Registers exception handlers (ISRs 0–31)
- Registers hardware interrupt handlers (IRQs 32–47)
- Loads the IDT pointer into the CPU

---
```c
void set_idt_gate(int n, uint64_t handler, uint8_t flags);
```
Configures a single IDT entry.

| Name      | Type       | Description                                                            |
| --------- | ---------- | ---------------------------------------------------------------------- |
| `n`       | `int`      | Interrupt vector index (0–255)                                         |
| `handler` | `uint64_t` | Address of the handler function (ISR or IRQ stub)                      |
| `flags`   | `uint8_t`  | Descriptor attributes (e.g., `0x8E` = present, ring 0, interrupt gate) |

**Usage Example:**
```c
set_idt_gate(32, (uint64_t)irq0_handler, 0x8E);
```

---
## 🧠 Usage Notes
- Include this header in any kernel source file that interacts with the interrupt subsystem:
```c
#include "arch/x86_64/IDT/idt.h"
```
- The actual implementation of these functions is located in `idt.c`.
- Must be called **after** the Global Descriptor Table (GDT) and long mode are set up.

## ✅ Summary

| Component            | Description                                      |
| -------------------- | ------------------------------------------------ |
| **`IDT_ENTRIES`**    | Number of interrupt vectors (256 total)          |
| **`set_idt_gate()`** | Assigns a handler to a specific interrupt vector |
| **`idt_init()`**     | Builds and loads the entire IDT                  |

>**💡 The IDT is a critical kernel structure that ensures proper handling of both CPU and hardware interrupts.**
---