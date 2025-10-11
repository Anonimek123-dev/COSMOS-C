## 🖥️ `print.c` — VGA Text Mode Console Implementation

This file provides the **implementation of the console printing functions** used by the kernel. It writes characters directly to the **VGA text buffer** at memory address `0xB8000`.

---

### 📌 Key Definitions

```c
const static size_t NUM_COLS = 80;  // screen width
const static size_t NUM_ROWS = 25;  // screen height

struct Char {
    uint8_t character; // ASCII character
    uint8_t color;     // foreground + background color
};

struct Char* buffer = (struct Char*) 0xb8000; // VGA buffer
size_t col = 0;   // current column
size_t row = 0;   // current row
uint8_t color = WHITE | BLACK << 4; // default text color
```

- `struct Char` represents a single cell in VGA text mode.
- `buffer` points to the memory-mapped VGA text buffer.
- `col` and `row` track the current cursor position.
- `color` stores the current foreground and background colors.

---
### 🧩 Core Functions
#### `print_clear()`

Clears the screen by zeroing out all rows and resets the cursor.

```c
void print_clear() {
    for (size_t r = 0; r < NUM_ROWS; r++) clear_row(r);
    col = 0;
    row = 0;
    print_update_cursor();
}
```
---
#### `print_char(char character)`
Prints a single character, handling special cases like:

- `'\n'` — newline
- `'\b'` — backspace
- Line wrapping when `col >= NUM_COLS`

```c
buffer[col + NUM_COLS * row] = (struct Char){ .character = (uint8_t)character, .color = color };
col++;
print_update_cursor();
```
---
#### `print_str(char* str)`
Prints a null-terminated string by calling `print_char()` repeatedly.
```c
for (size_t i = 0;; i++) {
    if (str[i] == '\0') return;
    print_char(str[i]);
}
```
---
#### `print_int(int integer)`
Prints an integer in decimal, supporting negative numbers.
```c
char buf[12]; // buffer for digits
// convert integer to string and print in reverse
```
---
#### Cursor Handling
- `print_set_cursor(size_t col_, size_t row_)` — Updates the hardware cursor on screen using ports `0x3D4` and `0x3D5`.
- `print_update_cursor()` — Calls `print_set_cursor()` to update the cursor after printing.
- `draw_cursor()` — Optional helper for positioning the cursor in text UI elements.
---
### Color Handling
```c
void print_set_color(uint8_t fg, uint8_t bg) {
    color = fg | (bg << 4);
}
```
- `fg` = foreground color
- `bg` = background color
- Combines both into a single byte for VGA text mode.
---
### 📌 Miscellaneous

- **Scrolling**: When printing beyond the last row, all rows are shifted up and the last row is cleared.
- **Direct memory access**: Writes directly to `0xB8000`, no BIOS calls are used.
- **Cursor updates**: Always synced with `col` and `row`.
---
### 📝 Summary
`print.c` provides **low-level console output** for the kernel, including:

- Character and string printing
- Integer printing
- Cursor management
- Color control
- Scrolling

It serves as a **debug and output layer** before any higher-level driver or framebuffer is initialized.

---