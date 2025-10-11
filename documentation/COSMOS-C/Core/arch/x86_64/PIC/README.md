# 📁 Folder: `PIC` — Programmable Interrupt Controller (Intel 8259)

The **`PIC` folder** contains the low-level implementation for handling **hardware interrupt routing** via the **Intel 8259 Programmable Interrupt Controller**, which is still used in x86-compatible systems (even when APIC is available for backward compatibility).

This module is essential for your **kernel’s interrupt subsystem** — it ensures that hardware-generated IRQs (like keyboard or timer interrupts) are correctly **mapped, delivered, and acknowledged**.

---

## 🧠 Purpose

The **8259 PIC** is responsible for:
- Handling **hardware interrupt requests (IRQs)**.
- **Prioritizing** multiple interrupt lines.
- **Sending interrupt vectors** to the CPU.
- **Acknowledging (EOI)** interrupts when the handler finishes.
- Allowing **masking/unmasking** of specific interrupts.

In protected mode kernels, the PICs are typically **remapped** because their default interrupt vectors overlap with **CPU exceptions (0x00–0x1F)**.

---
## ⚙️ Core Functions

### `void pic_init(void);`
Initializes the master and slave PICs by:
- Remapping their interrupt vectors (`0x20–0x2F`).
- Masking all IRQ lines to avoid spurious interrupts at boot.

---
#### `void pic_remap(int offset1, int offset2);`
Changes the interrupt vector offsets:
- **Master PIC** → `offset1` (usually `0x20`)
- **Slave PIC** → `offset2` (usually `0x28`)

This prevents overlap with CPU exceptions.

Example mapping after remap:

| IRQ | Device         | Vector |
|------|----------------|---------|
| 0    | Timer          | 0x20 |
| 1    | Keyboard       | 0x21 |
| 8–15 | Slave devices  | 0x28–0x2F |

---

### `void pic_send_eoi(unsigned char irq);`
Sends an **End Of Interrupt (EOI)** signal to the PIC(s) after handling an interrupt.  
Without sending EOI, the PIC will not deliver further interrupts of the same line.

- For IRQs ≥ 8, an EOI is sent to **both the Slave and Master**.
- For IRQs < 8, EOI is sent only to **the Master**.

---

## 🧩 How It Fits Into the Kernel

The PIC interacts closely with:
- **IDT (Interrupt Descriptor Table):** where the interrupt vectors are registered.
- **ISR/IRQ handlers:** which respond to interrupts.
- **Devices (e.g., PS/2 keyboard):** which generate IRQs.

Example flow for a keyboard interrupt:
[Keyboard press]
↓
PIC (IRQ1)
↓
CPU interrupt vector 0x21
↓
IDT entry → ISR handler
↓
keyboard_irq_handler()
↓
pic_send_eoi(1)

---
## 🔧 Implementation Summary

| File | Purpose |
|------|----------|
| `pic.h` | Defines constants and declares the PIC interface. |
| `pic.c` | Implements remapping, initialization, and EOI sending. |

---

## 🧰 Technical Notes

- The **PIC1 (Master)** handles IRQ0–7 through ports `0x20–0x21`.
- The **PIC2 (Slave)** handles IRQ8–15 through ports `0xA0–0xA1`.
- Both use **I/O ports** to configure and send commands.
- The **`outb()`** and **`inb()`** functions (from `port.h`) are used for communication.

---
## ✅ Summary
| Component          | Description                                    |
| ------------------ | ---------------------------------------------- |
| **PIC1 (Master)**  | Handles IRQ0–7                                 |
| **PIC2 (Slave)**   | Handles IRQ8–15                                |
| **pic_init()**     | Initializes and remaps both PICs               |
| **pic_remap()**    | Sets new interrupt vector offsets              |
| **pic_send_eoi()** | Sends acknowledgment after IRQ is handled      |
| **Dependencies**   | `port.h`, `isr.c`, `idt.c`, and device drivers |
---
**🧩 In essence:**
The `PIC` folder provides the **core interrupt routing system for your kernel**.
It remaps, initializes, and controls how hardware IRQs reach the CPU, ensuring your interrupt system works safely and predictably.

---