#pragma once
#include <stdint.h>

void timer_init(void);
void timer_tick(void);
void sleep_ms(uint64_t ms);
uint64_t timer_uptime_ms(void);

extern volatile uint64_t ticks;

typedef struct {
    uint64_t hours;
    uint64_t minutes;
    uint64_t seconds;
    uint64_t milliseconds;
} timer_time_t;

timer_time_t timer_convert_ms(uint64_t ms);
void timer_print_uptime(void);