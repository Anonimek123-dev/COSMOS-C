# 🧠 `isr.h` — Interrupt Service Routine Header

## 📄 Overview
The **`isr.h`** header provides the **C interface** for the interrupt service routines (ISRs) used by the kernel.  
It declares the central **`isr_handler()`** function — the bridge between low-level assembly stubs (in `isr.asm`) and higher-level C interrupt handling logic (in `irq.c`).

---

## 🧩 Source Code

```c
#ifndef ISR_H
#define ISR_H

#include <stdint.h>

/**
 * @brief Central interrupt handler called from assembly.
 *
 * This function is invoked by `isr_common_stub` (defined in `isr.asm`)
 * whenever a CPU exception or hardware interrupt occurs.
 *
 * @param vector The interrupt vector number (0–255) pushed by the ISR stub.
 *
 * Vectors 0–31 → CPU exceptions (e.g. divide-by-zero, page fault)
 * Vectors 32–47 → Hardware IRQs (e.g. keyboard, timer)
 * Above 47 → Reserved / future use
 */
void isr_handler(uint64_t vector);

#endif
```
---
## 🧠 Purpose

This header ensures that:

- **Assembly and C layers are linked properly**, by declaring the `isr_handler()` symbol.
- The kernel’s interrupt system has a **common entry point** for all ISRs and IRQs.
- The handler can distinguish between **CPU exceptions and hardware interrupts** based on the vector number.
---
## ⚙️ Flow Summary
| Layer              | Function                      | Description                                    |
| ------------------ | ----------------------------- | ---------------------------------------------- |
| 🧩 `isr.asm`       | `isr_common_stub`             | Saves registers, calls `isr_handler(vector)`   |
| 🧠 `isr_handler()` | (C function)                  | Determines interrupt type and dispatches logic |
| 🖥️ Device Drivers | e.g. `keyboard_irq_handler()` | Responds to specific IRQ events                |
---
## 🧾 Notes

- The handler follows **System V AMD64 ABI**: the vector number is passed in `rdi`.
- It must **not block or sleep** — it runs at interrupt level.
- Future enhancements might include a **register dump structure** or **error code handling**.
---
## ✅ Summary
| Symbol                         | Purpose                                                      |
| ------------------------------ | ------------------------------------------------------------ |
| `isr_handler(uint64_t vector)` | Main C entry point for all interrupts.                       |
| `<stdint.h>`                   | Provides `uint64_t` for consistent 64-bit parameter passing. |
| Include Guard                  | Prevents multiple header inclusions.                         |
---

⚡**In short:**

`isr.h` defines the **link between the assembly-level interrupt stubs and the C-level kernel logic**, forming the backbone of interrupt handling in the OS.

---