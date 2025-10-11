# 🖥️ Folder: `HAL/console`

The **`console`** folder inside the HAL (Hardware Abstraction Layer) contains the code responsible for **outputting text to the screen**.  
It abstracts the VGA text mode and provides simple printing functions for the kernel.

---

## 📂 Structure

- **`print.c`** — Implements the actual logic to write characters, strings, and numbers to the VGA text buffer.
- **`print.h`** — Header file that exposes the functions for use by other parts of the kernel.

---

## 🧩 Purpose

1. **Hardware Abstraction**  
   The console module hides the details of writing to memory-mapped VGA buffers, allowing the kernel to print text without directly managing hardware.

2. **Debugging and Logging**  
   Provides functions to display messages, errors, and debug information on the screen during early boot and runtime.

3. **Simple Output API**  
   Common functions include:
   - `print_str(const char* str)` — Print a string to the screen.
   - `print_char(char c)` — Print a single character.
   - `print_int(int num)` — Print an integer.
   - Additional helpers for formatting or newline handling.

---

## ⚡ Example Use

```c
#include "HAL/console/print.h"

print_str("Kernel initialized.\n");
print_int(42);
This prints "Kernel initialized." followed by the number 42 to the VGA console.

```
---
## 📝 Summary
The HAL/console folder provides a simple and consistent API for text output, essential for kernel debugging and runtime logging before more advanced I/O systems (like framebuffer graphics) are initialized.

---