#pragma once

#include <stdint.h>

// ===================== BASIC ASCII KEYS =====================
#define KEY_ESC       0x1B
#define KEY_BACKSPACE 0x08
#define KEY_TAB       0x09
#define KEY_SPACE     0x20

// ===================== CURSOR & EDIT KEYS =====================
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

// ===================== FUNCTION KEYS =====================
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

// ===================== EXTRA FUNCTION KEYS =====================
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

// ===================== MODIFIERS =====================
#define KEY_LSHIFT    0xA0
#define KEY_RSHIFT    0xA1
#define KEY_CTRL      0xA2
#define KEY_ALT       0xA4
#define KEY_CAPSLOCK  0xA6
#define KEY_NUMLOCK   0xA7
#define KEY_SCROLL    0xA8

// ===================== NUMPAD =====================
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

// ===================== SYSTEM / MEDIA KEYS =====================
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

// ===================== DRIVER API =====================
void keyboard_init(void);
int  keyboard_getchar(void);
void keyboard_irq_handler(void);
