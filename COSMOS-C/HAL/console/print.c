#include "arch/x86_64/IRQ/port.h"
#include "HAL/console/print.h"
#include <stddef.h>
#include <stdint.h>

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = WHITE | BLACK << 4;

void clear_row(size_t row) {
    struct Char empty = { .character = ' ', .color = color };
    for (size_t c = 0; c < NUM_COLS; c++) {
        buffer[c + NUM_COLS * row] = empty;
    }
}

void print_clear() {
    for (size_t r = 0; r < NUM_ROWS; r++) clear_row(r);
    col = 0;
    row = 0;
    print_update_cursor();
}

void print_newline() {
    col = 0;
    if (row < NUM_ROWS - 1) {
        row++;
    } else {
        // scroll
        for (size_t r = 1; r < NUM_ROWS; r++) {
            for (size_t c = 0; c < NUM_COLS; c++) {
                buffer[c + NUM_COLS * (r - 1)] = buffer[c + NUM_COLS * r];
            }
        }
        clear_row(NUM_ROWS - 1);
    }
    print_update_cursor();
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }
    if (character == '\b') {
        if (col == 0) {
            if (row == 0) return;
            row--;
            col = NUM_COLS - 1;
        } else {
            col--;
        }
        buffer[col + NUM_COLS * row] = (struct Char){ .character = ' ', .color = color };
        print_update_cursor();
        return;
    }
    if (col >= NUM_COLS) print_newline();
    buffer[col + NUM_COLS * row] = (struct Char){ .character = (uint8_t)character, .color = color };
    col++;
    print_update_cursor();
}

void print_str(char* str) {
    for (size_t i = 0;; i++) {
        if (str[i] == '\0') return;
        print_char(str[i]);
    }
}

void print_int(int integer) {
    char buf[12];
    int i = 0;
    int neg = 0;
    if (integer == 0) { print_char('0'); return; }
    if (integer < 0) { neg = 1; integer = -integer; }
    while (integer > 0) { buf[i++] = '0' + (integer % 10); integer /= 10; }
    if (neg) buf[i++] = '-';
    for (int j = i - 1; j >= 0; j--) print_char(buf[j]);
}

void print_set_color(uint8_t fg, uint8_t bg) {
    color = fg | (bg << 4);
}

void print_set_cursor(size_t col_, size_t row_) {
    // Ustawiamy globalne col/row tak, żeby print_char() pisał we właściwe miejsce
    col = col_;
    row = row_;

    uint16_t pos = row * NUM_COLS + col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}


void print_update_cursor() {
    print_set_cursor(col, row);
}

void draw_cursor(int start_col, int cursor_pos, int row, int blink_state){
    (void) blink_state;
    print_set_cursor((size_t)(start_col + cursor_pos), (size_t)row);
}
