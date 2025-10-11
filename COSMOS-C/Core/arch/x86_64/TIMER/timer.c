#include "Core/arch/x86_64/TIMER/timer.h"
#include "Core/arch/x86_64/TIMER/PIT/pit.h"
#include "Core/arch/x86_64/TIMER/callback/callback.h"
#include "HAL/console/print.h"
#include <stdint.h>

volatile uint64_t ticks = 0;

void timer_init(void) {
    pit_init(1000); // 1000 Hz = 1ms tick    
}

void timer_tick(void) {
    ticks++;
    timer_callbacks_update();
}

void sleep_ms(uint64_t ms) {
    uint64_t start = ticks;
    print_char(' ');
    while ((ticks - start) < ms) {
        __asm__ volatile("hlt");
    }
}

uint64_t timer_uptime_ms(void) {
    return ticks;
}

// ================= TIME CONVERSION =================
timer_time_t timer_convert_ms(uint64_t ms) {
    timer_time_t t;
    t.milliseconds = ms % 1000;
    uint64_t total_seconds = ms / 1000;
    t.seconds = total_seconds % 60;
    uint64_t total_minutes = total_seconds / 60;
    t.minutes = total_minutes % 60;
    t.hours = total_minutes / 60;
    return t;
}

// Prints uptime in HH:MM:SS.ms format
void timer_print_uptime(void) {
    timer_time_t t = timer_convert_ms(ticks);
    print_int(t.hours);   print_str(":");
    print_int(t.minutes); print_str(":");
    print_int(t.seconds); print_str(".");
    print_int(t.milliseconds); print_str("\n");
}