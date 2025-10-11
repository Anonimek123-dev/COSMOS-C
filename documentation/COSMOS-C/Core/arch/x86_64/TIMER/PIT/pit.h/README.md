# PIT Header (`pit.h`)

## 📄 Overview

This header file declares the interface for controlling the **Programmable Interval Timer (PIT)** on x86_64 systems.  
The PIT is used for generating periodic timer interrupts, which can drive system tick counters, delays, and other time-related operations.

---

## 🧩 Function

### `void pit_init(uint32_t freq)`

Initializes the PIT to generate interrupts at the specified frequency.

#### Parameters:

- `freq` — The desired frequency in Hertz (Hz) at which the PIT should tick.

#### Usage Example:

```c
#include "pit.h"

int main() {
    pit_init(1000); // Initialize PIT to tick every 1 ms (1000 Hz)
    return 0;
}
```
---
## ⚙️ Notes

- The PIT frequency must be within the hardware limits (typically 19.183182 MHz base divided by 16-bit divisor).
- This header does not define any global state; it only provides the function prototype.
- The actual PIT configuration is performed in `pit.c`, which interacts with the PIT I/O ports.
---