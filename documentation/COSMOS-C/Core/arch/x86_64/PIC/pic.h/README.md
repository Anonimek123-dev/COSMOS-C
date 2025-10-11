# ⚙️ File: `pic.h` — PIC Driver Header

This header defines the interface for controlling the **8259A Programmable Interrupt Controller (PIC)**.

---

## 📄 Source Code

```c
#ifndef PIC_H
#define PIC_H

/* Remap the PIC interrupt vectors to custom offsets */
void pic_remap(int offset1, int offset2);

/* Send End Of Interrupt (EOI) to the PIC */
void pic_send_eoi(unsigned char irq);

#endif
```
---
## 🧩 Explanation

- `pic_remap(int offset1, int offset2)`
Adjusts the IRQ vector offsets to avoid conflicts with CPU exceptions.
Typically, the Master PIC is remapped to 0x20 and the Slave PIC to 0x28.

- `pic_send_eoi(unsigned char irq)`
Notifies the PIC that the IRQ specified by irq has been handled.
Ensures that the PIC can continue sending further interrupts.

- **Include Guards** (#ifndef PIC_H ... #endif)
Prevents multiple inclusions of this header file in other source files.

---
## ✅ Example Usage
```c
#include "arch/x86_64/PIC/pic.h"

void init_pic() {
    pic_remap(0x20, 0x28);  // Remap Master and Slave PIC vectors
}
```
---