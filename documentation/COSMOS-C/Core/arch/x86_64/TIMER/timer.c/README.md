# ⏱️ `timer.c` — System Timer and Timekeeping Module

## 📄 Overview
This file implements the **core system timer** for the x86_64 kernel.  
It provides **millisecond-based timing**, **sleep functionality**, **uptime tracking**,  
and **conversion utilities** for representing time in human-readable format.

It also integrates with the **Programmable Interval Timer (PIT)** and  
the **software callback system** (`callback.c`) for delayed function execution.

---

## 🧩 Key Responsibilities

| Feature | Description |
|----------|--------------|
| `timer_init()` | Initializes the PIT for a 1 kHz tick rate (1 ms per tick). |
| `timer_tick()` | Called by the IRQ0 handler on every timer interrupt. Increments the global tick counter and updates software callbacks. |
| `sleep_ms()` | Blocks the CPU for a given number of milliseconds using the timer tick counter. |
| `timer_uptime_ms()` | Returns the number of milliseconds since system boot. |
| `timer_convert_ms()` | Converts uptime in milliseconds into hours, minutes, seconds, and milliseconds. |
| `timer_print_uptime()` | Prints formatted uptime (HH:MM:SS.ms) to the console. |

---

## 🧠 Global Variables

```c
volatile uint64_t ticks = 0;
```
- **Description:**

    Global counter incremented by the timer interrupt every millisecond.

    Used as a base for timing and scheduling.
---
## ⚙️ Functions
### 🧩 `void timer_init(void)`
Initializes the PIT with a 1 kHz frequency — meaning one tick per millisecond.
```c
void timer_init(void) {
    pit_init(1000); // 1000 Hz = 1ms tick
}
```
- **Depends on:** `pit_init()` from `PIT/pit.c`
- **Called by:** `hardwaresetup()` in `main.c`
---
### ⚡ `void timer_tick(void)`
Called on each **IRQ0** (timer interrupt).

Updates `ticks` and executes registered software callbacks.
```c
void timer_tick(void) {
    ticks++;
    timer_callbacks_update();
}
```
**Note:**

This function should be called from the IRQ handler for IRQ0 (system timer).

---
### 😴 `void sleep_ms(uint64_t ms)`
Blocks the current CPU thread for the given number of milliseconds.

Implemented as a **busy-wait loop**, sleeping via `hlt` instructions until the desired tick count is reached.
```c
void sleep_ms(uint64_t ms) {
    uint64_t start = ticks;
    while ((ticks - start) < ms) {
        __asm__ volatile("hlt");
    }
}
```
**Caution:**

Since this halts only the current CPU core, it is not **multitasking-safe**.

Use this only before scheduler initialization.

---
### 🕒 `uint64_t timer_uptime_ms(void)`

Returns the total uptime in milliseconds since boot.
```c
uint64_t timer_uptime_ms(void) {
    return ticks;
}
```
---
### ⏳ `timer_time_t timer_convert_ms(uint64_t ms)`

Converts milliseconds into a readable time structure.
```c
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
```
- **Used for:** `timer_print_uptime()`

- **Structure** (`timer_time_t`):
```c
typedef struct {
    uint64_t hours;
    uint64_t minutes;
    uint64_t seconds;
    uint64_t milliseconds;
} timer_time_t;
```
---
### 🧾 `void timer_print_uptime(void)`

Prints the current system uptime in **HH:MM:SS.ms** format.
```c
void timer_print_uptime(void) {
    timer_time_t t = timer_convert_ms(ticks);
    print_int(t.hours);   print_str(":");
    print_int(t.minutes); print_str(":");
    print_int(t.seconds); print_str(".");
    print_int(t.milliseconds); print_str("\n");
}
```
- **Example Output:**
`0:00:42.315`

---
## 🔁 Integration
### Linked Components
| Component    | Role                                                                       |
| ------------ | -------------------------------------------------------------------------- |
| `PIT/pit.c`  | Provides low-level hardware interface for the Programmable Interval Timer. |
| `callback.c` | Manages software-level timeouts (`set_timeout()`).                         |
| `print.c`    | Provides basic console output for debugging and logging.                   |
---
## 🧩 Summary Table
| Function               | Purpose                               | Called By           |
| ---------------------- | ------------------------------------- | ------------------- |
| `timer_init()`         | Set up PIT for 1ms ticks              | `hardwaresetup()`   |
| `timer_tick()`         | Increment tick counter + callbacks    | IRQ0 handler        |
| `sleep_ms()`           | Delay execution for `ms` milliseconds | Anywhere (blocking) |
| `timer_uptime_ms()`    | Get current uptime                    | Kernel subsystems   |
| `timer_convert_ms()`   | Convert ms to human time              | Internal            |
| `timer_print_uptime()` | Print formatted uptime                | Debug, shell        |
---
## ⚠️ Notes

- The PIT runs at **1000 Hz** for 1 ms resolution.
- `sleep_ms()` **should not** be used in multitasking contexts.
- `ticks` may overflow after ~584 million years — practically safe 😄.
- This timer serves as the foundation for **task scheduling, timeouts,** and **performance measurement** in later kernel stages.
---
🧠 **Author:** COSMOS-C Kernel Team

📦 **Module:** `Core/arch/x86_64/TIMER/timer.c`

🕒 **Last Updated:** 11 October 2025

---