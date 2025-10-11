# 🌐 64-bit Entry Point (`main64.asm`)

## 🔹 `long_mode_start`
This is the entry point after the CPU has switched to 64-bit Long Mode.  
It prepares segment registers and jumps to the main kernel function.

```asm
global long_mode_start
extern kernel_main

section .text
bits 64
long_mode_start:
    ; Zero out all data segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call the 64-bit kernel entry point
    call kernel_main

    ; Halt the CPU after kernel_main returns
    hlt
```
- The segment registers (DS, ES, FS, GS, SS) are set to zero because in Long Mode, the flat memory model is used.
- `call kernel_main` transfers control to the main kernel logic.
- `hlt` ensures the CPU stops if `kernel_main` ever returns, as there is no OS scheduler yet.
---