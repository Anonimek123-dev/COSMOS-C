# 🧭 `callback.h` — Timer Callback Interface

## 📘 Overview
This header defines the **timer callback interface** used by the kernel timer system.  
It enables scheduling **delayed function calls** (software timers) similar to `setTimeout()` in higher-level languages.

Callbacks are handled by the timer interrupt service routine, allowing asynchronous actions to occur after a specified delay.

---

## 🧩 Constants and Macros

### `#define MAX_CALLBACKS 16`
- Defines the **maximum number of concurrent software timers** supported by the system.
- If more than 16 callbacks are registered, additional calls to `set_timeout()` will be ignored until a slot becomes free.

---

## 🧠 Typedefs

### `typedef void (*timer_callback_t)(void);`
Defines the type for callback functions used by the timer system.

Each callback function must:
- Take **no arguments**
- Return **void**

Example:
```c
void my_callback(void) {
    print_str("Timer fired!\n");
}
```

---

## ⚙️ Functions

### `void set_timeout(timer_callback_t cb, uint64_t ms);`
Registers a function to be executed **after a specified delay** (in milliseconds).

#### Parameters:
- `cb` — The callback function pointer to execute.
- `ms` — The delay time in **milliseconds** before calling `cb`.

#### Example:
```c
set_timeout(my_callback, 2000); // Executes after 2 seconds
```

---

### `void timer_callbacks_update(void);`
Checks all active timers and executes any whose delay has expired.  
This function is called automatically by the **timer interrupt handler** (`timer_tick()` in `timer.c`).

#### Notes:
- Typically, you should **not call this manually** — it runs as part of the timer interrupt loop.
- When a callback fires, it is **automatically deactivated** and its slot becomes free.

---

## 🧠 Internal Usage

The callback system is implemented in `callback.c` and relies on:
- `timer_uptime_ms()` from `timer.c` for current tick count.
- The periodic tick interrupt from the **PIT (Programmable Interval Timer)**.

---

## ✅ Example Usage

```c
#include "Core/arch/x86_64/TIMER/callback/callback.h"
#include "HAL/console/print.h"

void hello(void) {
    print_str("Hello after 3 seconds!\n");
}

void kernel_init(void) {
    set_timeout(hello, 3000);
}
```

Output:
```
Hello after 3 seconds!
```

---
**Author:** COSMOS-C Kernel Development Team  
**Purpose:** Provide lightweight, event-driven timing for kernel tasks.

---