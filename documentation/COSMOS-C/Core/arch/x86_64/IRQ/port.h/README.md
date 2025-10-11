# ⚙️ `port.h` — Low-Level I/O Port Access

## 📄 Overview
The **`port.h`** header provides a set of **inline assembly functions** that enable direct communication with hardware devices via **I/O ports** on x86/x86_64 architectures.

These functions are essential for:
- Configuring and controlling hardware (e.g., PIC, PS/2, PIT)
- Reading and writing device registers
- Implementing drivers at the bare-metal level (keyboard, timer, etc.)

---

## 🧩 Source Code

```c
#ifndef PORT_H
#define PORT_H

#include <stdint.h>

/**
 * @brief Read a byte from an I/O port.
 * 
 * @param port Port address to read from.
 * @return The byte value read from the port.
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * @brief Write a byte to an I/O port.
 * 
 * @param port Port address to write to.
 * @param val Byte value to write.
 */
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/**
 * @brief Read a word (16 bits) from an I/O port.
 * 
 * @param port Port address to read from.
 * @return The 16-bit word value read from the port.
 */
static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * @brief Write a word (16 bits) to an I/O port.
 * 
 * @param port Port address to write to.
 * @param val 16-bit word value to write.
 */
static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

#endif
```
---
## 🧠 Explanation
These functions act as **wrappers for CPU I/O instructions:**
| Function | Assembly Instruction | Description                                               |
| -------- | -------------------- | --------------------------------------------------------- |
| `inb()`  | `inb %dx, %al`       | Reads a byte from the port specified in `%dx` into `%al`. |
| `outb()` | `outb %al, %dx`      | Writes a byte from `%al` to the port in `%dx`.            |
| `inw()`  | `inw %dx, %ax`       | Reads a 16-bit word from the port into `%ax`.             |
| `outw()` | `outw %ax, %dx`      | Writes a 16-bit word from `%ax` to the port.              |

Each function uses inline **GCC assembly** with:
- `"a"` → accumulator register (`AL`/`AX`)
- `"Nd"` → allows constants or DX register as port operand
---
## 🧰 Typical Use Cases
| Subsystem            | Example Use                                           |
| -------------------- | ----------------------------------------------------- |
| 🖥️ **PIC**          | Send end-of-interrupt (`outb(0x20, 0x20)`)            |
| ⌨️ **PS/2 Keyboard** | Read keycode from port `0x60`                         |
| ⏱️ **PIT Timer**     | Configure frequency via ports `0x40–0x43`             |
| 🧱 **CMOS / RTC**    | Access real-time clock registers at ports `0x70–0x71` |
---
## ⚠️ Important Notes
- I/O ports are **architecture-specific** (x86 only).
- Using the wrong port can cause **undefined hardware behavior** or a **triple fault**.
- These functions are marked `static inline` to ensure:
    - No call overhead (inlined by the compiler)
    - Availability across multiple C files without linker conflicts.
---
## ✅ Summary
| Function | Type   | Bits   | Purpose             |
| -------- | ------ | ------ | ------------------- |
| `inb`    | Input  | 8-bit  | Read byte from port |
| `outb`   | Output | 8-bit  | Write byte to port  |
| `inw`    | Input  | 16-bit | Read word from port |
| `outw`   | Output | 16-bit | Write word to port  |
---
🧩 **In short:**

`port.h` provides **direct CPU-level I/O control**, forming the foundation for all hardware interaction in your OS kernel.

---