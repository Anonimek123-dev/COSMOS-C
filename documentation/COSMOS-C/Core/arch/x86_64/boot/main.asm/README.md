# ⚙️ File: `main.asm`

## Overview

This file contains the **main boot code of the kernel**, executed immediately after being loaded by the bootloader (e.g., GRUB).  
It’s written in **NASM assembly (Intel syntax)** and is responsible for:

1. Verifying that the bootloader passed valid data (Multiboot2),
2. Checking if the CPU supports `CPUID` and 64-bit (Long Mode),
3. Initializing basic memory structures (page tables, GDT),
4. Switching from **32-bit protected mode** to **64-bit long mode**.

Once all checks and setup steps are complete, the code jumps (`jmp`) to `long_mode_start`, defined elsewhere (typically in `long_mode.asm` or `kernel64.asm`).

---

## 📘 Code with Comments

```asm
global start
extern long_mode_start

section .text
bits 32
start:
    ; Set up stack pointer
    mov esp, stack_top

    ; Perform checks
    call check_multiboot
    call check_cpuid
    call check_long_mode

    ; Set up memory paging and enable it
    call setup_page_tables
    call enable_paging

    ; Load 64-bit GDT and jump to long mode
    lgdt [gdt64.pointer]
    jmp gdt64.code_segment:long_mode_start

    hlt
```

## 🧪 Environment Checks

### 🔹 `check_multiboot`
Verifies whether the bootloader passed the correct Multiboot2 magic number in `EAX`.

```asm
check_multiboot:
    cmp eax, 0x36d76289        ; Multiboot2 magic number
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "M"                ; error code 'M'
    jmp error
```
If `EAX != 0x36d76289`, the kernel halts with an **“ERR: M”** message.

### 🔹 `check_cpuid`
Checks whether the CPU supports the `CPUID` instruction.

```asm
check_cpuid:
    pushfd                     ; save EFLAGS
    pop eax
    mov ecx, eax
    xor eax, 1 << 21           ; toggle ID bit
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd                      ; restore EFLAGS
    cmp eax, ecx               ; compare if bit changed
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "C"
    jmp error
```
If `CPUID` is not supported, the kernel halts with **“ERR: C”**.

### 🔹 `check_long_mode`
Verifies that the CPU supports 64-bit Long Mode.

```asm
check_long_mode:
    mov eax, 0x80000000       ; query max extended CPUID function
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode          ; if < 0x80000001, long mode not supported

    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29         ; check Long Mode bit (bit 29 of EDX)
    jz .no_long_mode          ; if not set, long mode unavailable
    ret
.no_long_mode:
    mov al, "L"               ; error code 'L'
    jmp error
```

Halts the kernel with **“ERR: L”** if Long Mode is unsupported.

## 🧱 Memory Initialization
### 🔹 `setup_page_tables`
Sets up the basic 3-level paging hierarchy for Long Mode (L4 → L3 → L2).
Uses 2 MiB pages to identity map the first 1 GiB of memory.
```asm
setup_page_tables:
    mov eax, page_table_l3
    or eax, 0b11               ; present + writable
    mov [page_table_l4], eax

    mov eax, page_table_l2
    or eax, 0b11
    mov [page_table_l3], eax

    mov ecx, 0
.loop:
    mov eax, 0x200000          ; 2 MiB
    mul ecx                     ; calculate physical address
    or eax, 0b10000011          ; present + writable + huge page
    mov [page_table_l2 + ecx*8], eax
    inc ecx
    cmp ecx, 512
    jne .loop
    ret
```

Creates **512 entries in L2 table,** mapping **1 GiB of memory** in **2 MiB pages**.

### 🔹 `enable_paging`
Activates paging, PAE, and Long Mode.
```asm
enable_paging:
    mov eax, page_table_l4
    mov cr3, eax               ; load top-level page table

    mov eax, cr4
    or eax, 1 << 5             ; enable PAE
    mov cr4, eax

    mov ecx, 0xC0000080        ; MSR_EFER
    rdmsr
    or eax, 1 << 8             ; enable Long Mode
    wrmsr

    mov eax, cr0
    or eax, 1 << 31            ; enable paging
    mov cr0, eax
    ret
```

After this, the CPU is in **64-bit paging mode,** ready to jump to **Long Mode**.

## ⚠️ Error Handling
```asm
error:
    mov dword [0xb8000], 0x4f524f45 ; "ERRO"
    mov dword [0xb8004], 0x4f3a4f52 ; "R:O"
    mov dword [0xb8008], 0x4f204f20 ; space
    mov byte  [0xb800a], al         ; error code ('M', 'C', or 'L')
    hlt
```
Uses VGA text buffer at `0xB8000` to display a simple error message and halts.

## 🧩 BSS Section
```asm
section .bss
align 4096
page_table_l4: resb 4096
page_table_l3: resb 4096
page_table_l2: resb 4096
stack_bottom:  resb 4096 * 4
stack_top:
```

Allocates memory for page tables and a 16 KiB stack.
`stack_top` is the initial stack pointer.

## 🧱 `.rodata` Section – 64-bit GDT
```asm
section .rodata
gdt64:
    dq 0                           ; null descriptor
.code_segment: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; code segment
.pointer:
    dw $ - gdt64 - 1               ; GDT size
    dq gdt64                       ; GDT address
```

Minimal Global Descriptor Table for Long Mode, with one 64-bit code segment.

## 📈 Kernel Initialization Flow
| Step | Function            | Purpose                                |
| ---- | ------------------- | -------------------------------------- |
| 1    | `check_multiboot`   | Ensure kernel loaded via Multiboot2    |
| 2    | `check_cpuid`       | Verify CPU supports CPUID              |
| 3    | `check_long_mode`   | Check for 64-bit Long Mode support     |
| 4    | `setup_page_tables` | Create initial paging tables           |
| 5    | `enable_paging`     | Enable PAE, Long Mode, and paging      |
| 6    | `lgdt` + `jmp`      | Load GDT and jump to `long_mode_start` |

---