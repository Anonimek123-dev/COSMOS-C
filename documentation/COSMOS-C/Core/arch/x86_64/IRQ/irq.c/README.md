## ⚡ `irq.c` — Core Interrupt Request (IRQ) Handling Logic

### 🧩 Overview

The `irq.c` file provides the **core logic for handling interrupts** (both CPU exceptions and hardware IRQs).  
It acts as a high-level C interface for the **low-level interrupt stubs** written in assembly (`isr.S`), handling routing, acknowledgment, and delegation to specific device drivers.

---

### 📄 Source Code

```c
#include <stdint.h>
#include "HAL/console/print.h"
#include "arch/x86_64/PIC/pic.h"
#include "Drivers/PS2/keyboard/ps2.h"

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
```

### 🧠 Functionality Breakdown
#### 🪫 `isr_handler(uint64_t vector)`

This is the **central interrupt handler** called from the low-level assembly ISR stubs.
It receives the **interrupt vector number** (passed via the `rdi` register) and decides how to process it.

---
- **Exception Handling** (**vectors** `0–31`)

If the vector number is ≤ 31, the interrupt corresponds to a **CPU exception** (like divide-by-zero, page fault, etc.).
The handler prints out a message showing which exception occurred.
```c
if (vector <= 31) {
    print_str("Exception vector: ");
    print_int((int)vector);
    print_str("\n");
}
```
*(In the future, this could be extended to show detailed CPU state, stack info, or panic screen.)*

---
- **⚙️ Hardware IRQ Handling** (**vectors** `32–47`)

When the vector number is between 32 and 47, it corresponds to a **hardware interrupt (IRQ)**, typically routed through the `PIC`.

The IRQ number is obtained by subtracting 32:
```c
unsigned char irq = (unsigned char)(vector - 32);
```
Then a switch statement dispatches the interrupt to the correct device driver:
```c
switch (irq) {
    case 0: // PIT timer IRQ0
        timer_tick();
        break;
    case 1: // Keyboard IRQ
        keyboard_irq_handler();
        break;
    default:
        // Other IRQs not yet handled
        break;
}
```
Finally, the interrupt must be acknowledged by sending an **End Of Interrupt (EOI)** signal to the **PIC**:
```c
pic_send_eoi(irq);
```
This tells the PIC that the CPU is done handling the interrupt, allowing new interrupts to be processed.

---
- ⚠️ Unexpected Vectors

If a vector number falls outside the expected range (0–47), the handler logs a message:
```c
print_str("Unhandled vector: ");
print_int((int)vector);
print_str("\n");
```

---
#### 🧷 `enable_irq(void)`

A small utility wrapper to **enable hardware interrupts** globally by setting the **IF (Interrupt Flag)** in the CPU’s `EFLAGS` register.
```c
__asm__ volatile ("sti");
```
This instruction allows the CPU to begin accepting interrupts after initialization.

---
### 🔄 Typical Flow

1. A device (e.g., keyboard) triggers a hardware interrupt (e.g., IRQ1).
2. The PIC forwards the signal to the CPU.
3. The CPU jumps to the correct **IDT entry** (mapped to an ISR stub in assembly).
4. The ISR stub calls the C function `isr_handler(vector)`, passing the interrupt vector.
5. The handler:
    - Distinguishes between CPU exceptions and hardware IRQs.
    - Calls the correct driver (e.g., `keyboard_irq_handler()`).
    - Sends an EOI signal to the PIC via `pic_send_eoi(irq)`.

---
### ⚙️ Integration

| Subsystem           | Role                                                |
| ------------------- | --------------------------------------------------- |
| **IDT**             | Maps interrupt vectors (0–255) to handlers.         |
| **PIC**             | Routes hardware IRQs (0–15) to IDT vectors (32–47). |
| **PIT**             | Uses IRQ0 (system timer).                           |
| **Keyboard Driver** | Uses IRQ1 for input events.                         |
| **Console**         | Used for logging interrupt messages.                |
---

### ✅ Summary

| Function                 | Description                                                              |
| ------------------------ | ------------------------------------------------------------------------ |
| `isr_handler()`          | Central interrupt handler — dispatches CPU exceptions and hardware IRQs. |
| `enable_irq()`           | Enables hardware interrupts globally (`sti`).                            |
| `pic_send_eoi()`         | Acknowledges handled IRQs to the PIC.                                    |
| `keyboard_irq_handler()` | Device-specific IRQ handler for PS/2 keyboard.                           |
---

🧩 The `irq.c` module serves as the **bridge between the low-level interrupt hardware and the kernel’s device drivers**, providing a clean and extensible interrupt handling interface.

---