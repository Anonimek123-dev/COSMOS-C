# 🧭 `timer.h` — Timer Interface and Timekeeping API

## 📄 Overview
`timer.h` provides the **public API** for the kernel’s timing system,  
including time tracking, sleep delays, and uptime conversion.

It defines key **data structures**, **function declarations**, and  
interfaces to interact with the **PIT-based timer** and **software callbacks**.

---

## 🧱 Header Summary

```c
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Structure representing time components (H:M:S:ms)
typedef struct {
    uint64_t hours;
    uint64_t minutes;
    uint64_t seconds;
    uint64_t milliseconds;
} timer_time_t;

// Core timer functions
void timer_init(void);
void timer_tick(void);
void sleep_ms(uint64_t ms);
uint64_t timer_uptime_ms(void);

// Conversion and display utilities
timer_time_t timer_convert_ms(uint64_t ms);
void timer_print_uptime(void);

#endif
```

---

## 🧩 Key Data Structure

### `timer_time_t`
A utility struct used to represent uptime in a human-readable format.

```c
typedef struct {
    uint64_t hours;
    uint64_t minutes;
    uint64_t seconds;
    uint64_t milliseconds;
} timer_time_t;
```

| Field | Type | Description |
|--------|------|-------------|
| `hours` | `uint64_t` | Total elapsed hours since boot |
| `minutes` | `uint64_t` | Remaining minutes (0–59) |
| `seconds` | `uint64_t` | Remaining seconds (0–59) |
| `milliseconds` | `uint64_t` | Remaining milliseconds (0–999) |

---

## ⚙️ Function Prototypes

### 🧩 `void timer_init(void)`
Initializes the system timer hardware (PIT) to tick every 1 millisecond.

- **Called by:** `hardwaresetup()` during kernel startup  
- **Depends on:** `pit_init()` implementation in `PIT/pit.c`

---

### ⚡ `void timer_tick(void)`
Invoked by the **IRQ0 interrupt handler** on every PIT tick.  
Increments the internal `ticks` counter and triggers callback updates.

- **Should be called by:** the IRQ dispatcher when IRQ0 (Timer) occurs  
- **Typically executes:** `timer_callbacks_update()` internally

---

### 😴 `void sleep_ms(uint64_t ms)`
Blocks the CPU for the specified duration (in milliseconds).

- Uses a busy-wait loop with `hlt` for efficiency  
- **Unsafe for multitasking** — should only be used during single-core init

---

### 🕒 `uint64_t timer_uptime_ms(void)`
Returns the total number of milliseconds elapsed since system startup.

- **Useful for:** measuring delays, timeouts, profiling kernel performance

---

### ⏳ `timer_time_t timer_convert_ms(uint64_t ms)`
Converts a millisecond timestamp into a structured time format.

| Input | Output |
|--------|---------|
| `1234567 ms` | `{ hours=0, minutes=20, seconds=34, milliseconds=567 }` |

- Often used by `timer_print_uptime()` to display uptime.

---

### 🧾 `void timer_print_uptime(void)`
Prints the current system uptime to the console in **HH:MM:SS.ms** format.

**Example output:**
```
0:00:42.315
```

- Uses `print_str()` and `print_int()` from the console HAL.

---

## 🔁 Integration

| Module | Relationship |
|----------|--------------|
| `PIT/pit.c` | Hardware driver providing millisecond interrupts |
| `callback.c` | Software timer system for `set_timeout()` and periodic callbacks |
| `print.c` | Provides console printing functions used for debug output |
| `idt.c` / `irq.c` | Dispatch IRQ0 → `timer_tick()` |

---

## 🧩 Summary Table

| Function | Purpose | Context |
|-----------|----------|----------|
| `timer_init()` | Start PIT timer | Kernel init |
| `timer_tick()` | Increment tick counter | IRQ0 |
| `sleep_ms()` | Delay execution | Early init / test |
| `timer_uptime_ms()` | Get uptime in ms | Kernel subsystems |
| `timer_convert_ms()` | Convert ms to H:M:S:ms | Utility |
| `timer_print_uptime()` | Print uptime | Debugging |

---

## ⚠️ Notes

- The timer operates at **1000 Hz**, giving **1 ms precision**.
- The internal tick counter (`ticks`) is 64-bit — effectively overflow-proof.
- Sleep and uptime rely entirely on **PIT interrupts** being correctly configured.
- For multitasking systems, this module will serve as the base for the **kernel scheduler**.

---

**🧠 Author:** COSMOS-C Kernel Team  
**📦 Module:** `Core/arch/x86_64/TIMER/timer.h`  
**🕒 Last Updated:** 11 October 2025

---