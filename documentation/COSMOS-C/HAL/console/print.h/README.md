# Console Printing API (`print.h`)

## Overview
This header provides **console printing and text display utilities** for the kernel.  
It supports colored text, cursor control, and basic character/integer output.

---

## Enumerations

### `enum Colors`

Defines the color codes for foreground and background text:

| Name        | Value |
| ----------- | ----- |
| BLACK       | 0     |
| BLUE        | 1     |
| GREEN       | 2     |
| CYAN        | 3     |
| RED         | 4     |
| MAGENTA     | 5     |
| BROWN       | 6     |
| LIGHT_GRAY  | 7     |
| DARK_GRAY   | 8     |
| LIGHT_BLUE  | 9     |
| LIGHT_GREEN | 10    |
| LIGHT_CYAN  | 11    |
| LIGHT_RED   | 12    |
| PINK        | 13    |
| YELLOW      | 14    |
| WHITE       | 15    |

---

## Functions

### `void print_clear()`
Clears the console screen and resets the cursor position.

### `void print_char(char symbol)`
Prints a single character at the current cursor position.

### `void print_str(char* str)`
Prints a null-terminated string starting at the current cursor position.

### `void print_int(int integer)`
Prints an integer as a decimal number.

### `void print_set_color(uint8_t foreground, uint8_t background)`
Sets the current text color.

- `foreground` → Foreground color (use `enum Colors`)
- `background` → Background color (use `enum Colors`)

### `void print_newline(void)`
Moves the cursor to the beginning of the next line.

### `void print_set_cursor(size_t col, size_t row)`
Sets the cursor position to the specified column and row.

### `void print_update_cursor(void)`
Updates the hardware cursor to match the internal cursor position.

### `void draw_cursor(int start_col, int cursor_pos, int row, int blink_state)`
Draws the cursor at the given position, optionally blinking.

- `start_col` → Starting column of the text line
- `cursor_pos` → Current cursor column
- `row` → Row index
- `blink_state` → Controls blinking (on/off)
---