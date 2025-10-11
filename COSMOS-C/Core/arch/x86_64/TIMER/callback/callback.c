#include "callback.h"
#include "Core/arch/x86_64/TIMER/timer.h"

typedef struct {
    timer_callback_t cb;
    uint64_t target_tick;
    uint8_t active;
} callback_entry_t;

static callback_entry_t callbacks[MAX_CALLBACKS];

// add new timer
void set_timeout(timer_callback_t cb, uint64_t ms) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        if (!callbacks[i].active) {
            callbacks[i].cb = cb;
            callbacks[i].target_tick = timer_uptime_ms() + ms;
            callbacks[i].active = 1;
            break;
        }
    }
}

// checks whether any timer has reached its target
void timer_callbacks_update(void) {
    uint64_t now = timer_uptime_ms();
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        if (callbacks[i].active && now >= callbacks[i].target_tick) {
            callbacks[i].active = 0; // deactivation
            if (callbacks[i].cb) callbacks[i].cb();
        }
    }
<<<<<<< HEAD
}
=======
}
>>>>>>> 24522878a6beab94af9c35dce3e21874168045e6
