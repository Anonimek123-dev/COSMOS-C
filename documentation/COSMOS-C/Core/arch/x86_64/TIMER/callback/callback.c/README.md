# ⏱️ `callback.c` — Software Timer Callback System

### 📄 Overview
This file implements a **simple software timer and callback mechanism** for the kernel.  
It allows scheduling functions to be executed after a specified delay (similar to `setTimeout()` in JavaScript).

The system relies on the global `ticks` counter updated by the hardware timer (PIT).  
Callbacks are checked and executed from `timer_callbacks_update()` — typically called in every timer interrupt.

---

## 🧩 Data Structures

### `callback_entry_t`
Represents a single software timer entry.

```c
typedef struct {
    timer_callback_t cb;     // Pointer to callback function
    uint64_t target_tick;    // Tick count when the callback should trigger
    uint8_t active;          // Whether this entry is active (1) or free (0)
} callback_entry_t;
```

- **`cb`** — Function pointer to the user-defined callback.
- **`target_tick`** — Absolute time (in ticks) when the callback should fire.
- **`active`** — Indicates if the slot is in use.

---

## 🧠 Constants

```c
#define MAX_CALLBACKS ...
```

The maximum number of software timers that can exist simultaneously.  
Defined in the corresponding `callback.h`.

---

## ⚙️ Global Variables

```c
static callback_entry_t callbacks[MAX_CALLBACKS];
```

An array that stores all registered timers.  
Each element corresponds to one scheduled callback.

---

## 🚀 Functions

### `void set_timeout(timer_callback_t cb, uint64_t ms);`

Schedules a callback to be executed after a delay.

```c
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
```

#### **Parameters**
- `cb` — Function to call after the delay.
- `ms` — Delay duration in milliseconds.

#### **Behavior**
- Finds the first available slot in the `callbacks` array.
- Stores the callback and its target execution time (`current_time + ms`).
- Marks the entry as active.

---

### `void timer_callbacks_update(void);`

Checks all registered callbacks and executes those whose delay has expired.

```c
void timer_callbacks_update(void) {
    uint64_t now = timer_uptime_ms();
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        if (callbacks[i].active && now >= callbacks[i].target_tick) {
            callbacks[i].active = 0; // deactivate before calling
            if (callbacks[i].cb) callbacks[i].cb();
        }
    }
}
```

#### **Behavior**
- Retrieves the current system uptime in milliseconds.
- Iterates through all active callbacks.
- If the current time (`now`) is greater or equal to `target_tick`,  
  the callback is triggered and the slot is freed.

---

## 🧩 Integration

This system works closely with the **hardware timer interrupt**:
- Each timer tick increments the global `ticks` counter.
- The PIT interrupt handler calls `timer_callbacks_update()`.
- Callbacks scheduled via `set_timeout()` are executed asynchronously in kernel space.

---

## 🧠 Example Usage

```c
void say_hello(void) {
    print_str("Hello from callback!\n");
}

void kernel_init(void) {
    timer_init();
    set_timeout(say_hello, 3000); // run after 3 seconds
}
```

**Result:**
```
Hello from callback!
```
after approximately 3 seconds.

---

## ✅ Summary Table

| Function | Description |
|-----------|-------------|
| `set_timeout()` | Schedule a new delayed callback |
| `timer_callbacks_update()` | Poll and execute expired callbacks |
| `callbacks[]` | Static list of active timers |
| `timer_uptime_ms()` | Returns system uptime in milliseconds |

---

## 💡 Notes
- This implementation is **non-blocking** — it doesn’t pause execution.
- It can be used to simulate delayed tasks in the kernel without using `sleep()`.
- You may extend it to support **repeating timers** (intervals) or **callback cancellation**.

---