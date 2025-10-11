# ⚙️ File: `pic.c` — Basic 8259 PIC (Programmable Interrupt Controller) Driver

This file provides **low-level control** over the Intel **8259A PIC**, used to manage **hardware interrupts (IRQs)** on legacy x86 systems.  
It implements **remapping** of interrupt vectors and **EOI (End Of Interrupt)** signaling.

---

## 📄 Source Code

```c
#include "arch/x86_64/PIC/pic.h" 
#include "arch/x86_64/IRQ/port.h"

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1

/* ------------------------------------------------------------
   Remap the PIC Interrupt Vectors
   ------------------------------------------------------------ */
void pic_remap(int offset1, int offset2) {
    uint8_t a1 = inb(PIC1_DATA);  // Save current masks
    uint8_t a2 = inb(PIC2_DATA);

    /* Start initialization sequence for both PICs */
    outb(PIC1_CMD, 0x11);  // ICW1: start init, expect ICW4
    outb(PIC2_CMD, 0x11);

    /* Set vector offsets */
    outb(PIC1_DATA, offset1);  // ICW2: Master PIC vector offset
    outb(PIC2_DATA, offset2);  // ICW2: Slave PIC vector offset

    /* Setup cascade configuration */
    outb(PIC1_DATA, 4);  // ICW3: Tell Master that Slave is at IRQ2 (bitmask 0000 0100)
    outb(PIC2_DATA, 2);  // ICW3: Tell Slave its cascade ID (0000 0010)

    /* Set environment info */
    outb(PIC1_DATA, 0x01);  // ICW4: 8086/88 (MCS-80/85) mode
    outb(PIC2_DATA, 0x01);

    /* Restore previously saved masks */
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

/* ------------------------------------------------------------
   Send End of Interrupt (EOI) Signal
   ------------------------------------------------------------ */
void pic_send_eoi(unsigned char irq) {
    if (irq >= 8)
        outb(PIC2_CMD, 0x20);  // Acknowledge Slave PIC first
    outb(PIC1_CMD, 0x20);      // Always acknowledge Master PIC
}
```
---
## 🧩 Explanation
1. **🧠`pic_remap(int offset1, int offset2)`**
By default, the **8259 PIC** maps hardware IRQs to interrupt vectors **0x08–0x0F**, which **overlap with CPU exceptions**.
To avoid this conflict, the OS remaps them to **0x20–0x2F** (32–47).

**Steps performed:**
1. Save existing IRQ masks.
2. Send initialization commands (`0x11`) to both PICs.
3. Set new offsets for interrupt vectors.
4. Configure the **cascade connection** between the Master and Slave PICs.
5. Set 8086 mode.
6. Restore original masks.
---
2. **🔁`pic_send_eoi(unsigned char irq)`**

Once an IRQ is handled, the CPU must **send an End Of Interrupt (EOI)** command to inform the PIC that the interrupt has been serviced.

- If the interrupt came from the **Slave PIC** (`irq >= 8`), send EOI to both **Slave** and **Master**.
- Otherwise, send EOI only to **Master**.
---
## 🧱 Summary
| Function         | Purpose                                                                |
| ---------------- | ---------------------------------------------------------------------- |
| `pic_remap()`    | Changes interrupt vector mappings to avoid overlap with CPU exceptions |
| `pic_send_eoi()` | Sends acknowledgment to PIC after IRQ handling                         |
| **Ports Used**   | `0x20–0x21` (Master), `0xA0–0xA1` (Slave)                              |
---
✅ Usage Example (Initialization in Kernel):
```c
#include "arch/x86_64/PIC/pic.h"

void kernel_init() {
    pic_remap(0x20, 0x28);  // Move PIC interrupts to 0x20–0x2F
}
```
---