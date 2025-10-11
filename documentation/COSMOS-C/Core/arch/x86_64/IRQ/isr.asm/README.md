# ⚙️ `isr.asm` — Interrupt Service Routine (ISR) & IRQ Assembly Stubs

## 🧩 Overview

The `isr.asm` file defines **interrupt stubs** for all **CPU exceptions (0–31)** and **hardware IRQs (32–47)**.  
Each stub performs low-level context saving, identifies which interrupt occurred, and then jumps into a **common handler** written in C (`isr_handler` in `irq.c`).

This file forms the **lowest layer of the interrupt handling mechanism**, bridging CPU hardware traps with higher-level kernel logic.

---

## 📄 Source Code

```asm
; isr.asm - explicit ISR/IRQ stubs (Intel/NASM syntax) for x86_64
; Each stub saves registers, pushes vector, then jumps to a common handler.

extern isr_handler

section .text
global isr0,isr1,isr2,isr3,isr4,isr5,isr6,isr7
global isr8,isr9,isr10,isr11,isr12,isr13,isr14,isr15
global isr16,isr17,isr18,isr19,isr20,isr21,isr22,isr23
global isr24,isr25,isr26,isr27,isr28,isr29,isr30,isr31
global irq32,irq33,irq34,irq35,irq36,irq37,irq38,irq39
global irq40,irq41,irq42,irq43,irq44,irq45,irq46,irq47
global isr_common_stub

%macro ISR_STUB 1
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rdi
    push rsi
    push rbp
    push rbx
    push rdx
    push rcx
    push rax
    push %1
    jmp isr_common_stub
%endmacro

isr0:  ISR_STUB 0
isr1:  ISR_STUB 1
isr2:  ISR_STUB 2
isr3:  ISR_STUB 3
isr4:  ISR_STUB 4
isr5:  ISR_STUB 5
isr6:  ISR_STUB 6
isr7:  ISR_STUB 7
isr8:  ISR_STUB 8
isr9:  ISR_STUB 9
isr10: ISR_STUB 10
isr11: ISR_STUB 11
isr12: ISR_STUB 12
isr13: ISR_STUB 13
isr14: ISR_STUB 14
isr15: ISR_STUB 15
isr16: ISR_STUB 16
isr17: ISR_STUB 17
isr18: ISR_STUB 18
isr19: ISR_STUB 19
isr20: ISR_STUB 20
isr21: ISR_STUB 21
isr22: ISR_STUB 22
isr23: ISR_STUB 23
isr24: ISR_STUB 24
isr25: ISR_STUB 25
isr26: ISR_STUB 26
isr27: ISR_STUB 27
isr28: ISR_STUB 28
isr29: ISR_STUB 29
isr30: ISR_STUB 30
isr31: ISR_STUB 31

irq32: ISR_STUB 32
irq33: ISR_STUB 33
irq34: ISR_STUB 34
irq35: ISR_STUB 35
irq36: ISR_STUB 36
irq37: ISR_STUB 37
irq38: ISR_STUB 38
irq39: ISR_STUB 39
irq40: ISR_STUB 40
irq41: ISR_STUB 41
irq42: ISR_STUB 42
irq43: ISR_STUB 43
irq44: ISR_STUB 44
irq45: ISR_STUB 45
irq46: ISR_STUB 46
irq47: ISR_STUB 47

; common stub: vector is on top of stack
isr_common_stub:
    mov rdi, [rsp]      ; vector -> rdi (first arg)
    call isr_handler
    add rsp, 8           ; pop vector
    pop rax
    pop rcx
    pop rdx
    pop rbx
    pop rbp
    pop rsi
    pop rdi
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    iretq
```

---
## 🧠 Functionality Breakdown
### 🧩 `%macro ISR_STUB`

This macro generates a **complete interrupt stub** for a given vector number.

It:

1. Saves all general-purpose registers.
2. Pushes the interrupt vector number onto the stack.
3. Jumps to the shared handler `isr_common_stub`.

This ensures that every interrupt type — whether CPU exception or hardware IRQ — follows a **consistent entry sequence**.

---
### 🧱 Generated Stubs

The file defines:
- **32 CPU exception stubs** (`isr0`–`isr31`)
- **16 hardware IRQ stubs** (`irq32`–`irq47`)

These functions are later referenced in `idt.c`, which installs them into the **Interrupt Descriptor Table (IDT)**.

---
🧵 `isr_common_stub`

This routine is shared by all stubs.
It retrieves the pushed interrupt vector from the stack, calls the high-level C handler, and restores the register state.

**Step-by-step:**
1. Move vector number into `rdi` (the first argument for System V ABI calls).
```asm
mov rdi, [rsp]
```
2. Call the C handler:
```asm
call isr_handler
```
3. Clean up the stack (remove vector):
```asm
add rsp, 8
```
4. Restore all registers in reverse order.
5. Return from interrupt:
```asm
iretq
```

---
## 🔄 Flow Summary

| Step           | Description                                                               |
| -------------- | ------------------------------------------------------------------------- |
| 🧠 CPU         | Interrupt occurs → CPU pushes context & jumps to the corresponding stub.  |
| ⚙️ ISR Stub    | Saves registers, pushes vector ID, jumps to `isr_common_stub`.            |
| 💡 Common Stub | Calls `isr_handler(vector)` in C.                                         |
| 🧮 C Handler   | Determines if it’s an exception or IRQ → delegates to appropriate driver. |
| 🔚 Return      | Registers restored, `iretq` executes → returns control to previous code.  |
---
## 🧩 Integration Points

| Component                      | Role                                         |
| ------------------------------ | -------------------------------------------- |
| **`IDT`**                      | Holds pointers to each ISR/IRQ stub.         |
| **`isr_handler` (in `irq.c`)** | Handles exceptions & device interrupts in C. |
| **`pic_send_eoi`**             | Acknowledges hardware IRQ completion.        |
| **`keyboard_irq_handler`**     | Example of device-specific handler for IRQ1. |
---
## ✅ Summary

| Symbol            | Purpose                                           |
| ----------------- | ------------------------------------------------- |
| `ISR_STUB`        | Macro generating register-saving interrupt stubs. |
| `isr0`–`isr31`    | CPU exception handlers.                           |
| `irq32`–`irq47`   | Hardware interrupt handlers.                      |
| `isr_common_stub` | Shared logic calling the C-level handler.         |
| `iretq`           | Returns from interrupt, restoring full context.   |
---

⚡ This file is **the foundation of the interrupt subsystem** — it ensures that all interrupts enter the kernel safely and consistently, preserving register state before delegating to higher-level logic.

---