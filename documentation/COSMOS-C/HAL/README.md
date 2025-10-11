# 🏗️ Folder: `HAL` — Hardware Abstraction Layer

The **HAL (Hardware Abstraction Layer)** folder provides a set of platform-independent interfaces for interacting with hardware.  
It acts as a bridge between low-level hardware drivers and higher-level OS components, allowing the kernel to operate without worrying about hardware-specific details.

---
## 🧩 Purpose

1. **Encapsulation**  
   HAL abstracts direct hardware access, so kernel code doesn’t need to deal with port I/O or memory-mapped registers directly.

2. **Portability**  
   By using HAL functions, OS code can be reused across different hardware platforms or architectures with minimal changes.

3. **Consistency**  
   Provides unified interfaces for common operations like printing to the console, reading keyboard input, or handling timers.

---

## ⚡ Examples of HAL Modules

- **`console/print.c` and `print.h`**  
  Functions to write strings, characters, and numbers to the VGA text buffer.

- **`keyboard/ps2.c` and `ps2.h`**  
  Abstraction for PS/2 keyboard input handling (sometimes under `Drivers` or `HAL` depending on design).

---

## 📝 Summary

The `HAL` folder provides **clean, hardware-independent APIs** so that the rest of the OS can function without dealing with the quirks of each piece of hardware.  
It’s an essential layer for building a maintainable and modular OS kernel.

---