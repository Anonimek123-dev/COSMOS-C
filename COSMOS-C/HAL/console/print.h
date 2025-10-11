#pragma once

#include <stdint.h>
#include <stddef.h>


enum Colors {
    BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GRAY = 7,
	DARK_GRAY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	PINK = 13,
	YELLOW = 14,
	WHITE = 15,
};

void print_clear();
void print_char(char symbol);
void print_str(char* str);
void print_int(int integer);
void print_set_color(uint8_t foreground, uint8_t background);
void print_newline(void);
void print_set_cursor(size_t col, size_t row);
void print_update_cursor(void);
void draw_cursor(int start_col, int cursor_pos, int row, int blink_state);