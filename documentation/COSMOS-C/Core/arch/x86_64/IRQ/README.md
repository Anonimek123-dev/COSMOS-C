# ⚡ `IRQ/` — Interrupt Request Handling Module

## 📁 Folder Overview

The **`IRQ/`** directory contains all source files responsible for **handling hardware interrupts (IRQs)** in the operating system kernel.  
It works together with the **IDT (Interrupt Descriptor Table)** and **PIC (Programmable Interrupt Controller)** subsystems to route and manage interrupt signals from external hardware devices such as the keyboard, timer, and disk controllers.

---

## 🧠 Purpose

In x86 architecture, **hardware devices** signal the CPU using **IRQ lines (Interrupt Requests)**.  
Each IRQ corresponds to an interrupt vector in the **IDT**, starting at vector **32** (after remapping the PIC).

For example:

| Device | IRQ Line | IDT Vector |
|---------|-----------|------------|
| System Timer (PIT) | 0 | 32 |
| Keyboard | 1 | 33 |
| Cascade (Slave PIC) | 2 | 34 |
| COM2 / COM4 | 3 | 35 |
| COM1 / COM3 | 4 | 36 |
| Floppy Disk | 6 | 38 |
| Real-Time Clock | 8 | 40 |

---

## ⚙️ Key Responsibilities

The IRQ subsystem manages:
1. **Remapping the PIC** — to move IRQs away from CPU exceptions (0–31 → reserved by CPU).
2. **Setting up IDT entries (vectors 32–47)** — linking them to low-level interrupt service routines (ISRs).
3. **Dispatching interrupts** — calling the correct device driver or handler function.
4. **Acknowledging interrupts** — sending End Of Interrupt (EOI) signals to the PIC after handling.
5. **Enabling/disabling IRQs** dynamically — depending on device state or kernel mode.

---
## 🔄 Flow of a Hardware Interrupt
Device → PIC → CPU → IDT Entry → ISR Stub (isr.asm)

1. Hardware device triggers an IRQ.
2. PIC sends an interrupt signal to the CPU.
3. CPU jumps to the corresponding **IDT entry (32–47)**.
4. ISR stub (from `isr.asm`) calls the C-level special function.
5. The IRQ subsystem executes the appropriate handler function.
6. The interrupt is acknowledged with **EOI** to the PIC.

---
## 🧩 Integration with Other Subsystems
| Subsystem       | Role                                           |
| --------------- | ---------------------------------------------- |
| **IDT**         | Maps IRQ vectors to ISR stubs.                 |
| **PIC**         | Handles low-level routing and acknowledgement. |
| **PS/2 Driver** | Uses IRQ1 (keyboard).                          |
| **PIT Driver**  | Uses IRQ0 (system timer).                      |
---
## ✅ Summary
| Component   | Purpose                                                  |
| ----------- | -------------------------------------------------------- |
| `irq.c`     | Core IRQ handling logic (initialization, dispatch, EOI). |
| `irq.h`     | Declarations and constants for IRQ system.               |
| IRQ vectors | 32–47 (mapped from PIC hardware lines).                  |
| Integration | Works with IDT + PIC to manage hardware interrupts.      |

⚙️ The `IRQ` module forms the bridge between **hardware devices** and the **kernel**, allowing real-time responses to external events like keyboard input, timers, and system calls.

---