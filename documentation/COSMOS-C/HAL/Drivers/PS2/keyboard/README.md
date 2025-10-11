# ⌨️ `HAL/Drivers/PS2/keyboard` — PS/2 Keyboard Driver

The `keyboard` folder inside `HAL/Drivers/PS2` contains the **source and header files** for handling PS/2 keyboard input. This driver is responsible for **reading keystrokes from the keyboard**, decoding scancodes, and delivering key events to the kernel.

---

## 📁 Typical Structure

└── keyboard/

|    ├── ps2.c

|    └── ps2.h

- **ps2.c** — Implementation of the PS/2 keyboard driver.
- **ps2.h** — Header file defining the keyboard driver API.
---
## 🔹 Purpose

1. **Initialize the PS/2 keyboard**:
   - Sets the keyboard controller to a known state.
   - Enables IRQ1 for keyboard input.
2. **Interrupt-driven input**:
   - Uses the **IRQ system** to handle key presses asynchronously.
   - Reduces CPU usage compared to polling.
3. **Scancode processing**:
   - Reads raw scancodes from the keyboard via I/O ports.
   - Converts scancodes into key codes or characters.
4. **Input abstraction**:
   - Provides higher-level functions to read key presses in the kernel.
   - Interfaces with the kernel console or other input consumers.
---
## 🔹 Key Functions (from `ps2.c`)

- **keyboard_irq_handler()** — Handles IRQ1 when a key is pressed.
- **ps2_init()** — Initializes the keyboard controller and enables interrupts.
- **read_scancode()** — Reads raw scancodes from the keyboard controller.
- **process_scancode()** — Converts scancodes to ASCII characters or special key codes.
---
## 🔹 Concepts

- **IRQ1**: Standard IRQ for the PS/2 keyboard.
- **Port I/O**: Uses `inb`/`outb` functions to communicate with the keyboard controller.
- **Scancodes**: Each key press/release generates a unique scancode, which the driver translates.
- **Interrupt-driven input**: Ensures the CPU responds only when a key is pressed.
---
## 📝 Summary

The `PS2/keyboard` folder implements the **low-level keyboard driver** for the kernel. It ensures:

- Reliable input through interrupts
- Accurate scancode-to-character translation
- Seamless integration with the kernel’s console or input system

It is a critical component for enabling **interactive text input** in the operating system.

---