# ⌨️ `ps2.c` — Detailed PS/2 Keyboard Driver Overview

This file implements a complete **PS/2 keyboard driver** for an x86_64 kernel. It handles **scancode translation**, **modifier and lock key tracking**, **LED control**, **input buffering**, and **interactive console input management**. Below is a detailed breakdown of every component and its functionality.

---
## 1️⃣ Constants & Ports

```c
#define PS2_DATA_PORT 0x60
#define PS2_CMD_PORT  0x64
#define KB_BUF_SIZE 256
```
- **PS2_DATA_PORT (0x60)**: Used to read scancodes sent by the keyboard.
- **PS2_CMD_PORT (0x64)**: Used to send commands to the keyboard controller.
- **KB_BUF_SIZE**: Size of the circular buffer for storing key presses (256 bytes).
```c
#define LINE_BUF_SIZE 128
#define HISTORY_SIZE 16
#define TAB_SIZE 4
#define BLINK_THRESHOLD 20000
```
- **LINE_BUF_SIZE**: Maximum characters per input line.
- **HISTORY_SIZE**: Maximum stored lines for history navigation.
- **TAB_SIZE**: Number of spaces to insert for a tab.
- **BLINK_THRESHOLD**: Number of "ticks" before toggling the cursor blink state.
---
## 2️⃣ Modifier & Lock States

The driver tracks the state of modifier and lock keys:

```c
static bool shift_pressed = false;
static bool ctrl_pressed  = false;
static bool alt_pressed   = false;
static bool capslock_on   = false;
static bool numlock_on    = true;
static bool scrolllock_on = false;
static bool extended      = false;
static bool pause_seq     = false;
```
- **shift_pressed**: Tracks whether either Shift key is pressed.
- **ctrl_pressed**: Tracks Control key.
- **alt_pressed**: Tracks Alt key.
- **capslock_on / numlock_on / scrolllock_on**: Reflect the lock keys’ state.
- **extended**: True when an `0xE0` prefix is received (extended key).
- **pause_seq**: True when beginning a multi-byte Pause key sequence (`0xE1`).
---
## 3️⃣ Circular Input Buffer
The keyboard driver uses a circular buffer for non-blocking input:

```c
static uint8_t kb_buf[KB_BUF_SIZE];
static volatile int kb_head = 0;
static volatile int kb_tail = 0;
```
- `kb_put(c)`: Adds a key code to the buffer, skipping if full.
- `kb_get()`: Retrieves the next key code from the buffer or returns -1 if empty.
- `keyboard_getchar()`: Public API for fetching the next key press.

**Why circular buffer?**

It allows **interrupt-driven input** without losing keys and supports asynchronous processing in the kernel.

---
## 4️⃣ LED Control
```c
static void kb_update_leds(void);
```
- Sends **0xED command** to the keyboard to set LED status.
- Constructs a bitmask:
```txt
bit 0: ScrollLock
bit 1: NumLock
bit 2: CapsLock
```
- Updates the keyboard LEDs whenever lock keys change.
---
## 5️⃣ Scancode Translation Tables
Two tables convert scancodes to ASCII:

```c
static unsigned char scancode2ascii[128];
static unsigned char scancode2ascii_shift[128];
```
- scancode2ascii: Normal ASCII characters without Shift.
- scancode2ascii_shift: ASCII characters when Shift is active.
- Extended keys and special keys are handled separately.
---
## 6️⃣ Scancode Processing
```c
static char translate_scancode(uint8_t code);
```
- Handles **Pause** (`0xE1`) and **PrintScreen** sequences.
- Detects **key release** (`code & 0x80`).
- Updates modifier states for Shift, Ctrl, Alt.
- Updates lock states (CapsLock, NumLock, ScrollLock) and triggers LED updates.
- Converts **normal key presses** into ASCII using `scancode2ascii` or `scancode2ascii_shift`.
---
## 7️⃣ Extended Key Handling
```c
static void handle_extended(uint8_t code, bool released);
```
- Processes extended key sequences (`0xE0` prefix).
- Maps keys like **arrow keys, Home/End, Insert/Delete, Media keys,** and **Windows keys**.
- Only handles key presses (not releases).
```c
static bool handle_printscreen(uint8_t code);
```
- Detects multi-byte **PrintScreen sequences**.
- Converts them to a special key (`KEY_PRTSCR`) in the buffer.
---
## 8️⃣ IRQ Handler
```c
void keyboard_irq_handler(void);
```
- Triggered by **IRQ1** (keyboard interrupt).
- Reads the scancode from port `0x60`.
- Converts it to ASCII (or a special key) using `translate_scancode`.
- Adds the result to the circular buffer (`kb_put`).
---
### 9️⃣ Console Update / Line Editing
```c
void kb_update(void);
```
This function **manages interactive editing**:

- Fetches characters from the buffer (`keyboard_getchar()`).
- Handles:
    - **Enter**: finishes line, updates history.
    - **Backspace/Delete**: removes character.
    - **Arrow keys**: moves cursor; navigates history.
    - **Home/End**: jumps to start/end of line.
    - **Tab**: inserts spaces.
- Maintains a **history buffer** of the last 16 lines.
- Tracks cursor position, blinking, and updates the screen with `print_char` and `draw_cursor`.
- Cursor blink is implemented via `blink_counter` and `BLINK_THRESHOLD`.

**Internal State Variables:**
```c
static char buffer[LINE_BUF_SIZE];   // current input line
static int line_len;                 // current line length
static int cursor_pos;               // cursor position
static int start_col;                // column offset
static char history[HISTORY_SIZE][LINE_BUF_SIZE];
static int history_len;              // total number of history lines
static int history_pos;              // current position in history
static int blink_state;              // cursor on/off
static int blink_counter;            // blink timer
```
---
### 🔟 Initialization
```c
void keyboard_init(void);
```
- Resets buffer and state variables.
- Resets Pause sequence and LED status.
- Calls `kb_update_leds()` to reflect lock states.
- Prints `"PS/2 KEYBOARD DRIVER INITIALIZED\n"` to the console.
---
## 1️⃣1️⃣ Summary of Functionality

1. **Low-Level Input Handling:**
    - Reads scancodes directly from PS/2 ports.
    - Processes both standard and extended keys.
2. **Modifier & Lock Management:**
    - Shift, Ctrl, Alt, CapsLock, NumLock, ScrollLock.
    - Updates LED indicators.
3. **Circular Buffering:**
    - Stores key presses asynchronously.
    - Prevents lost keys during kernel processing.
4. **Interactive Console Support:**
    - Cursor movement and blinking.
    - Line editing (insert, delete, backspace, tab, home/end).
    - Command history navigation.
5. **Interrupt-Based Operation:**
    - IRQ1 triggers handler for responsive input.
6. **Scancode to ASCII Conversion:**
    - Handles normal keys, shifted keys, numpad keys, function keys, and media keys.
---
### 🔹 Integration Notes
- Requires `port.h` for I/O operations.
- Depends on `HAL/console/print.h` for displaying characters.
- Works with the kernel console (`row` and `col` variables) for proper cursor positioning.
- Can be extended to support additional keys or custom macros.
---
This driver provides **complete PS/2 keyboard support** suitable for low-level kernel development, supporting a responsive, interactive console environment.

---