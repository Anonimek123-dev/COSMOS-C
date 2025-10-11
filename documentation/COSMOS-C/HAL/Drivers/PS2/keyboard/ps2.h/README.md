# 🧩 `ps2.h` — Full Technical Documentation

This file defines the **PS/2 keyboard driver interface** and provides **symbolic key codes** for use across the kernel.  
It’s located in:  
`HAL/Drivers/PS2/keyboard/ps2.h`

---
## 🧱 1. Include Guard
```c
#pragma once
```
Prevents the file from being included multiple times during compilation,
avoiding duplicate symbol definitions.

---
## 📚 2. Includes
```c
#include <stdint.h>
```
Includes fixed-width integer types (e.g., **uint8_t**, **uint16_t**),
ensuring consistent type usage across low-level hardware code.

---
## 🔤 3. Basic ASCII Keys
```c
#define KEY_ESC       0x1B
#define KEY_BACKSPACE 0x08
#define KEY_TAB       0x09
#define KEY_SPACE     0x20
```
These correspond directly to standard **ASCII codes**.
They represent printable control keys that can be displayed or interpreted directly in text output.

---
## 🧭 4. Cursor & Editing Keys
```c
#define KEY_UP        0x80
#define KEY_DOWN      0x81
#define KEY_LEFT      0x82
#define KEY_RIGHT     0x83
#define KEY_DELETE    0x7F
#define KEY_INSERT    0x86
#define KEY_HOME      0x84
#define KEY_END       0x85
#define KEY_PGUP      0x87
#define KEY_PGDN      0x88
```
- All have values ≥ `0x80` to avoid clashing with ASCII.
- These represent **non-printable control keys**.
- Used by terminal, shell, and GUI subsystems for navigation or text editing.
---
## ⚙️ 5. Function Keys (F1–F12)
```c
#define KEY_F1        0x90
#define KEY_F2        0x91
#define KEY_F3        0x92
#define KEY_F4        0x93
#define KEY_F5        0x94
#define KEY_F6        0x95
#define KEY_F7        0x96
#define KEY_F8        0x97
#define KEY_F9        0x98
#define KEY_F10       0x99
#define KEY_F11       0x9A
#define KEY_F12       0x9B 
```
- Unique codes for **function keys**.
- Makes it easy to detect shortcuts or special kernel commands.
---
## 🧠 6. Extra Function Keys (F13–F24)
```c
#define KEY_F13       0xF0
#define KEY_F14       0xF1
#define KEY_F15       0xF2
#define KEY_F16       0xF3
#define KEY_F17       0xF4
#define KEY_F18       0xF5
#define KEY_F19       0xF6
#define KEY_F20       0xF7
#define KEY_F21       0xF8
#define KEY_F22       0xF9
#define KEY_F23       0xFA
#define KEY_F24       0xFB
```
- Rare, but some professional or server keyboards include these.
- Defined for completeness and future-proofing.
---
## 🧷 7. Modifier Keys
```c
#define KEY_LSHIFT    0xA0
#define KEY_RSHIFT    0xA1
#define KEY_CTRL      0xA2
#define KEY_ALT       0xA4
#define KEY_CAPSLOCK  0xA6
#define KEY_NUMLOCK   0xA7
#define KEY_SCROLL    0xA8
```
Represent keys that modify the behavior of others (e.g., `Shift`, `Ctrl`, `Alt`).
Toggle keys like `CapsLock`, `NumLock`, and `ScrollLock` also have dedicated codes.

---
## 🔢 8. Numeric Keypad (Numpad)
```c
#define KEY_KP0       0xB0
#define KEY_KP1       0xB1
#define KEY_KP2       0xB2
#define KEY_KP3       0xB3
#define KEY_KP4       0xB4
#define KEY_KP5       0xB5
#define KEY_KP6       0xB6
#define KEY_KP7       0xB7
#define KEY_KP8       0xB8
#define KEY_KP9       0xB9
#define KEY_KP_DOT    0xBA
#define KEY_KP_MUL    0xBC
#define KEY_KP_MINUS  0xBD
#define KEY_KP_PLUS   0xBE
#define KEY_KP_ENTER  0xBF
#define KEY_KP_EQUAL  0xFC
```
- Represents the **right-side numeric keypad keys**.
- Distinguishes between main number row (`'1'`, `'2'`, etc.) and numpad input.
---
## 🎛️ 9. System & Media Keys
```c
#define KEY_PRTSCR    0xC0
#define KEY_PAUSE     0xC1
#define KEY_SYSRQ     0xFD

#define KEY_LWIN      0xC2
#define KEY_RWIN      0xC3
#define KEY_MENU      0xC4

#define KEY_POWER     0xC5
#define KEY_SLEEP     0xC6
#define KEY_WAKE      0xC7

#define KEY_MUTE      0xC8
#define KEY_VOLUP     0xC9
#define KEY_VOLDOWN   0xCA
#define KEY_PLAY      0xCB
#define KEY_STOP      0xCC
#define KEY_NEXT      0xCD
#define KEY_PREV      0xCE
#define KEY_MAIL      0xCF
#define KEY_WWW       0xD0
```
### Categories:
- **System Control**: `Print Screen`, `Pause`, `SysRq`
- **Windows/Meta Keys**: `LWIN`, `RWIN`, `MENU`
- **Power Management**: `POWER`, `SLEEP`, `WAKE`
- **Media Controls**: `PLAY`, `STOP`, `NEXT`, `VOLUP`, `MUTE`, etc.

These are derived from **extended scancodes** (`E0` prefix in PS/2 protocol).

---
## 🧩 10. Driver API
At the end, the header defines the public API used by other parts of the kernel:
```c
void keyboard_init(void);
int  keyboard_getchar(void);
void keyboard_irq_handler(void);
```
### 🔹 `keyboard_init()`

Initializes the PS/2 keyboard driver, resets internal buffers,
and configures the hardware interface.

### 🔹 `keyboard_getchar()`

Returns the next character (or key code) from the keyboard buffer.
If no key is available, it returns `-1`.

### 🔹 `keyboard_irq_handler()`

Called automatically when **IRQ1** fires (keyboard interrupt).
It reads the scancode from port `0x60`, decodes it, and stores it in the input buffer.

---
## 🔗 11. Dependencies
This header cooperates with:
- `ps2.c` → actual implementation of logic and IRQ handling.
- `arch/x86_64/IRQ/port.h` → low-level I/O (`inb`, `outb`).
- `HAL/console/print.h` → optional debug/print output.
- `IDT subsystem` → connects `keyboard_irq_handler` to IRQ1.
---
## 🧮 12. Summary Table
| Category         | Description                                           |
| ---------------- | ----------------------------------------------------- |
| 🔡 ASCII keys    | Basic printable or control characters                 |
| 🧭 Cursor keys   | Navigation and editing controls                       |
| ⚙️ Function keys | F1–F24 for shortcuts and advanced input               |
| 🧷 Modifiers     | Shift, Ctrl, Alt, Lock keys                           |
| 🔢 Numpad        | Numeric keypad-specific keys                          |
| 🎵 Media keys    | Multimedia and system control                         |
| ⚙️ API           | Functions for init, IRQ handling, and input retrieval |
---
## 🧠 13. Concept Overview

`ps2.h` acts as a bridge between hardware-level keyboard logic
and higher-level input systems (like the kernel shell or GUI).
It exposes simple, human-readable constants and a clean API, hiding
low-level scancode handling and hardware interrupts.

---
## ✅ In short:
`ps2.h` provides a unified interface for PS/2 keyboard handling —
offering readable key codes, clear function prototypes,
and cross-module compatibility throughout the OS kernel.

---