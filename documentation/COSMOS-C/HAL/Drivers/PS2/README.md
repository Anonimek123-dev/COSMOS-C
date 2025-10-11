# ⌨️ `Drivers/PS2` — PS/2  Drivers

The `PS2` folder inside `HAL/Drivers` contains **drivers for PS/2 input devices**, primarily keyboards (and potentially mice). These drivers allow the kernel to **read key presses, interpret scancodes**, and integrate input with the IRQ system.

---

## 📁 Typical Structure

- PS2/

- keyboard/
    - ps2.c

    - ps2.h

- **keyboard/** — Handles PS/2 keyboard input.
---

## 🔹 Purpose

1. **Device initialization**: Sets up the PS/2 controller and keyboard to start sending scancodes.
2. **Interrupt handling**: Connects the keyboard IRQ to the kernel’s IRQ system via PIC.
3. **Scancode translation**: Converts raw scancodes into meaningful key events.
4. **Abstraction**: Provides a clean API to higher-level input subsystems.

---
## 🔹 Key Concepts

- **IRQ-driven input**: Avoids busy-wait polling; the CPU only handles key presses when an interrupt occurs.
- **Scancodes**: Raw codes from the keyboard which must be mapped to characters.
- **Controller commands**: PS/2 devices are controlled using specific command bytes sent to the controller via I/O ports.

---

## 📝 Summary

`HAL/Drivers/PS2` provides **essential input functionality** for the kernel:

- Initializes PS/2 devices
- Reads and processes scancodes
- Integrates with the kernel's IRQ system
- Provides a clean API for keyboard input handling

This folder forms the foundation for any **text-based interface** in the OS, like a kernel console.

---