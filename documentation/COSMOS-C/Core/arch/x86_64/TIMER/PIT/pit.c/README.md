# ⚙️ `pit.c` — Programmable Interval Timer (PIT) Initialization

## 📄 Overview

This file implements the **Programmable Interval Timer (PIT)** initialization for the x86_64 architecture.  
The PIT generates periodic interrupts at a specified frequency, typically used for system ticks, task scheduling, and kernel timing functions.

---

## 🧠 Constants

```c
#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_FREQUENCY 1193182 // Hz
```

| Constant | Description |
|-----------|--------------|
| `PIT_CHANNEL0` | I/O port address for channel 0 data register |
| `PIT_COMMAND`  | I/O port address for PIT command register |
| `PIT_FREQUENCY` | Base PIT clock frequency (1,193,182 Hz) |

---

## 🛠️ Function: `pit_init(uint32_t frequency)`

Initializes the PIT to generate interrupts at a desired frequency.

```c
void pit_init(uint32_t frequency);
```

### 🔹 Parameters
- `frequency`: Desired output frequency in Hz (e.g., `1000` for 1 kHz or 1 ms tick).

### 🔹 Description
The function programs the PIT **Channel 0** in **Mode 3 (Square Wave Generator)** using a frequency divisor derived from the PIT’s base clock.  
It sends the correct command and divisor bytes to the appropriate I/O ports using the `outb()` function.

### 🔹 Implementation

```c
void pit_init(uint32_t frequency) {
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);
    outb(PIT_COMMAND, 0x36);            // Channel 0, low/high byte, mode 3
    outb(PIT_CHANNEL0, divisor & 0xFF); // Low byte
    outb(PIT_CHANNEL0, divisor >> 8);   // High byte
}
```

### 🧩 Formula
```
divisor = PIT_FREQUENCY / desired_frequency
```

- The PIT divides its base frequency (1,193,182 Hz) by this divisor to obtain the interrupt rate.
- Example:  
  For `frequency = 1000 Hz`,  
  `divisor = 1193182 / 1000 ≈ 1193`

---

## ⚡ Example Usage

```c
#include "pit.h"

void timer_init(void) {
    pit_init(1000); // Set PIT to 1 kHz (1 ms interval)
}
```

---

## ✅ Result

After initialization:
- The PIT generates interrupts at the configured frequency.
- The CPU’s timer interrupt handler (IRQ0) is triggered periodically.
- The kernel can use this tick rate to implement timekeeping, task scheduling, and delays.

---

## 🧠 Notes

- Mode 3 produces a stable square wave suitable for periodic system interrupts.
- PIT is backward-compatible with legacy PC architectures.
- Modern kernels may replace PIT with **APIC** or **HPET**, but PIT remains useful for early boot timing.

---