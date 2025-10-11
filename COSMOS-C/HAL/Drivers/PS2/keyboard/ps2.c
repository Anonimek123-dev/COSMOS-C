#include "Drivers/PS2/keyboard/ps2.h"
#include "arch/x86_64/IRQ/port.h"
#include "console/print.h"
#include <stdbool.h>
#include <stdint.h>

// ===================== CONSTANTS =====================
// PS/2 ports and buffer size
#define PS2_DATA_PORT 0x60
#define PS2_CMD_PORT  0x64
#define KB_BUF_SIZE 256

// ===================== MODIFIER STATES =====================
static bool shift_pressed     = false;  // Shift key state
static bool ctrl_pressed      = false;  // Control key state
static bool alt_pressed       = false;  // Alt key state
static bool capslock_on       = false;  // CapsLock state
static bool numlock_on        = true;   // NumLock state
static bool scrolllock_on     = false;  // ScrollLock state
static bool extended          = false;  // Extended key prefix
static bool pause_seq         = false;  // Pause key sequence

// ===================== SPECIAL TRACKING =====================
static uint8_t ps_seq[4];  // Stores multi-byte key sequences
static int ps_index = 0;

// ===================== BUFFER =====================
static uint8_t kb_buf[KB_BUF_SIZE];       // Circular buffer for key presses
static volatile int kb_head = 0;          // Head index
static volatile int kb_tail = 0;          // Tail index

// Put character into buffer
static inline void kb_put(uint8_t c) {
    int next = (kb_head + 1) % KB_BUF_SIZE;
    if (next == kb_tail) return;  // Buffer full
    kb_buf[kb_head] = c;
    kb_head = next;
}

// Get character from buffer
static int kb_get(void) {
    if (kb_head == kb_tail) return -1;  
    int c = kb_buf[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUF_SIZE;
    return c;
}

// API function: get next key press
int keyboard_getchar(void) { return kb_get(); }

// ===================== LED UPDATE =====================
// Update keyboard LEDs based on lock states
static void kb_update_leds(void) {
    uint8_t mask = (scrolllock_on ? 1 : 0)
                 | (numlock_on    ? 2 : 0)
                 | (capslock_on   ? 4 : 0);

    while (inb(PS2_CMD_PORT) & 2);
    outb(PS2_DATA_PORT, 0xED);  // Command to set LEDs

    while (inb(PS2_CMD_PORT) & 2);
    outb(PS2_DATA_PORT, mask);  // Send LED mask
}

// ===================== ASCII TABLES =====================
static unsigned char scancode2ascii[128] = {
    // Normal keys without shift
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',
    0,0,0,0,0,0,0,0,0,0,
    '7','8','9','-',
    '4','5','6','+',
    '1','2','3','0','.',
    0,0,0,0,0,0,0,0,0,0
};

static unsigned char scancode2ascii_shift[128] = {
    // Shifted keys
    0,27,'!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,
    'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
    'Z','X','C','V','B','N','M','<','>','?',0,'*',0,' ',
    0,0,0,0,0,0,0,0,0,0,
    '7','8','9','-',
    '4','5','6','+',
    '1','2','3','0','.',
    0,0,0,0,0,0,0,0,0,0
};

static void handle_extended(uint8_t code, bool released);
static bool handle_printscreen(uint8_t code); 

// ===================== TRANSLATOR =====================
// Translate PS/2 scancode to ASCII or special key
static char translate_scancode(uint8_t code) {
    if (pause_seq) { pause_seq = false; kb_put(KEY_PAUSE); return 0; }
    if (code == 0xE1) { pause_seq = true; return 0; }
    if (handle_printscreen(code)) return 0;
    if (code == 0xE0) { extended = true; return 0; }

    bool released = (code & 0x80) != 0;
    code &= 0x7F;

    if (code == 0x54 && !released) { kb_put(KEY_SYSRQ); return 0; }
    if (code == 0x59 && !released) { kb_put(KEY_KP_EQUAL); return 0; }
    
    // Handle modifier keys and locks
    switch (code) {
        case 0x2A: shift_pressed = !released; if (!released) /*kb_put(KEY_LSHIFT);*/ return 0;
        case 0x36: shift_pressed = !released; if (!released) /*kb_put(KEY_RSHIFT);*/ return 0;
        case 0x1D: ctrl_pressed = !released; if (!released) { /*kb_put(KEY_CTRL);*/ print_str("[Ctrl]"); } return 0;
        case 0x38: alt_pressed=!released; if(!released){ /*kb_put(KEY_ALT);*/ print_str("[Alt]"); } return 0;
        case 0x3A: if (!released){ capslock_on=!capslock_on; kb_update_leds(); /*kb_put(KEY_CAPSLOCK);*/} return 0;
        case 0x45: if (!released){ numlock_on =!numlock_on; kb_update_leds(); /*kb_put(KEY_NUMLOCK);*/} return 0;
        case 0x46: if (!released){ scrolllock_on=!scrolllock_on; kb_update_leds(); /*kb_put(KEY_SCROLL);*/} return 0;
    }

    // Handle extended keys
    if (extended) { handle_extended(code, released); extended = false; return 0; }

    if (!released) {
        switch (code) {
            case 0x01: print_str("[Esc]"); return 0;
            case 0x3B: print_str("[F1]");  return 0;
            case 0x3C: print_str("[F2]");  return 0;
            case 0x3D: print_str("[F3]");  return 0;
            case 0x3E: print_str("[F4]");  return 0;
            case 0x3F: print_str("[F5]");  return 0;
            case 0x40: print_str("[F6]");  return 0;
            case 0x41: print_str("[F7]");  return 0;
            case 0x42: print_str("[F8]");  return 0;
            case 0x43: print_str("[F9]");  return 0;
            case 0x44: print_str("[F10]"); return 0;
            case 0x57: print_str("[F11]"); return 0;
            case 0x58: print_str("[F12]"); return 0;
            case 0x64: print_str("[F13]"); return 0;
            case 0x65: print_str("[F14]"); return 0;
            case 0x66: print_str("[F15]"); return 0;
            case 0x67: print_str("[F16]"); return 0;
            case 0x68: print_str("[F17]"); return 0;
            case 0x69: print_str("[F18]"); return 0;
            case 0x6A: print_str("[F19]"); return 0;
            case 0x6B: print_str("[F20]"); return 0;
            case 0x6C: print_str("[F21]"); return 0;
            case 0x6D: print_str("[F22]"); return 0;
            case 0x6E: print_str("[F23]"); return 0;
            case 0x6F: print_str("[F24]"); return 0;
        }
    }

    if (released) return 0;

    // Handle numpad keys
    switch (code) {
        case 0x47: if (numlock_on) return '7'; else kb_put(KEY_HOME); return 0;
        case 0x48: if (numlock_on) return '8'; else kb_put(KEY_UP);   return 0;
        case 0x49: if (numlock_on) return '9'; else print_str("[PageUp]"); return 0;
        case 0x4A: if (numlock_on) return '-'; else kb_put(KEY_KP_MINUS); return 0;        
        case 0x4B: if (numlock_on) return '4'; else kb_put(KEY_LEFT); return 0;
        case 0x4C: if (numlock_on) return '5'; else print_char(' ');  return 0;
        case 0x4D: if (numlock_on) return '6'; else kb_put(KEY_RIGHT);return 0;
        case 0x4E: if (numlock_on) return '+'; else kb_put(KEY_KP_PLUS);  return 0;
        case 0x4F: if (numlock_on) return '1'; else kb_put(KEY_END);  return 0;
        case 0x50: if (numlock_on) return '2'; else kb_put(KEY_DOWN); return 0;
        case 0x51: if (numlock_on) return '3'; else print_str("[PageDown]"); return 0;
        case 0x52: if (numlock_on) return '0'; else kb_put(KEY_INSERT); return 0;
        case 0x53: if (numlock_on) return '.'; else kb_put(KEY_DELETE); return 0;
        case 0x37: if (numlock_on) return '*'; else kb_put(KEY_KP_MUL); return 0;
    }

    // Translate to ASCII using shift/capslock
    if (shift_pressed ^ capslock_on) return scancode2ascii_shift[code];
    else return scancode2ascii[code];
}

// ===================== SPECIAL HANDLERS =====================
// Handle extended keys (E0 prefix)

static void handle_extended(uint8_t code, bool released) {
    if (released) return;  // Only handle key press
    switch (code) {
        case 0x1C: kb_put('\n'); break;
        case 0x1D: ctrl_pressed = !released; if (!released) { /*kb_put(KEY_CTRL);*/ print_str("[Ctrl]"); } break;
        case 0x35: kb_put('/'); break;
        case 0x38: alt_pressed= !released; if (!released) { /*kb_put(KEY_ALT);*/ print_str("[Alt]"); } break; 
        case 0x47: kb_put(KEY_HOME); break;
        case 0x48: kb_put(KEY_UP); break;
        case 0x49: /*kb_put(KEY_PGUP);*/ print_str("[PageUp]"); break; 
        case 0x4B: kb_put(KEY_LEFT); break;
        case 0x4D: kb_put(KEY_RIGHT); break;
        case 0x4F: kb_put(KEY_END); break;
        case 0x50: kb_put(KEY_DOWN); break;
        case 0x51: /*kb_put(KEY_PGDN);*/ print_str("[PageDown]"); break; 
        case 0x52: /*kb_put(KEY_INSERT);*/ print_str("[Insert]"); break; 
        case 0x53: kb_put(KEY_DELETE); break;
        case 0x5B: /*kb_put(KEY_LWIN);*/ print_str("[Left Windows]"); break;  
        case 0x5C: /*kb_put(KEY_RWIN);*/ print_str("[Right Windows]"); break; 
        case 0x5D: kb_put(KEY_MENU); break;
        case 0x20: kb_put(KEY_MUTE); break;
        case 0x30: kb_put(KEY_VOLUP); break;
        case 0x2E: kb_put(KEY_VOLDOWN); break;
        case 0x22: kb_put(KEY_PLAY); break;
        case 0x24: kb_put(KEY_STOP); break;
        case 0x19: kb_put(KEY_NEXT); break;
        case 0x10: kb_put(KEY_PREV); break;
        case 0x5E: kb_put(KEY_POWER); break;
        case 0x5F: kb_put(KEY_SLEEP); break;
        case 0x63: kb_put(KEY_WAKE); break;
        case 0x6C: kb_put(KEY_MAIL); break;
        case 0x32: kb_put(KEY_WWW); break;
        default: break;
    }
}

// Handle PrintScreen sequence
static bool handle_printscreen(uint8_t code) {
    ps_seq[ps_index++] = code;
    if (ps_index >= 4) {
        if (ps_seq[0]==0xE0 && ps_seq[1]==0x2A &&
            ps_seq[2]==0xE0 && ps_seq[3]==0x37) {
            kb_put(KEY_PRTSCR); ps_index = 0; return true;
        }
        if (ps_seq[0]==0xE0 && ps_seq[1]==0xB7 &&
            ps_seq[2]==0xE0 && ps_seq[3]==0xAA) {
            ps_index = 0; return true;
        }
        ps_index = 0;
    }
    return false;
}

// ===================== IRQ HANDLER =====================
// Called on PS/2 interrupt
void keyboard_irq_handler(void) {
    uint8_t sc = inb(PS2_DATA_PORT);
    char c = translate_scancode(sc);

    if (c) { 
        kb_put(c); 
    }
}

// ================ UPDATE PER FRAME ==================

extern void enable_irq(void);
extern size_t row; 
extern size_t col;   

#define LINE_BUF_SIZE 128
#define HISTORY_SIZE 16
#define TAB_SIZE 4
#define BLINK_THRESHOLD 20000

static char buffer[LINE_BUF_SIZE];
static int line_len = 0;
static int cursor_pos = 0;
static int start_col;

static char history[HISTORY_SIZE][LINE_BUF_SIZE];
static int history_len = 0;   // History Total Length
static int history_pos = -1;  // Current Position in History

static int blink_state = 0;   // Blinker State
static int blink_counter = 0; // "Ticks" counter

static inline void sanitize_cursor() {
    if (line_len < 0) line_len = 0;
    if (line_len > LINE_BUF_SIZE - 1) line_len = LINE_BUF_SIZE - 1;
    if (cursor_pos < 0) cursor_pos = 0;
    if (cursor_pos > line_len) cursor_pos = line_len;
}

void kb_update() {
    print_set_color(WHITE, BLACK);

    int ci = keyboard_getchar();
    if (ci < 0) {
        blink_counter++;
        if (blink_counter > BLINK_THRESHOLD) {
            blink_counter = 0;
            blink_state = !blink_state;
            draw_cursor(start_col, cursor_pos, row, blink_state);
        }
        return;
    }

    int c = ci & 0xFF;

    // ENTER
    if (c == '\n') {
        buffer[line_len] = '\0';

        // Dodaj do historii
        if (line_len > 0) {
            if (history_len < HISTORY_SIZE) {
                for (int i = 0; i <= line_len; i++)
                    history[history_len][i] = buffer[i];
                history_len++;
            } else {
                // Przesuń historię w górę
                for (int h = 1; h < HISTORY_SIZE; h++)
                    for (int i = 0; i <= LINE_BUF_SIZE; i++)
                        history[h-1][i] = history[h][i];
                for (int i = 0; i <= line_len; i++)
                    history[HISTORY_SIZE-1][i] = buffer[i];
            }
        }
        history_pos = -1;

        col = 0;
        row++;
        start_col = 0;
        line_len = 0;
        cursor_pos = 0;
        buffer[0] = '\0';
        sanitize_cursor();
    }

    // BACKSPACE
    else if (c == '\b') {
        if (cursor_pos > 0) {
            for (int i = cursor_pos - 1; i < line_len - 1; i++)
                buffer[i] = buffer[i + 1];

            cursor_pos--;
            line_len--;
            buffer[line_len] = '\0';
            sanitize_cursor();

            print_set_cursor(start_col + cursor_pos, row);
            for (int i = cursor_pos; i < line_len; i++)
                print_char(buffer[i]);
            print_char(' ');
            print_set_cursor(start_col + cursor_pos, row);
            draw_cursor(start_col, cursor_pos, row, blink_state);
        }
    }

    // LEFT ARROW
    else if (ci == KEY_LEFT) {
        cursor_pos--;
        sanitize_cursor();
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }

    // RIGHT ARROW
    else if (ci == KEY_RIGHT) {
        cursor_pos++;
        sanitize_cursor();
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }

    // UP ARROW (historia)
    else if (ci == KEY_UP) {
        if (history_len > 0) {
            if (history_pos == -1)
                history_pos = history_len - 1;
            else if (history_pos > 0)
                history_pos--;

            int old_len = line_len;

            for (int i = 0; i <= LINE_BUF_SIZE; i++)
                buffer[i] = history[history_pos][i];
            line_len = 0;
            while (buffer[line_len] != '\0') line_len++;
            cursor_pos = line_len;
            sanitize_cursor();

            print_set_cursor(start_col, row);
            for (int i = 0; i < old_len; i++) print_char(' ');
            print_set_cursor(start_col, row);
            for (int i = 0; i < line_len; i++) print_char(buffer[i]);
            print_set_cursor(start_col + cursor_pos, row);
            draw_cursor(start_col, cursor_pos, row, blink_state);
        }
    }

    // DOWN ARROW (historia)
    else if (ci == KEY_DOWN) {
        if (history_pos != -1) {
            int old_len = line_len;

            if (history_pos < history_len - 1)
                history_pos++;
            else
                history_pos = -1;

            if (history_pos == -1) {
                line_len = 0;
                cursor_pos = 0;
                buffer[0] = '\0';
            } else {
                for (int i = 0; i <= LINE_BUF_SIZE; i++)
                    buffer[i] = history[history_pos][i];
                line_len = 0;
                while (buffer[line_len] != '\0') line_len++;
                cursor_pos = line_len;
            }
            sanitize_cursor();

            print_set_cursor(start_col, row);
            for (int i = 0; i < old_len; i++) print_char(' ');
            print_set_cursor(start_col, row);
            for (int i = 0; i < line_len; i++) print_char(buffer[i]);
            print_set_cursor(start_col + cursor_pos, row);
            draw_cursor(start_col, cursor_pos, row, blink_state);
        }
    }

    // DELETE
    else if (ci == KEY_DELETE) {
        if (cursor_pos >= line_len) return;  // nic do usunięcia
        for (int i = cursor_pos; i < line_len - 1; i++)
            buffer[i] = buffer[i + 1];
        line_len--;
        buffer[line_len] = '\0';
        sanitize_cursor();

        print_set_cursor(start_col + cursor_pos, row);
        for (int i = cursor_pos; i < line_len; i++)
            print_char(buffer[i]);
        print_char(' ');
        print_set_cursor(start_col + cursor_pos, row);
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }

    // HOME
    else if (ci == KEY_HOME) {
        cursor_pos = 0;
        sanitize_cursor();
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }

    // END
    else if (ci == KEY_END) {
        cursor_pos = line_len;
        sanitize_cursor();
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }

    // TAB 
    else if (c == '\t') {
    int spaces = TAB_SIZE;
    if (line_len + spaces >= LINE_BUF_SIZE)
        spaces = LINE_BUF_SIZE - 1 - line_len;

    if (spaces <= 0) return;

    // text ->
    for (int i = line_len - 1; i >= cursor_pos; i--)
        buffer[i + spaces] = buffer[i];

    // Spaces
    for (int s = 0; s < spaces; s++)
        buffer[cursor_pos + s] = ' ';

    line_len += spaces;
    cursor_pos += spaces;
    buffer[line_len] = '\0';
    sanitize_cursor();

    // Refresh line
    print_set_cursor(start_col, row);
    for (int i = 0; i < line_len; i++)
        print_char(buffer[i]);
    print_char(' ');
    print_set_cursor(start_col + cursor_pos, row);
    draw_cursor(start_col, cursor_pos, row, blink_state);
    return;
}


// NORMAL CHARACTERS
else {     
    if (line_len < LINE_BUF_SIZE - 1) {
        for (int i = line_len; i > cursor_pos; i--)
            buffer[i] = buffer[i - 1];

        buffer[cursor_pos] = (char)c;
        line_len++;
        cursor_pos++;
        sanitize_cursor();

        print_set_cursor(start_col + cursor_pos - 1, row);
        for (int i = cursor_pos - 1; i < line_len; i++)
            print_char(buffer[i]);

        print_set_cursor(start_col + cursor_pos, row);
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }
}

    // BLINK
    blink_counter++;
    if (blink_counter > BLINK_THRESHOLD) {
        blink_counter = 0;
        blink_state = !blink_state;
        draw_cursor(start_col, cursor_pos, row, blink_state);
    }
}

// ===================== INIT =====================
// Initialize keyboard driver
void keyboard_init(void) {
    kb_head = kb_tail = 0;
    ps_index = 0;
    kb_update_leds();
    print_str("PS/2 KEYBOARD DRIVER INITIALIZED\n");
}