# This is a Full Documentation in COSMOS-C. If you find any bugs, please comment this repository in Github, in COSMOS-C discord server, or send a email  `cosmos-c@gmail.com`.

## What is it used for?

- **`buildenv/` - There, we are building an environment to open our .iso system.**
- **`build/`- This is where all compiled object files (with the .o extension) end up.**
- **`COSMOS-C/` - This is where the COSMOS-C files are located. If you want to add something, etc., this is the place to do it.**
- **`dist/` - This folder will appear after compilation. There, you will find the raw system binary file (.bin) and the bootable disk image (.iso).**
- **`src/` - Put your system files there.**
- **`targets/` - There you will find the linker file (`linker.ld`), the file `grub.cfg` and the raw system binary file too.**

## Makefile
### Definition

**Makefile – is a regular text file that contains instructions telling the computer how to build a program. This file works together with the make tool, which checks what has changed in the source code and automatically runs only those commands (e.g., compilation) that are needed.**

**The following are stored in Makefile:**

- targets 
     – what we want to achieve (e.g., an executable program file),
- dependencies 
     – what this target depends on (e.g., .c and .h files),
- rules 
    – how to do it step by step (e.g., compiler commands),
- variables 
    – to make it easier to change parameters, such as the compiler name or optimization flags.

**Thanks to Makefile, you don't have to remember long commands or compile everything manually – make does it for us, faster and only where necessary.**

### Makefile structure
>**Remember, if you create new files, folders, etc., you must MANUALLY modify the Makefile!**

At the beginning, Makefile downloads the kernel C files:

```Makefile
# kernel C files
kernel_source_files := $(shell find src/ -name '*.c')
kernel_object_files := $(patsubst src/%.c, build/kernel/%.o, $(kernel_source_files))
```
Then it downloads the x86_64 architecture C files from the COSMOS-C/Core/arch/x86_64 folder:

```Makefile
# x86_64 C files
x86_64_c_source_files := $(shell find COSMOS-C/Core/arch/x86_64 -name '*.c')
x86_64_c_object_files := $(patsubst COSMOS-C/Core/arch/x86_64/%.c, build/x86_64/%.o, $ x86_64_c_source_files)
```

Then it downloads the x86_64 architecture .asm files from the COSMOS-C/Core/arch/x86_64 folder:

```Makefile
# x86_64 ASM files
x86_64_asm_source_files := $(shell find COSMOS-C/Core/arch/x86_64 -name '*.asm')
x86_64_asm_object_files := $(patsubst COSMOS-C/Core/arch/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))
```

Then it downloads the C driver files from the COSMOS-C/HAL/Drivers folder:

```Makefile
# drivers C files
drivers_c_source_files := $(shell find COSMOS-C/HAL -name '*.c')
drivers_c_object_files := $(patsubst COSMOS-C/HAL/%.c, build/x86_64/%.o, $(drivers_c_source_files))
```

Then it combines all x86_64 architecture files:

```Makefile
# all x86_64 object files
x86_64_object_files := $(x86_64_c_object_files) $(x86_64_asm_object_files)
```

Then it builds kernel files:

```Makefile
build/kernel/%.o: src/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I src/ -I COSMOS-C -I COSMOS-C/Core -I COSMOS-C/HAL -ffreestanding $< -o $@
```

Then it builds x86_64 architecture C files:

```Makefile
build/x86_64/%.o: COSMOS-C/Core/arch/x86_64/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I COSMOS-C -I COSMOS-C/Core -I COSMOS-C/HAL -ffreestanding $< -o $@
```

Then it builds x86_64 architecture .asm files:

```Makefile
build/x86_64/%.o: COSMOS-C/Core/arch/x86_64/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@
```

Then it builds the driver files:

```Makefile
build/x86_64/%.o: COSMOS-C/HAL/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I COSMOS-C -I COSMOS-C/Core -I COSMOS-C/HAL -ffreestanding $< -o $@
```

Then it responds to the user command ```make build-x86_64```:

```Makefile
.PHONY: build-x86_64
build-x86_64: $(kernel_object_files) $(x86_64_object_files) $(drivers_c_object_files)
	mkdir -p dist/x86_64
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld \
		$(kernel_object_files) $(x86_64_object_files) $(drivers_c_object_files)
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso
```
>**NOTE: Remember, you must modify in Makefile all `kernel.bin` and `kernel.iso` for your system name!**

**And that's it for Makefile.**

## .gitignore
### Definition
**.gitignore – a special text file used in the Git version control system, in which you enter the names of files and folders that are to be ignored by Git.**

**This means that everything on the list in .gitignore will not be:**

- tracked
- recorded in the change history,
- or sent to the repository.

**The most common items to ignore are:**

- temporary files (e.g., *.log, *.tmp),
- directories with compilation files (/build, /dist),
- editor local settings (.vscode/, .idea/),
- files containing private data (e.g., passwords, API keys).

**Thanks to .gitignore, the repository remains clean and contains only those files that are really needed for the project.**

### .gitignore in COSMOS-C
**The structure of the .gitignore file in COSMOS-C is very simple, and I think it needs no explanation:**
```gitignore
boot/kernel.bin
```
## linker.ld

### Definition
**`linker.ld` is a script file used by GNU Linker (`ld`) to define how data and code are laid out in an execuative file or library. In practice, it is a configuration for the linker that allows the programmer to specify where individual sections of code and data should be located (e. g., in memory), which librares to use, and how to link object files to create the final program.**

### What is a linker?

- **A linker is a program that combines previosly compiled modules (object files) and librares into a**
**single execuatle file or shared library.**
- **This is a one of the stages of program creation, following compilation, which translates source code** **into machine language.**
- **In UNIX systems, the `.ld` program, which is a part of the GNU Binutils project, is often used.**

### The role of the linker.ld file
- **A file with the `.ld` (or similar) extention is a configuration file or linker script that contains instructions for the `.ld` program (e.g., text code, data, stack) in memory.** 
- **The programmer can specify which librares are to be included in the final file and which are to remain as dynamic librares to be loaded during program execution.**

### Why is needed?
**`linker.ld` is used when more complex control over the linking process is needed, beyond the default linker settings. It is particularly useful when developing embedded systems or when manual memory managment is required.**

### linker.ld in COSMOS-C
**The structure of the linker.ld file in COSMOS-C is very simple, and I think it needs no explanation:**
```ld
ENTRY(start)

SECTIONS
{
    . = 1M;

    .boot :
    {
        KEEP(*(.multiboot_header))
    }

    .text :
    {
        *(.text)
    }
}
```

## grub.cfg
### Definition
`grub.cfg` – the main configuration file for the **GRUB 2 boot loader**.
It specifies how GRUB should behave when the computer starts up, i.e.:
- **the wait time** (how many seconds GRUB should wait for the system to be selected),
- **the default entry** (which system should start automatically),
- **menuentry** – entries in the boot menu, each of which describes an operating system or kernel that can be booted,
- **boot commands** – e.g., multiboot2 (for a multiboot2-compatible kernel), linux (for Linux), initrd (for initramfs), or boot (start command).
**Thanks to grub.cfg, the user can boot different operating systems or different configurations of the same system.**

---
### grub.cfg in COSMOS-C
**Now let's see what grub.cfg looks like:**
```cfg
set timeout=0
set default=0

menuentry "DiabloOS" {
    multiboot2 /boot/kernel.bin
    boot
}
```
**🔎 Explanation of the example:**
- **`set timeout=0` → GRUB does not display the menu, it immediately boots the default system.**
- **`set default=0` → sets the first entry in the menu as the default.**
- **`menuentry “DiabloOS”` → creates a menu item called “DiabloOS”.**
- **`multiboot2 /boot/kernel.bin` → points to your system's kernel file, which GRUB is to boot in multiboot2 mode.**
- **`boot` → starts loading.**

## Dockerfile
### Definition
A **Dockerfile** is a plain text file that acts as a **recipe for building a development environment**.

**You can think of it as step-by-step instructions that specify:**
1. **Which base system to use** (e.g., Linux, Ubuntu).
2. **Which programs or tools to install** (e.g., compiler, assembler, build utilities).
3. **How to configure** the working directory for the project.
4. **How to set up the environment** so that it can be reproduced anywhere.

**With Dockerfile:**
- **You don’t need to manually install tools** on your computer.
- **Everyone** using the same **Dockerfile** gets **the exact same environment.**
- **It eliminates** the classic **“works on my machine” problem.**
---

### Dockerfile in COSMOS-C
**Here is the Dockerfile in COSMOS-C:**
```dockerfile
FROM randomdude/gcc-cross-x86_64-elf

RUN apt-get update 
RUN apt-get upgrade -y
RUN apt-get install -y nasm
RUN apt-get install -y xorriso
RUN apt-get install -y grub-pc-bin
RUN apt-get install -y grub-common

VOLUME /root/env
WORKDIR /root/env
```

## main.c
### Definition
The **`main.c`** file is usually the **starting point of a C program**.  
It contains the (in COSMOS-C)`kernel_main()` function, which is the first function executed when a program starts running.

In the context of an **operating system kernel**, `main.c` often serves as the **entry point for initializing the system**.  
Here, instead of being a normal application, it may:
- Initialize hardware drivers (keyboard, screen, etc.).
- Set up memory management.
- Start the kernel loop that keeps the OS running.

---
### main.c in COSMOS-C
>**NOTE: Remember that main.c is already a file in your system, so you can modify it however you want (just like everything else), but I recommend leaving the functions and includes that are already there.**

**Let's now discuss in detail everything that is contained in the main.c file.**

---
**First, let's include all the necessary files from COSMOS-C:**
```c
#include "arch/x86_64/IDT/idt.h"
#include "arch/x86_64/PIC/pic.h"
#include "Drivers/PS2/keyboard/ps2.h"
#include "HAL/console/print.h"
#include <stdint.h>
```
1. **Include the idt.h file.**
2. **Include the pic.h file.**
3. **Include the PS/2 keyboard driver.**
4. **Include the file with the print_str() functions, etc.**
5. **Let's include stdint.h**
>**NOTE: We will discuss all of the files mentioned later.**

**Let's move on!**

---
**Now let's define all the functions we need in our kernel:**
```c
void kernel_init(void);
void hardwaresetup(void);
void kernel_update(void);
void enable_irq(void);
void kb_update(void);
```
1. **`void kernel_init(void);`**
- **Purpose: main initialization function of the operating system (kernel).**

- **What it does:**
	- Called immediately after the kernel starts (e.g., by kernel_main or after taking control from the bootloader).
	- Initializes basic data structures, drivers, and the console.
	- Can run basic tests, set stack pointers, memory, etc.

💡 You can think of it as the **“system power switch.”**

2. **`void hardwaresetup(void);`**
- **Purpose: hardware configuration.**

- **What it does:**
	- Sets up input/output devices (keyboard, timer, ports).
	- Prepares hardware interrupt handling (e.g., from PS/2 keyboard, PIT timer).
	- Initializes interrupt tables (IDT, GDT if not already ready).

💡 It's like a **“hardware checklist”** – the kernel needs to know how to communicate with the hardware.

3. **`void kernel_update(void);`**
- **Purpose: main kernel update loop.**

- **What it does:**
	- Called every frame.
	- Checks user input.
	- Can run background processes, handle events, respond to interrupts.

💡Refreshes **the system status** on an ongoing basis.

4. **`void enable_irq(void);`**
- **Purpose: to enable hardware interrupts (IRQ).**

- **What it does:**
	- On x86 architecture, it sets the IF (Interrupt Flag) in the RFLAGS register using the sti (Set Interrupts) instruction.
	- This allows the processor to start handling interrupts from devices (e.g., pressing a key on the keyboard).

💡 It's like **"unlocking a phone"** – only then does the system start responding to its environment.

5. **`void kb_update(void);`**
- **Purpose: updates the keyboard status.**

- **What it does:**
	- Checks if there are new characters in the input buffer.
	- Handles arrows, backspace, delete, enter, etc.
	- Draws the cursor (flashing).
	- Transfers the entered data to the console or command interpreter.

💡 This is **the brain of the keyboard** – without it, the system knows nothing about the keys pressed.

---
**✅ Summary in simple terms:**
- `kernel_init()` → system startup.
- `hardwaresetup()` → hardware preparation.
- `kernel_update()` → continuous system operation.
- `enable_irq()` → enabling interrupt response.
- `kb_update()` → keyboard and cursor handling.

---
**Now let's see what happens in the `kernel_main()` function, which executes immediately after the kernel is loaded:**
```c
// Main kernel function
void kernel_main() {
    kernel_init();   // Init
    hardwaresetup(); // set IDT, remap PIC, init keyboard, enable interrupts    
    while (1) {
        kernel_update();
    }
}
```
1. **Load the `kernel_init()` function, which contains information about kernel startup**
2. **Load the `hardware_setup()` function, which configures interrupts and devices**
3. **Load the main kernel loop (`kernel_update()`)**

**And that's it for `kernel_main()` function.**

---

**Now let's see what happens in `kernel_init()`!**
```c
// Init
void kernel_init() {    
    print_clear();
    print_set_color(LIGHT_GREEN, BLACK);    
    print_str("COSMOS-C booted successfully!\n");
}
```
**As you can see, the `kernel_main()` function is the first function loaded in the kernel, but this function immediately loads the `kernel_init()` function, which is where the kernel actually starts. In the `kernel_init()` function:**
1. First, we clean the screen.
2. Then, using the `print_set_color()` function, we set the text color to light green and the background color to black.
3. Finally, it prints **COSMOS-C booted successfully!** on the screen.

---
**Let's move on to the next function: `hardware_setup()`.**
```c
// Called once to set up interrupts + devices
void hardwaresetup(void) {    
    idt_init();            // 1) initialize IDT (sets up interrupt gates)    
    pic_remap(0x20, 0x28); // 2) remap PIC so IRQs 0..15 map to vectors 0x20..0x2F   
    keyboard_init();       // 3) initialize keyboard driver (buffers, state)    
    enable_irq();          // 4) enable interrupts globally
}
```
**Function `hardwaresetup()` is called to configure interrupts and devices. In this function:**
1. **`idt_init()` initialize IDT (sets up interrupt gates).**
2. **`pic_remap(0x20, 0x28)` remap PIC so IRQs 0..15 map to vectors 0x20..0x2F.**
3. **`keyboard_init();` initialize PS/2 keyboard driver.**
4. **`enable_irq()` enable interrupts globally.**

---
**We are left with the last kernel function, its main loop: `kernel_update()`, which runs every frame.**
```c
void kernel_update(void) {
    kb_update();
}
```
**And for now, we are only executing the `kb_update()` function.**

**And that's it for `main.c`.**

## And what can we find in the COSMOS-C folder itself?
- **HAL**
- **Core**

### Let's start with the `Core` folder!

---
**In the `Core` folder, you will find the `arch` folder, which contains files for different architectures (currently, COSMOS-C only supports one architecture). So, as you might guess, the `arch` folder contains an `x86_64` folder, which contains files for the x86 architecture.**

---
### `x86_64` folder

---
**In the x86_64 folder, you will find the following folders:**
- **boot**
- **IDT**
- **IRQ**
- **PIC**

---
### `boot` folder

---
**In the `boot` folder, you will find .asm files that load before the kernel immediately after booting by the bootloader.**

---

## header.asm
**The `header.asm` file contains the assembly header.**
```asm
section .multiboot_header
header_start:
    ; magic number
    dd 0xe85250d6 ; multiboot2
    ; architecture
    dd 0 ; protected mode i386
    ; header length
    dd header_end - header_start
    ; checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; end tag
    dw 0
    dw 0
    dd 8
header_end:
```
- **`section .multiboot_header`** 

The `multiboot_header` section defines **the Multiboot2 header** required by the bootloader (e.g., GRUB) to correctly identify and load the kernel. This is **the minimum header compliant with the Multiboot2 specification** [Multiboot2 specification](https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html).

- **`header_start:`** and **`header_end:`**

These are the tags that **start the assembly header**.

- **`dd 0xe85250d6`**

```asm
; magic number
    dd 0xe85250d6 ; multiboot2
```

**Magic number** – identifies the header as Multiboot2. The bootloader searches for this value in the first 32768 bytes of the kernel.

- **`dd 0`**

```asm
; architecture
    dd 0 ; protected mode i386
```

**Architecture** – indicates the processor architecture:
- `0` = i386 (protected mode)
- `4` = MIPS

- **`dd header_end - header_start`**

```asm
; header length
    dd header_end - header_start
```

**Header length** – the length of the entire header (in bytes), from `header_start` to `header_end`.

- **`dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))`**

```asm
; checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))
```

**Checksum** – the sum of all 32-bit fields in the header must be `0` (mod 2³²).

- **`dw 0 dw 0 dd 8`**

```asm
; end tag
    dw 0
    dw 0
    dd 8
```

**End tag** – indicates that this is the end of the header. Each Multiboot2 header must end with a tag of type `0` and length `8`.

---
## ⚙️ File: `main.asm`

### Overview

This file contains the **main boot code of the kernel**, executed immediately after being loaded by the bootloader (e.g., GRUB).  
It’s written in **NASM assembly (Intel syntax)** and is responsible for:

1. Verifying that the bootloader passed valid data (Multiboot2),
2. Checking if the CPU supports `CPUID` and 64-bit (Long Mode),
3. Initializing basic memory structures (page tables, GDT),
4. Switching from **32-bit protected mode** to **64-bit long mode**.

Once all checks and setup steps are complete, the code jumps (`jmp`) to `long_mode_start`, defined elsewhere (typically in `long_mode.asm` or `kernel64.asm`).

---

### 📘 Code with Comments

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

### 🧪 Environment Checks

#### 🔹 `check_multiboot`
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

#### 🔹 `check_cpuid`
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

#### 🔹 `check_long_mode`
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

### 🧱 Memory Initialization
#### 🔹 `setup_page_tables`
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

#### 🔹 `enable_paging`
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

### ⚠️ Error Handling
```asm
error:
    mov dword [0xb8000], 0x4f524f45 ; "ERRO"
    mov dword [0xb8004], 0x4f3a4f52 ; "R:O"
    mov dword [0xb8008], 0x4f204f20 ; space
    mov byte  [0xb800a], al         ; error code ('M', 'C', or 'L')
    hlt
```
Uses VGA text buffer at `0xB8000` to display a simple error message and halts.

### 🧩 BSS Section
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

### 🧱 `.rodata` Section – 64-bit GDT
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

### 📈 Kernel Initialization Flow
| Step | Function            | Purpose                                |
| ---- | ------------------- | -------------------------------------- |
| 1    | `check_multiboot`   | Ensure kernel loaded via Multiboot2    |
| 2    | `check_cpuid`       | Verify CPU supports CPUID              |
| 3    | `check_long_mode`   | Check for 64-bit Long Mode support     |
| 4    | `setup_page_tables` | Create initial paging tables           |
| 5    | `enable_paging`     | Enable PAE, Long Mode, and paging      |
| 6    | `lgdt` + `jmp`      | Load GDT and jump to `long_mode_start` |

---

## 🌐 64-bit Entry Point (`main64.asm`)

### 🔹 `long_mode_start`
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

## 🧩 IDT Folder Documentation

### 📁 Folder Overview

The **`IDT/`** folder in the kernel source contains the implementation of the  
**Interrupt Descriptor Table (IDT)** — a crucial structure that tells the CPU  
how to handle exceptions, traps, and hardware interrupts.

In COSMOS-C, this folder contains **two files**:

IDT/

- **idt.c** - Implementation of IDT setup and management

- **idt.h** - Header file defining IDT structures and declarations

Let's move on!

---
## ⚙️ `idt.c` — Interrupt Descriptor Table Implementation

### 📄 Overview

This file defines the **Interrupt Descriptor Table (IDT)** initialization logic for the kernel’s **x86_64 architecture**.  
The IDT maps CPU exceptions and hardware interrupts to specific handler routines (ISRs and IRQs).

The implementation sets up the IDT structure, populates each vector, and loads it into the CPU using the `lidt` instruction.

---

### 🧠 Key Structures
---
#### `struct idt_entry`

Defines a single **64-bit IDT entry**.  
Each entry describes the address and attributes of one interrupt handler.

```c
struct idt_entry {
    uint16_t offset_low;   // Lower 16 bits of handler address
    uint16_t selector;     // Code segment selector (usually kernel segment)
    uint8_t  ist;          // Interrupt Stack Table index
    uint8_t  type_attr;    // Type and attributes (e.g., 0x8E = present, interrupt gate)
    uint16_t offset_mid;   // Middle 16 bits of handler address
    uint32_t offset_high;  // Upper 32 bits of handler address
    uint32_t zero;         // Reserved (must be 0)
} __attribute__((packed));
```
---
#### `struct idt_ptr`

**The IDT pointer** structure passed to **the CPU** with the `lidt` instruction.

```c
struct idt_ptr {
    uint16_t limit;  // Size of IDT - 1
    uint64_t base;   // Base address of the IDT
} __attribute__((packed));
```

---
## 🧩 Constants and Globals

```c
#define KERNEL_CS 0x08
```

- `KERNEL_CS` → Segment selector for the kernel code segment (defined in GDT). Two static global variables are used:

```c
static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;
```

---
### 🛠️ Function: `set_idt_gate`

Configures a single IDT entry with a handler address and flags.

```c
void set_idt_gate(int n, uint64_t handler, uint8_t flags) {
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = KERNEL_CS;
    idt[n].ist         = 0;
    idt[n].type_attr   = flags;
    idt[n].offset_mid  = (handler >> 16) & 0xFFFF;
    idt[n].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].zero        = 0;
}
```

- `n` → Interrupt vector (0–255)
- `handler` → Address of ISR/IRQ handler
- `flags` → Attributes (e.g., `0x8E` = present, interrupt gate, ring 0)

---
### ⚡ ISR and IRQ Declarations
The file declares **external handler stubs** implemented in **assembly** (isr.asm):

- **ISRs (0–31)** — CPU exceptions
- **IRQs (32–47)** — Hardware interrupts (e.g., timer, keyboard)

```c
/* Extern stubs defined in isr.asm (we will create them explicitly there) */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq32();
extern void irq33();
extern void irq34();
extern void irq35();
extern void irq36();
extern void irq37();
extern void irq38();
extern void irq39();
extern void irq40();
extern void irq41();
extern void irq42();
extern void irq43();
extern void irq44();
extern void irq45();
extern void irq46();
extern void irq47();
```

These stubs will later call a common interrupt handler in C.

---
### 🚀 Function: `idt_init()`

Initializes and loads the IDT into the CPU.

```c
void idt_init(void) {
    // 1. Zero out the IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        set_idt_gate(i, 0, 0);
    }

    // 2. Define the standard interrupt gate attributes
    const uint8_t int_gate = 0x8E; // Present, ring 0, interrupt gate

    // 3. Register CPU exception handlers (vectors 0–31)
    set_idt_gate(0,  (uint64_t)isr0,  int_gate);
    set_idt_gate(1,  (uint64_t)isr1,  int_gate);
    /* ... */
    set_idt_gate(31, (uint64_t)isr31, int_gate);

    // 4. Register hardware IRQ handlers (vectors 32–47)
    set_idt_gate(32, (uint64_t)irq32, int_gate);
    /* ... */
    set_idt_gate(47, (uint64_t)irq47, int_gate);

    // 5. Load the IDT into the CPU
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint64_t)&idt;
    __asm__ volatile ("lidt %0" : : "m"(idtp));
}
```
### 🧩 Summary Table

| Step | Description                                         |
| ---- | --------------------------------------------------- |
| 1️⃣  | Zero out all IDT entries                            |
| 2️⃣  | Set up ISR handlers for CPU exceptions (0–31)       |
| 3️⃣  | Set up IRQ handlers for hardware interrupts (32–47) |
| 4️⃣  | Fill in the IDT pointer structure                   |
| 5️⃣  | Load IDT with the `lidt` instruction                |
---

### 🧠 Notes

- `0x8E` flag means:
    - Bit 7: Present
    - Bits 6–5: Descriptor privilege level (0 = kernel)
    - Bit 4: 0 (always)
    - Bits 3–0: Type (1110 = interrupt gate)
- The IDT must be reloaded after switching to long mode.
- In isr.asm, each ISR should save registers, call a C-level handler, and restore state.
---

### ✅ Result

After `idt_init()`:

- The CPU knows where to jump when an interrupt or exception occurs.
- System exceptions (like divide-by-zero or page fault) are properly handled.
- Hardware interrupts (like keyboard, timer, etc.) are routed safely to their handlers.

**💡 The IDT is the CPU’s “phonebook” for interrupts —** each entry connects a unique interrupt number to the function that handles it.

---
## 🧩 `idt.h` — Interrupt Descriptor Table (Header)

### 📄 Overview

This header file defines the **interface** for the Interrupt Descriptor Table (IDT) module used by the x86_64 kernel.  
It provides essential constants and function prototypes that allow other parts of the kernel to configure and initialize the IDT.

---

### 🧠 Purpose

The **IDT (Interrupt Descriptor Table)** maps interrupt and exception vectors to their respective handler functions.  
Every interrupt (e.g., divide-by-zero, keyboard input, system timer) has a unique vector number, and the IDT defines where control should jump when that interrupt occurs.

---

### 📦 Header Contents

```c
#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_ENTRIES 256

void idt_init(void);
void set_idt_gate(int n, uint64_t handler, uint8_t flags);

#endif
```
---
### ⚙️ Macro Definitions

```c
#define IDT_ENTRIES 256
```
Defines the total number of IDT entries — **256** possible interrupt vectors (0–255). This includes:

- **0–31** → CPU exceptions
- **32–47** → Hardware IRQs
- **48–255** → User-defined or system interrupts (e.g., syscalls)

---
### 🧩 Function Declarations
```c
void idt_init(void);
```
Initializes the **Interrupt Descriptor Table** and loads it into the CPU using the `lidt` instruction.
This function is typically called early during kernel initialization (after entering long mode).

**Responsibilities:**

- Clears all IDT entries
- Registers exception handlers (ISRs 0–31)
- Registers hardware interrupt handlers (IRQs 32–47)
- Loads the IDT pointer into the CPU

---
```c
void set_idt_gate(int n, uint64_t handler, uint8_t flags);
```
Configures a single IDT entry.

| Name      | Type       | Description                                                            |
| --------- | ---------- | ---------------------------------------------------------------------- |
| `n`       | `int`      | Interrupt vector index (0–255)                                         |
| `handler` | `uint64_t` | Address of the handler function (ISR or IRQ stub)                      |
| `flags`   | `uint8_t`  | Descriptor attributes (e.g., `0x8E` = present, ring 0, interrupt gate) |

**Usage Example:**
```c
set_idt_gate(32, (uint64_t)irq0_handler, 0x8E);
```

---
### 🧠 Usage Notes
- Include this header in any kernel source file that interacts with the interrupt subsystem:
```c
#include "arch/x86_64/IDT/idt.h"
```
- The actual implementation of these functions is located in `idt.c`.
- Must be called **after** the Global Descriptor Table (GDT) and long mode are set up.

### ✅ Summary

| Component            | Description                                      |
| -------------------- | ------------------------------------------------ |
| **`IDT_ENTRIES`**    | Number of interrupt vectors (256 total)          |
| **`set_idt_gate()`** | Assigns a handler to a specific interrupt vector |
| **`idt_init()`**     | Builds and loads the entire IDT                  |

>**💡 The IDT is a critical kernel structure that ensures proper handling of both CPU and hardware interrupts.**
---
## ⚡ `IRQ/` — Interrupt Request Handling Module

### 📁 Folder Overview

The **`IRQ/`** directory contains all source files responsible for **handling hardware interrupts (IRQs)** in the operating system kernel.  
It works together with the **IDT (Interrupt Descriptor Table)** and **PIC (Programmable Interrupt Controller)** subsystems to route and manage interrupt signals from external hardware devices such as the keyboard, timer, and disk controllers.

---

### 🧠 Purpose

In x86 architecture, **hardware devices** signal the CPU using **IRQ lines (Interrupt Requests)**.  
Each IRQ corresponds to an interrupt vector in the **IDT**, starting at vector **32** (after remapping the PIC).

For example:

| Device | IRQ Line | IDT Vector |
|---------|-----------|------------|
| System Timer (PIT) | 0 | 32 |
| Keyboard | 1 | 33 |
| Cascade (Slave PIC) | 2 | 34 |
| COM2 / COM4 | 3 | 35 |
| COM1 / COM3 | 4 | 36 |
| Floppy Disk | 6 | 38 |
| Real-Time Clock | 8 | 40 |

---

### ⚙️ Key Responsibilities

The IRQ subsystem manages:
1. **Remapping the PIC** — to move IRQs away from CPU exceptions (0–31 → reserved by CPU).
2. **Setting up IDT entries (vectors 32–47)** — linking them to low-level interrupt service routines (ISRs).
3. **Dispatching interrupts** — calling the correct device driver or handler function.
4. **Acknowledging interrupts** — sending End Of Interrupt (EOI) signals to the PIC after handling.
5. **Enabling/disabling IRQs** dynamically — depending on device state or kernel mode.

---
### 🔄 Flow of a Hardware Interrupt
Device → PIC → CPU → IDT Entry → ISR Stub (isr.asm)

1. Hardware device triggers an IRQ.
2. PIC sends an interrupt signal to the CPU.
3. CPU jumps to the corresponding **IDT entry (32–47)**.
4. ISR stub (from `isr.asm`) calls the C-level special function.
5. The IRQ subsystem executes the appropriate handler function.
6. The interrupt is acknowledged with **EOI** to the PIC.

---
### 🧩 Integration with Other Subsystems
| Subsystem       | Role                                           |
| --------------- | ---------------------------------------------- |
| **IDT**         | Maps IRQ vectors to ISR stubs.                 |
| **PIC**         | Handles low-level routing and acknowledgement. |
| **PS/2 Driver** | Uses IRQ1 (keyboard).                          |
| **PIT Driver**  | Uses IRQ0 (system timer).                      |
---
### ✅ Summary
| Component   | Purpose                                                  |
| ----------- | -------------------------------------------------------- |
| `irq.c`     | Core IRQ handling logic (initialization, dispatch, EOI). |
| `irq.h`     | Declarations and constants for IRQ system.               |
| IRQ vectors | 32–47 (mapped from PIC hardware lines).                  |
| Integration | Works with IDT + PIC to manage hardware interrupts.      |

⚙️ The `IRQ` module forms the bridge between **hardware devices** and the **kernel**, allowing real-time responses to external events like keyboard input, timers, and system calls.

---
## ⚡ `irq.c` — Core Interrupt Request (IRQ) Handling Logic

### 🧩 Overview

The `irq.c` file provides the **core logic for handling interrupts** (both CPU exceptions and hardware IRQs).  
It acts as a high-level C interface for the **low-level interrupt stubs** written in assembly (`isr.S`), handling routing, acknowledgment, and delegation to specific device drivers.

---

### 📄 Source Code

```c
#include <stdint.h>
#include "HAL/console/print.h"
#include "arch/x86_64/PIC/pic.h"
#include "Drivers/PS2/keyboard/ps2.h"

// Forward declare PIC send EOI and keyboard handler
extern void pic_send_eoi(unsigned char irq);
extern void keyboard_irq_handler(void);

// C handler called from assembly with vector in rdi
void isr_handler(uint64_t vector) {
    if (vector <= 31) {
        // CPU exception
        print_str("Exception vector: ");
        print_int((int)vector);
        print_str("\n");
        // For now: just return (or halt) - in dev you might halt
        // __asm__ volatile ("cli; hlt");
        return;
    }

    if (vector >= 32 && vector <= 47) {
        unsigned char irq = (unsigned char)(vector - 32);
        // dispatch common IRQs here
        switch (irq) {
            case 1: // keyboard
                keyboard_irq_handler();
                break;
            default:
                // other IRQs we don't handle yet
                break;
        }

        // send EOI to PICs
        pic_send_eoi(irq);
        return;
    }

    // unexpected vector
    print_str("Unhandled vector: ");
    print_int((int)vector);
    print_str("\n");
}

// enable interrupts (wrapper)
void enable_irq(void) {
    __asm__ volatile ("sti");
}
```

### 🧠 Functionality Breakdown
#### 🪫 `isr_handler(uint64_t vector)`

This is the **central interrupt handler** called from the low-level assembly ISR stubs.
It receives the **interrupt vector number** (passed via the `rdi` register) and decides how to process it.

---
- **Exception Handling** (**vectors** `0–31`)

If the vector number is ≤ 31, the interrupt corresponds to a **CPU exception** (like divide-by-zero, page fault, etc.).
The handler prints out a message showing which exception occurred.
```c
if (vector <= 31) {
    print_str("Exception vector: ");
    print_int((int)vector);
    print_str("\n");
}
```
*(In the future, this could be extended to show detailed CPU state, stack info, or panic screen.)*

---
- **⚙️ Hardware IRQ Handling** (**vectors** `32–47`)

When the vector number is between 32 and 47, it corresponds to a **hardware interrupt (IRQ)**, typically routed through the `PIC`.

The IRQ number is obtained by subtracting 32:
```c
unsigned char irq = (unsigned char)(vector - 32);
```
Then a switch statement dispatches the interrupt to the correct device driver:
```c
switch (irq) {
    case 1: // Keyboard IRQ
        keyboard_irq_handler();
        break;
    default:
        // Other IRQs not yet handled
        break;
}
```
Finally, the interrupt must be acknowledged by sending an **End Of Interrupt (EOI)** signal to the **PIC**:
```c
pic_send_eoi(irq);
```
This tells the PIC that the CPU is done handling the interrupt, allowing new interrupts to be processed.

---
- ⚠️ Unexpected Vectors

If a vector number falls outside the expected range (0–47), the handler logs a message:
```c
print_str("Unhandled vector: ");
print_int((int)vector);
print_str("\n");
```

---
#### 🧷 `enable_irq(void)`

A small utility wrapper to **enable hardware interrupts** globally by setting the **IF (Interrupt Flag)** in the CPU’s `EFLAGS` register.
```c
__asm__ volatile ("sti");
```
This instruction allows the CPU to begin accepting interrupts after initialization.

---
### 🔄 Typical Flow

1. A device (e.g., keyboard) triggers a hardware interrupt (e.g., IRQ1).
2. The PIC forwards the signal to the CPU.
3. The CPU jumps to the correct **IDT entry** (mapped to an ISR stub in assembly).
4. The ISR stub calls the C function `isr_handler(vector)`, passing the interrupt vector.
5. The handler:
    - Distinguishes between CPU exceptions and hardware IRQs.
    - Calls the correct driver (e.g., `keyboard_irq_handler()`).
    - Sends an EOI signal to the PIC via `pic_send_eoi(irq)`.

---
### ⚙️ Integration

| Subsystem           | Role                                                |
| ------------------- | --------------------------------------------------- |
| **IDT**             | Maps interrupt vectors (0–255) to handlers.         |
| **PIC**             | Routes hardware IRQs (0–15) to IDT vectors (32–47). |
| **Keyboard Driver** | Uses IRQ1 for input events.                         |
| **Console**         | Used for logging interrupt messages.                |
---

### ✅ Summary

| Function                 | Description                                                              |
| ------------------------ | ------------------------------------------------------------------------ |
| `isr_handler()`          | Central interrupt handler — dispatches CPU exceptions and hardware IRQs. |
| `enable_irq()`           | Enables hardware interrupts globally (`sti`).                            |
| `pic_send_eoi()`         | Acknowledges handled IRQs to the PIC.                                    |
| `keyboard_irq_handler()` | Device-specific IRQ handler for PS/2 keyboard.                           |
---

🧩 The `irq.c` module serves as the **bridge between the low-level interrupt hardware and the kernel’s device drivers**, providing a clean and extensible interrupt handling interface.

---
## ⚙️ `isr.asm` — Interrupt Service Routine (ISR) & IRQ Assembly Stubs

### 🧩 Overview

The `isr.asm` file defines **interrupt stubs** for all **CPU exceptions (0–31)** and **hardware IRQs (32–47)**.  
Each stub performs low-level context saving, identifies which interrupt occurred, and then jumps into a **common handler** written in C (`isr_handler` in `irq.c`).

This file forms the **lowest layer of the interrupt handling mechanism**, bridging CPU hardware traps with higher-level kernel logic.

---

### 📄 Source Code

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
### 🧠 Functionality Breakdown
#### 🧩 `%macro ISR_STUB`

This macro generates a **complete interrupt stub** for a given vector number.

It:

1. Saves all general-purpose registers.
2. Pushes the interrupt vector number onto the stack.
3. Jumps to the shared handler `isr_common_stub`.

This ensures that every interrupt type — whether CPU exception or hardware IRQ — follows a **consistent entry sequence**.

---
#### 🧱 Generated Stubs

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
### 🔄 Flow Summary

| Step           | Description                                                               |
| -------------- | ------------------------------------------------------------------------- |
| 🧠 CPU         | Interrupt occurs → CPU pushes context & jumps to the corresponding stub.  |
| ⚙️ ISR Stub    | Saves registers, pushes vector ID, jumps to `isr_common_stub`.            |
| 💡 Common Stub | Calls `isr_handler(vector)` in C.                                         |
| 🧮 C Handler   | Determines if it’s an exception or IRQ → delegates to appropriate driver. |
| 🔚 Return      | Registers restored, `iretq` executes → returns control to previous code.  |
---
### 🧩 Integration Points

| Component                      | Role                                         |
| ------------------------------ | -------------------------------------------- |
| **`IDT`**                      | Holds pointers to each ISR/IRQ stub.         |
| **`isr_handler` (in `irq.c`)** | Handles exceptions & device interrupts in C. |
| **`pic_send_eoi`**             | Acknowledges hardware IRQ completion.        |
| **`keyboard_irq_handler`**     | Example of device-specific handler for IRQ1. |
---
### ✅ Summary

| Symbol            | Purpose                                           |
| ----------------- | ------------------------------------------------- |
| `ISR_STUB`        | Macro generating register-saving interrupt stubs. |
| `isr0`–`isr31`    | CPU exception handlers.                           |
| `irq32`–`irq47`   | Hardware interrupt handlers.                      |
| `isr_common_stub` | Shared logic calling the C-level handler.         |
| `iretq`           | Returns from interrupt, restoring full context.   |
---

⚡ This file is **the foundation of the interrupt subsystem** — it ensures that all interrupts enter the kernel safely and consistently, preserving register state before delegating to higher-level logic.

## 🧠 `isr.h` — Interrupt Service Routine Header

### 📄 Overview
The **`isr.h`** header provides the **C interface** for the interrupt service routines (ISRs) used by the kernel.  
It declares the central **`isr_handler()`** function — the bridge between low-level assembly stubs (in `isr.asm`) and higher-level C interrupt handling logic (in `irq.c`).

---

### 🧩 Source Code

```c
#ifndef ISR_H
#define ISR_H

#include <stdint.h>

/**
 * @brief Central interrupt handler called from assembly.
 *
 * This function is invoked by `isr_common_stub` (defined in `isr.asm`)
 * whenever a CPU exception or hardware interrupt occurs.
 *
 * @param vector The interrupt vector number (0–255) pushed by the ISR stub.
 *
 * Vectors 0–31 → CPU exceptions (e.g. divide-by-zero, page fault)
 * Vectors 32–47 → Hardware IRQs (e.g. keyboard, timer)
 * Above 47 → Reserved / future use
 */
void isr_handler(uint64_t vector);

#endif
```
---
### 🧠 Purpose

This header ensures that:

- **Assembly and C layers are linked properly**, by declaring the `isr_handler()` symbol.
- The kernel’s interrupt system has a **common entry point** for all ISRs and IRQs.
- The handler can distinguish between **CPU exceptions and hardware interrupts** based on the vector number.
---
### ⚙️ Flow Summary
| Layer              | Function                      | Description                                    |
| ------------------ | ----------------------------- | ---------------------------------------------- |
| 🧩 `isr.asm`       | `isr_common_stub`             | Saves registers, calls `isr_handler(vector)`   |
| 🧠 `isr_handler()` | (C function)                  | Determines interrupt type and dispatches logic |
| 🖥️ Device Drivers | e.g. `keyboard_irq_handler()` | Responds to specific IRQ events                |
---
### 🧾 Notes

- The handler follows **System V AMD64 ABI**: the vector number is passed in `rdi`.
- It must **not block or sleep** — it runs at interrupt level.
- Future enhancements might include a **register dump structure** or **error code handling**.
---
### ✅ Summary
| Symbol                         | Purpose                                                      |
| ------------------------------ | ------------------------------------------------------------ |
| `isr_handler(uint64_t vector)` | Main C entry point for all interrupts.                       |
| `<stdint.h>`                   | Provides `uint64_t` for consistent 64-bit parameter passing. |
| Include Guard                  | Prevents multiple header inclusions.                         |
---

⚡**In short:**

`isr.h` defines the **link between the assembly-level interrupt stubs and the C-level kernel logic**, forming the backbone of interrupt handling in the OS.

---
## ⚙️ `port.h` — Low-Level I/O Port Access

### 📄 Overview
The **`port.h`** header provides a set of **inline assembly functions** that enable direct communication with hardware devices via **I/O ports** on x86/x86_64 architectures.

These functions are essential for:
- Configuring and controlling hardware (e.g., PIC, PS/2, PIT)
- Reading and writing device registers
- Implementing drivers at the bare-metal level (keyboard, timer, etc.)

---

### 🧩 Source Code

```c
#ifndef PORT_H
#define PORT_H

#include <stdint.h>

/**
 * @brief Read a byte from an I/O port.
 * 
 * @param port Port address to read from.
 * @return The byte value read from the port.
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * @brief Write a byte to an I/O port.
 * 
 * @param port Port address to write to.
 * @param val Byte value to write.
 */
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/**
 * @brief Read a word (16 bits) from an I/O port.
 * 
 * @param port Port address to read from.
 * @return The 16-bit word value read from the port.
 */
static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * @brief Write a word (16 bits) to an I/O port.
 * 
 * @param port Port address to write to.
 * @param val 16-bit word value to write.
 */
static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

#endif
```
---
### 🧠 Explanation
These functions act as **wrappers for CPU I/O instructions:**
| Function | Assembly Instruction | Description                                               |
| -------- | -------------------- | --------------------------------------------------------- |
| `inb()`  | `inb %dx, %al`       | Reads a byte from the port specified in `%dx` into `%al`. |
| `outb()` | `outb %al, %dx`      | Writes a byte from `%al` to the port in `%dx`.            |
| `inw()`  | `inw %dx, %ax`       | Reads a 16-bit word from the port into `%ax`.             |
| `outw()` | `outw %ax, %dx`      | Writes a 16-bit word from `%ax` to the port.              |

Each function uses inline **GCC assembly** with:
- `"a"` → accumulator register (`AL`/`AX`)
- `"Nd"` → allows constants or DX register as port operand
---
### 🧰 Typical Use Cases
| Subsystem            | Example Use                                           |
| -------------------- | ----------------------------------------------------- |
| 🖥️ **PIC**          | Send end-of-interrupt (`outb(0x20, 0x20)`)            |
| ⌨️ **PS/2 Keyboard** | Read keycode from port `0x60`                         |
| ⏱️ **PIT Timer**     | Configure frequency via ports `0x40–0x43`             |
| 🧱 **CMOS / RTC**    | Access real-time clock registers at ports `0x70–0x71` |
---
### ⚠️ Important Notes
- I/O ports are **architecture-specific** (x86 only).
- Using the wrong port can cause **undefined hardware behavior** or a **triple fault**.
- These functions are marked `static inline` to ensure:
    - No call overhead (inlined by the compiler)
    - Availability across multiple C files without linker conflicts.
---
### ✅ Summary
| Function | Type   | Bits   | Purpose             |
| -------- | ------ | ------ | ------------------- |
| `inb`    | Input  | 8-bit  | Read byte from port |
| `outb`   | Output | 8-bit  | Write byte to port  |
| `inw`    | Input  | 16-bit | Read word from port |
| `outw`   | Output | 16-bit | Write word to port  |
---
🧩 **In short:**

`port.h` provides **direct CPU-level I/O control**, forming the foundation for all hardware interaction in your OS kernel.

---
## 📁 Folder: `PIC` — Programmable Interrupt Controller (Intel 8259)

The **`PIC` folder** contains the low-level implementation for handling **hardware interrupt routing** via the **Intel 8259 Programmable Interrupt Controller**, which is still used in x86-compatible systems (even when APIC is available for backward compatibility).

This module is essential for your **kernel’s interrupt subsystem** — it ensures that hardware-generated IRQs (like keyboard or timer interrupts) are correctly **mapped, delivered, and acknowledged**.

---


---

### 🧠 Purpose

The **8259 PIC** is responsible for:
- Handling **hardware interrupt requests (IRQs)**.
- **Prioritizing** multiple interrupt lines.
- **Sending interrupt vectors** to the CPU.
- **Acknowledging (EOI)** interrupts when the handler finishes.
- Allowing **masking/unmasking** of specific interrupts.

In protected mode kernels, the PICs are typically **remapped** because their default interrupt vectors overlap with **CPU exceptions (0x00–0x1F)**.

---

### ⚙️ Core Functions

#### `void pic_init(void);`
Initializes the master and slave PICs by:
- Remapping their interrupt vectors (`0x20–0x2F`).
- Masking all IRQ lines to avoid spurious interrupts at boot.

---

#### `void pic_remap(int offset1, int offset2);`
Changes the interrupt vector offsets:
- **Master PIC** → `offset1` (usually `0x20`)
- **Slave PIC** → `offset2` (usually `0x28`)

This prevents overlap with CPU exceptions.

Example mapping after remap:

| IRQ | Device         | Vector |
|------|----------------|---------|
| 0    | Timer          | 0x20 |
| 1    | Keyboard       | 0x21 |
| 8–15 | Slave devices  | 0x28–0x2F |

---

#### `void pic_send_eoi(unsigned char irq);`
Sends an **End Of Interrupt (EOI)** signal to the PIC(s) after handling an interrupt.  
Without sending EOI, the PIC will not deliver further interrupts of the same line.

- For IRQs ≥ 8, an EOI is sent to **both the Slave and Master**.
- For IRQs < 8, EOI is sent only to **the Master**.

---

### 🧩 How It Fits Into the Kernel

The PIC interacts closely with:
- **IDT (Interrupt Descriptor Table):** where the interrupt vectors are registered.
- **ISR/IRQ handlers:** which respond to interrupts.
- **Devices (e.g., PS/2 keyboard):** which generate IRQs.

Example flow for a keyboard interrupt:
[Keyboard press]
↓
PIC (IRQ1)
↓
CPU interrupt vector 0x21
↓
IDT entry → ISR handler
↓
keyboard_irq_handler()
↓
pic_send_eoi(1)

---
### 🔧 Implementation Summary

| File | Purpose |
|------|----------|
| `pic.h` | Defines constants and declares the PIC interface. |
| `pic.c` | Implements remapping, initialization, and EOI sending. |

---

### 🧰 Technical Notes

- The **PIC1 (Master)** handles IRQ0–7 through ports `0x20–0x21`.
- The **PIC2 (Slave)** handles IRQ8–15 through ports `0xA0–0xA1`.
- Both use **I/O ports** to configure and send commands.
- The **`outb()`** and **`inb()`** functions (from `port.h`) are used for communication.

---
### ✅ Summary
| Component          | Description                                    |
| ------------------ | ---------------------------------------------- |
| **PIC1 (Master)**  | Handles IRQ0–7                                 |
| **PIC2 (Slave)**   | Handles IRQ8–15                                |
| **pic_init()**     | Initializes and remaps both PICs               |
| **pic_remap()**    | Sets new interrupt vector offsets              |
| **pic_send_eoi()** | Sends acknowledgment after IRQ is handled      |
| **Dependencies**   | `port.h`, `isr.c`, `idt.c`, and device drivers |
---
**🧩 In essence:**
The `PIC` folder provides the **core interrupt routing system for your kernel**.
It remaps, initializes, and controls how hardware IRQs reach the CPU, ensuring your interrupt system works safely and predictably.

---
## ⚙️ File: `pic.c` — Basic 8259 PIC (Programmable Interrupt Controller) Driver

This file provides **low-level control** over the Intel **8259A PIC**, used to manage **hardware interrupts (IRQs)** on legacy x86 systems.  
It implements **remapping** of interrupt vectors and **EOI (End Of Interrupt)** signaling.

---

### 📄 Source Code

```c
#include "arch/x86_64/PIC/pic.h" 
#include "arch/x86_64/IRQ/port.h"

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1

/* ------------------------------------------------------------
   Remap the PIC Interrupt Vectors
   ------------------------------------------------------------ */
void pic_remap(int offset1, int offset2) {
    uint8_t a1 = inb(PIC1_DATA);  // Save current masks
    uint8_t a2 = inb(PIC2_DATA);

    /* Start initialization sequence for both PICs */
    outb(PIC1_CMD, 0x11);  // ICW1: start init, expect ICW4
    outb(PIC2_CMD, 0x11);

    /* Set vector offsets */
    outb(PIC1_DATA, offset1);  // ICW2: Master PIC vector offset
    outb(PIC2_DATA, offset2);  // ICW2: Slave PIC vector offset

    /* Setup cascade configuration */
    outb(PIC1_DATA, 4);  // ICW3: Tell Master that Slave is at IRQ2 (bitmask 0000 0100)
    outb(PIC2_DATA, 2);  // ICW3: Tell Slave its cascade ID (0000 0010)

    /* Set environment info */
    outb(PIC1_DATA, 0x01);  // ICW4: 8086/88 (MCS-80/85) mode
    outb(PIC2_DATA, 0x01);

    /* Restore previously saved masks */
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

/* ------------------------------------------------------------
   Send End of Interrupt (EOI) Signal
   ------------------------------------------------------------ */
void pic_send_eoi(unsigned char irq) {
    if (irq >= 8)
        outb(PIC2_CMD, 0x20);  // Acknowledge Slave PIC first
    outb(PIC1_CMD, 0x20);      // Always acknowledge Master PIC
}
```
---
### 🧩 Explanation
1. **🧠`pic_remap(int offset1, int offset2)`**
By default, the **8259 PIC** maps hardware IRQs to interrupt vectors **0x08–0x0F**, which **overlap with CPU exceptions**.
To avoid this conflict, the OS remaps them to **0x20–0x2F** (32–47).

**Steps performed:**
1. Save existing IRQ masks.
2. Send initialization commands (`0x11`) to both PICs.
3. Set new offsets for interrupt vectors.
4. Configure the **cascade connection** between the Master and Slave PICs.
5. Set 8086 mode.
6. Restore original masks.
---
2. **🔁`pic_send_eoi(unsigned char irq)`**

Once an IRQ is handled, the CPU must **send an End Of Interrupt (EOI)** command to inform the PIC that the interrupt has been serviced.

- If the interrupt came from the **Slave PIC** (`irq >= 8`), send EOI to both **Slave** and **Master**.
- Otherwise, send EOI only to **Master**.
---
### 🧱 Summary
| Function         | Purpose                                                                |
| ---------------- | ---------------------------------------------------------------------- |
| `pic_remap()`    | Changes interrupt vector mappings to avoid overlap with CPU exceptions |
| `pic_send_eoi()` | Sends acknowledgment to PIC after IRQ handling                         |
| **Ports Used**   | `0x20–0x21` (Master), `0xA0–0xA1` (Slave)                              |
---
✅ Usage Example (Initialization in Kernel):
```c
#include "arch/x86_64/PIC/pic.h"

void kernel_init() {
    pic_remap(0x20, 0x28);  // Move PIC interrupts to 0x20–0x2F
}
```
---
## ⚙️ File: `pic.h` — PIC Driver Header

This header defines the interface for controlling the **8259A Programmable Interrupt Controller (PIC)**.

---

### 📄 Source Code

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
### 🧩 Explanation

- `pic_remap(int offset1, int offset2)`
Adjusts the IRQ vector offsets to avoid conflicts with CPU exceptions.
Typically, the Master PIC is remapped to 0x20 and the Slave PIC to 0x28.

- `pic_send_eoi(unsigned char irq)`
Notifies the PIC that the IRQ specified by irq has been handled.
Ensures that the PIC can continue sending further interrupts.

- **Include Guards** (#ifndef PIC_H ... #endif)
Prevents multiple inclusions of this header file in other source files.

---
### ✅ Example Usage
```c
#include "arch/x86_64/PIC/pic.h"

void init_pic() {
    pic_remap(0x20, 0x28);  // Remap Master and Slave PIC vectors
}
```
---
## 🏗️ Folder: `HAL` — Hardware Abstraction Layer

The **HAL (Hardware Abstraction Layer)** folder provides a set of platform-independent interfaces for interacting with hardware.  
It acts as a bridge between low-level hardware drivers and higher-level OS components, allowing the kernel to operate without worrying about hardware-specific details.

---
### 🧩 Purpose

1. **Encapsulation**  
   HAL abstracts direct hardware access, so kernel code doesn’t need to deal with port I/O or memory-mapped registers directly.

2. **Portability**  
   By using HAL functions, OS code can be reused across different hardware platforms or architectures with minimal changes.

3. **Consistency**  
   Provides unified interfaces for common operations like printing to the console, reading keyboard input, or handling timers.

---

### ⚡ Examples of HAL Modules

- **`console/print.c` and `print.h`**  
  Functions to write strings, characters, and numbers to the VGA text buffer.

- **`keyboard/ps2.c` and `ps2.h`**  
  Abstraction for PS/2 keyboard input handling (sometimes under `Drivers` or `HAL` depending on design).

---

### 📝 Summary

The `HAL` folder provides **clean, hardware-independent APIs** so that the rest of the OS can function without dealing with the quirks of each piece of hardware.  
It’s an essential layer for building a maintainable and modular OS kernel.

---
## 🖥️ Folder: `HAL/console`

The **`console`** folder inside the HAL (Hardware Abstraction Layer) contains the code responsible for **outputting text to the screen**.  
It abstracts the VGA text mode and provides simple printing functions for the kernel.

---

## 📂 Structure

- **`print.c`** — Implements the actual logic to write characters, strings, and numbers to the VGA text buffer.
- **`print.h`** — Header file that exposes the functions for use by other parts of the kernel.

---

## 🧩 Purpose

1. **Hardware Abstraction**  
   The console module hides the details of writing to memory-mapped VGA buffers, allowing the kernel to print text without directly managing hardware.

2. **Debugging and Logging**  
   Provides functions to display messages, errors, and debug information on the screen during early boot and runtime.

3. **Simple Output API**  
   Common functions include:
   - `print_str(const char* str)` — Print a string to the screen.
   - `print_char(char c)` — Print a single character.
   - `print_int(int num)` — Print an integer.
   - Additional helpers for formatting or newline handling.

---

### ⚡ Example Use

```c
#include "HAL/console/print.h"

print_str("Kernel initialized.\n");
print_int(42);
This prints "Kernel initialized." followed by the number 42 to the VGA console.

```
---
### 📝 Summary
The HAL/console folder provides a simple and consistent API for text output, essential for kernel debugging and runtime logging before more advanced I/O systems (like framebuffer graphics) are initialized.

---
## 🖥️ `print.c` — VGA Text Mode Console Implementation

This file provides the **implementation of the console printing functions** used by the kernel. It writes characters directly to the **VGA text buffer** at memory address `0xB8000`.

---

### 📌 Key Definitions

```c
const static size_t NUM_COLS = 80;  // screen width
const static size_t NUM_ROWS = 25;  // screen height

struct Char {
    uint8_t character; // ASCII character
    uint8_t color;     // foreground + background color
};

struct Char* buffer = (struct Char*) 0xb8000; // VGA buffer
size_t col = 0;   // current column
size_t row = 0;   // current row
uint8_t color = WHITE | BLACK << 4; // default text color
```

- `struct Char` represents a single cell in VGA text mode.
- `buffer` points to the memory-mapped VGA text buffer.
- `col` and `row` track the current cursor position.
- `color` stores the current foreground and background colors.

---
### 🧩 Core Functions
#### `print_clear()`

Clears the screen by zeroing out all rows and resets the cursor.

```c
void print_clear() {
    for (size_t r = 0; r < NUM_ROWS; r++) clear_row(r);
    col = 0;
    row = 0;
    print_update_cursor();
}
```
---
#### `print_char(char character)`
Prints a single character, handling special cases like:

- `'\n'` — newline
- `'\b'` — backspace
- Line wrapping when `col >= NUM_COLS`

```c
buffer[col + NUM_COLS * row] = (struct Char){ .character = (uint8_t)character, .color = color };
col++;
print_update_cursor();
```
---
#### `print_str(char* str)`
Prints a null-terminated string by calling `print_char()` repeatedly.
```c
for (size_t i = 0;; i++) {
    if (str[i] == '\0') return;
    print_char(str[i]);
}
```
---
#### `print_int(int integer)`
Prints an integer in decimal, supporting negative numbers.
```c
char buf[12]; // buffer for digits
// convert integer to string and print in reverse
```
---
#### Cursor Handling
- `print_set_cursor(size_t col_, size_t row_)` — Updates the hardware cursor on screen using ports `0x3D4` and `0x3D5`.
- `print_update_cursor()` — Calls `print_set_cursor()` to update the cursor after printing.
- `draw_cursor()` — Optional helper for positioning the cursor in text UI elements.
---
### Color Handling
```c
void print_set_color(uint8_t fg, uint8_t bg) {
    color = fg | (bg << 4);
}
```
- `fg` = foreground color
- `bg` = background color
- Combines both into a single byte for VGA text mode.
---
### 📌 Miscellaneous

- **Scrolling**: When printing beyond the last row, all rows are shifted up and the last row is cleared.
- **Direct memory access**: Writes directly to `0xB8000`, no BIOS calls are used.
- **Cursor updates**: Always synced with `col` and `row`.
---
### 📝 Summary
`print.c` provides **low-level console output** for the kernel, including:

- Character and string printing
- Integer printing
- Cursor management
- Color control
- Scrolling

It serves as a **debug and output layer** before any higher-level driver or framebuffer is initialized.

---
## 🛠️ `HAL/Drivers` — Hardware Abstraction Layer Drivers

The `Drivers` folder inside the HAL contains **device-specific drivers** that allow the kernel to interface with hardware components in a uniform way. These drivers provide the **low-level functionality** needed to control and interact with hardware, abstracting away the details of registers, ports, and protocols.

---

### 📁 Typical Structure

- **PS2/**: Contains drivers for the PS/2 keyboard and mouse, including input handling and IRQ integration.
- **Other drivers** can be added here for devices like timers, storage controllers, or PCI peripherals.

---

### 🔹 Purpose

1. **Hardware abstraction**: Provides a clean API for the kernel to interact with devices without needing to manipulate ports or registers directly.
2. **IRQ integration**: Drivers often register themselves to the interrupt system for asynchronous input handling.
3. **Encapsulation**: Keeps device-specific code separated from core kernel logic for maintainability and portability.

---

### 🔹 Example

- `HAL/Drivers/PS2/keyboard/ps2.c` interacts with the keyboard controller and reports key presses.
- It uses **port I/O** (`inb`/`outb`) and sends **EOI signals** to the PIC via HAL wrappers.
- The keyboard driver registers a callback to the IRQ handling system, allowing key presses to be handled asynchronously.

---

### 🔹 Key Concepts

- **Polling vs IRQ**: Some drivers can use polling (looping to check device status), but modern OSes prefer **interrupt-driven drivers**.
- **Port I/O vs Memory-Mapped I/O**: Drivers use either direct port access (`inb`/`outb`) or memory-mapped registers depending on the hardware.
- **Callbacks/Handlers**: Drivers often provide handlers that the IRQ subsystem calls when hardware events occur.

---

### 📝 Summary

The `HAL/Drivers` folder is **essential for modular hardware support**. It:

- Encapsulates low-level hardware interaction
- Integrates with the IRQ system
- Provides clean APIs for higher-level kernel code
- Prepares the OS to support multiple types of devices efficiently
---
## ⌨️ `Drivers/PS2` — PS/2  Drivers

The `PS2` folder inside `HAL/Drivers` contains **drivers for PS/2 input devices**, primarily keyboards (and potentially mice). These drivers allow the kernel to **read key presses, interpret scancodes**, and integrate input with the IRQ system.

---

### 📁 Typical Structure

|── PS2/
├── keyboard/
│ ├── ps2.c
│ └── ps2.h

- **keyboard/** — Handles PS/2 keyboard input.
---

### 🔹 Purpose

1. **Device initialization**: Sets up the PS/2 controller and keyboard to start sending scancodes.
2. **Interrupt handling**: Connects the keyboard IRQ to the kernel’s IRQ system via PIC.
3. **Scancode translation**: Converts raw scancodes into meaningful key events.
4. **Abstraction**: Provides a clean API to higher-level input subsystems.

---
## 🔹 Key Concepts

- **IRQ-driven input**: Avoids busy-wait polling; the CPU only handles key presses when an interrupt occurs.
- **Scancodes**: Raw codes from the keyboard which must be mapped to characters.
- **Controller commands**: PS/2 devices are controlled using specific command bytes sent to the controller via I/O ports.

---

## 📝 Summary

`HAL/Drivers/PS2` provides **essential input functionality** for the kernel:

- Initializes PS/2 devices
- Reads and processes scancodes
- Integrates with the kernel's IRQ system
- Provides a clean API for keyboard input handling

This folder forms the foundation for any **text-based interface** in the OS, like a kernel console.

---
## ⌨️ `HAL/Drivers/PS2/keyboard` — PS/2 Keyboard Driver

The `keyboard` folder inside `HAL/Drivers/PS2` contains the **source and header files** for handling PS/2 keyboard input. This driver is responsible for **reading keystrokes from the keyboard**, decoding scancodes, and delivering key events to the kernel.

---

### 📁 Typical Structure

└── keyboard/
    ├── ps2.c
    └── ps2.h

- **ps2.c** — Implementation of the PS/2 keyboard driver.
- **ps2.h** — Header file defining the keyboard driver API.
---
### 🔹 Purpose

1. **Initialize the PS/2 keyboard**:
   - Sets the keyboard controller to a known state.
   - Enables IRQ1 for keyboard input.
2. **Interrupt-driven input**:
   - Uses the **IRQ system** to handle key presses asynchronously.
   - Reduces CPU usage compared to polling.
3. **Scancode processing**:
   - Reads raw scancodes from the keyboard via I/O ports.
   - Converts scancodes into key codes or characters.
4. **Input abstraction**:
   - Provides higher-level functions to read key presses in the kernel.
   - Interfaces with the kernel console or other input consumers.
---
### 🔹 Key Functions (from `ps2.c`)

- **keyboard_irq_handler()** — Handles IRQ1 when a key is pressed.
- **ps2_init()** — Initializes the keyboard controller and enables interrupts.
- **read_scancode()** — Reads raw scancodes from the keyboard controller.
- **process_scancode()** — Converts scancodes to ASCII characters or special key codes.
---
### 🔹 Concepts

- **IRQ1**: Standard IRQ for the PS/2 keyboard.
- **Port I/O**: Uses `inb`/`outb` functions to communicate with the keyboard controller.
- **Scancodes**: Each key press/release generates a unique scancode, which the driver translates.
- **Interrupt-driven input**: Ensures the CPU responds only when a key is pressed.
---
### 📝 Summary

The `PS2/keyboard` folder implements the **low-level keyboard driver** for the kernel. It ensures:

- Reliable input through interrupts
- Accurate scancode-to-character translation
- Seamless integration with the kernel’s console or input system

It is a critical component for enabling **interactive text input** in the operating system.

---
## ⌨️ `ps2.c` — Detailed PS/2 Keyboard Driver Overview

This file implements a complete **PS/2 keyboard driver** for an x86_64 kernel. It handles **scancode translation**, **modifier and lock key tracking**, **LED control**, **input buffering**, and **interactive console input management**. Below is a detailed breakdown of every component and its functionality.

---
### 1️⃣ Constants & Ports

```c
#define PS2_DATA_PORT 0x60
#define PS2_CMD_PORT  0x64
#define KB_BUF_SIZE 256
```
- **PS2_DATA_PORT (0x60)**: Used to read scancodes sent by the keyboard.
- **PS2_CMD_PORT (0x64)**: Used to send commands to the keyboard controller.
- **KB_BUF_SIZE**: Size of the circular buffer for storing key presses (256 bytes).
```c
#define LINE_BUF_SIZE 128
#define HISTORY_SIZE 16
#define TAB_SIZE 4
#define BLINK_THRESHOLD 20000
```
- **LINE_BUF_SIZE**: Maximum characters per input line.
- **HISTORY_SIZE**: Maximum stored lines for history navigation.
- **TAB_SIZE**: Number of spaces to insert for a tab.
- **BLINK_THRESHOLD**: Number of "ticks" before toggling the cursor blink state.
---
### 2️⃣ Modifier & Lock States

The driver tracks the state of modifier and lock keys:

```c
static bool shift_pressed = false;
static bool ctrl_pressed  = false;
static bool alt_pressed   = false;
static bool capslock_on   = false;
static bool numlock_on    = true;
static bool scrolllock_on = false;
static bool extended      = false;
static bool pause_seq     = false;
```
- **shift_pressed**: Tracks whether either Shift key is pressed.
- **ctrl_pressed**: Tracks Control key.
- **alt_pressed**: Tracks Alt key.
- **capslock_on / numlock_on / scrolllock_on**: Reflect the lock keys’ state.
- **extended**: True when an `0xE0` prefix is received (extended key).
- **pause_seq**: True when beginning a multi-byte Pause key sequence (`0xE1`).
---
### 3️⃣ Circular Input Buffer
The keyboard driver uses a circular buffer for non-blocking input:

```c
static uint8_t kb_buf[KB_BUF_SIZE];
static volatile int kb_head = 0;
static volatile int kb_tail = 0;
```
- `kb_put(c)`: Adds a key code to the buffer, skipping if full.
- `kb_get()`: Retrieves the next key code from the buffer or returns -1 if empty.
- `keyboard_getchar()`: Public API for fetching the next key press.

**Why circular buffer?**

It allows **interrupt-driven input** without losing keys and supports asynchronous processing in the kernel.

---
### 4️⃣ LED Control
```c
static void kb_update_leds(void);
```
- Sends **0xED command** to the keyboard to set LED status.
- Constructs a bitmask:
```txt
bit 0: ScrollLock
bit 1: NumLock
bit 2: CapsLock
```
- Updates the keyboard LEDs whenever lock keys change.
---
### 5️⃣ Scancode Translation Tables
Two tables convert scancodes to ASCII:

```c
static unsigned char scancode2ascii[128];
static unsigned char scancode2ascii_shift[128];
```
- scancode2ascii: Normal ASCII characters without Shift.
- scancode2ascii_shift: ASCII characters when Shift is active.
- Extended keys and special keys are handled separately.
---
### 6️⃣ Scancode Processing
```c
static char translate_scancode(uint8_t code);
```
- Handles **Pause** (`0xE1`) and **PrintScreen** sequences.
- Detects **key release** (`code & 0x80`).
- Updates modifier states for Shift, Ctrl, Alt.
- Updates lock states (CapsLock, NumLock, ScrollLock) and triggers LED updates.
- Converts **normal key presses** into ASCII using `scancode2ascii` or `scancode2ascii_shift`.
---
### 7️⃣ Extended Key Handling
```c
static void handle_extended(uint8_t code, bool released);
```
- Processes extended key sequences (`0xE0` prefix).
- Maps keys like **arrow keys, Home/End, Insert/Delete, Media keys,** and **Windows keys**.
- Only handles key presses (not releases).
```c
static bool handle_printscreen(uint8_t code);
```
- Detects multi-byte **PrintScreen sequences**.
- Converts them to a special key (`KEY_PRTSCR`) in the buffer.
---
### 8️⃣ IRQ Handler
```c
void keyboard_irq_handler(void);
```
- Triggered by **IRQ1** (keyboard interrupt).
- Reads the scancode from port `0x60`.
- Converts it to ASCII (or a special key) using `translate_scancode`.
- Adds the result to the circular buffer (`kb_put`).
---
### 9️⃣ Console Update / Line Editing
```c
void kb_update(void);
```
This function **manages interactive editing**:

- Fetches characters from the buffer (`keyboard_getchar()`).
- Handles:
    - **Enter**: finishes line, updates history.
    - **Backspace/Delete**: removes character.
    - **Arrow keys**: moves cursor; navigates history.
    - **Home/End**: jumps to start/end of line.
    - **Tab**: inserts spaces.
- Maintains a **history buffer** of the last 16 lines.
- Tracks cursor position, blinking, and updates the screen with `print_char` and `draw_cursor`.
- Cursor blink is implemented via `blink_counter` and `BLINK_THRESHOLD`.

**Internal State Variables:**
```c
static char buffer[LINE_BUF_SIZE];   // current input line
static int line_len;                 // current line length
static int cursor_pos;               // cursor position
static int start_col;                // column offset
static char history[HISTORY_SIZE][LINE_BUF_SIZE];
static int history_len;              // total number of history lines
static int history_pos;              // current position in history
static int blink_state;              // cursor on/off
static int blink_counter;            // blink timer
```
---
### 🔟 Initialization
```c
void keyboard_init(void);
```
- Resets buffer and state variables.
- Resets Pause sequence and LED status.
- Calls `kb_update_leds()` to reflect lock states.
- Prints `"PS/2 KEYBOARD DRIVER INITIALIZED\n"` to the console.
---
### 1️⃣1️⃣ Summary of Functionality

1. **Low-Level Input Handling:**
    - Reads scancodes directly from PS/2 ports.
    - Processes both standard and extended keys.
2. **Modifier & Lock Management:**
    - Shift, Ctrl, Alt, CapsLock, NumLock, ScrollLock.
    - Updates LED indicators.
3. **Circular Buffering:**
    - Stores key presses asynchronously.
    - Prevents lost keys during kernel processing.
4. **Interactive Console Support:**
    - Cursor movement and blinking.
    - Line editing (insert, delete, backspace, tab, home/end).
    - Command history navigation.
5. **Interrupt-Based Operation:**
    - IRQ1 triggers handler for responsive input.
6. **Scancode to ASCII Conversion:**
    - Handles normal keys, shifted keys, numpad keys, function keys, and media keys.
---
#### 🔹 Integration Notes
- Requires `port.h` for I/O operations.
- Depends on `HAL/console/print.h` for displaying characters.
- Works with the kernel console (`row` and `col` variables) for proper cursor positioning.
- Can be extended to support additional keys or custom macros.
---
This driver provides **complete PS/2 keyboard support** suitable for low-level kernel development, supporting a responsive, interactive console environment.

---
## 🧩 `ps2.h` — Full Technical Documentation

This file defines the **PS/2 keyboard driver interface** and provides **symbolic key codes** for use across the kernel.  
It’s located in:  
`HAL/Drivers/PS2/keyboard/ps2.h`

---
### 🧱 1. Include Guard
```c
#pragma once
```
Prevents the file from being included multiple times during compilation,
avoiding duplicate symbol definitions.

---
### 📚 2. Includes
```c
#include <stdint.h>
```
Includes fixed-width integer types (e.g., **uint8_t**, **uint16_t**),
ensuring consistent type usage across low-level hardware code.

---
### 🔤 3. Basic ASCII Keys
```c
#define KEY_ESC       0x1B
#define KEY_BACKSPACE 0x08
#define KEY_TAB       0x09
#define KEY_SPACE     0x20
```
These correspond directly to standard **ASCII codes**.
They represent printable control keys that can be displayed or interpreted directly in text output.

---
### 🧭 4. Cursor & Editing Keys
```c
#define KEY_UP        0x80
#define KEY_DOWN      0x81
#define KEY_LEFT      0x82
#define KEY_RIGHT     0x83
#define KEY_DELETE    0x7F
#define KEY_INSERT    0x86
#define KEY_HOME      0x84
#define KEY_END       0x85
#define KEY_PGUP      0x87
#define KEY_PGDN      0x88
```
- All have values ≥ `0x80` to avoid clashing with ASCII.
- These represent **non-printable control keys**.
- Used by terminal, shell, and GUI subsystems for navigation or text editing.
---
### ⚙️ 5. Function Keys (F1–F12)
```c
#define KEY_F1        0x90
#define KEY_F2        0x91
#define KEY_F3        0x92
#define KEY_F4        0x93
#define KEY_F5        0x94
#define KEY_F6        0x95
#define KEY_F7        0x96
#define KEY_F8        0x97
#define KEY_F9        0x98
#define KEY_F10       0x99
#define KEY_F11       0x9A
#define KEY_F12       0x9B 
```
- Unique codes for **function keys**.
- Makes it easy to detect shortcuts or special kernel commands.
---
### 🧠 6. Extra Function Keys (F13–F24)
```c
#define KEY_F13       0xF0
#define KEY_F14       0xF1
#define KEY_F15       0xF2
#define KEY_F16       0xF3
#define KEY_F17       0xF4
#define KEY_F18       0xF5
#define KEY_F19       0xF6
#define KEY_F20       0xF7
#define KEY_F21       0xF8
#define KEY_F22       0xF9
#define KEY_F23       0xFA
#define KEY_F24       0xFB
```
- Rare, but some professional or server keyboards include these.
- Defined for completeness and future-proofing.
---
### 🧷 7. Modifier Keys
```c
#define KEY_LSHIFT    0xA0
#define KEY_RSHIFT    0xA1
#define KEY_CTRL      0xA2
#define KEY_ALT       0xA4
#define KEY_CAPSLOCK  0xA6
#define KEY_NUMLOCK   0xA7
#define KEY_SCROLL    0xA8
```
Represent keys that modify the behavior of others (e.g., `Shift`, `Ctrl`, `Alt`).
Toggle keys like `CapsLock`, `NumLock`, and `ScrollLock` also have dedicated codes.

---
### 🔢 8. Numeric Keypad (Numpad)
```c
#define KEY_KP0       0xB0
#define KEY_KP1       0xB1
#define KEY_KP2       0xB2
#define KEY_KP3       0xB3
#define KEY_KP4       0xB4
#define KEY_KP5       0xB5
#define KEY_KP6       0xB6
#define KEY_KP7       0xB7
#define KEY_KP8       0xB8
#define KEY_KP9       0xB9
#define KEY_KP_DOT    0xBA
#define KEY_KP_MUL    0xBC
#define KEY_KP_MINUS  0xBD
#define KEY_KP_PLUS   0xBE
#define KEY_KP_ENTER  0xBF
#define KEY_KP_EQUAL  0xFC
```
- Represents the **right-side numeric keypad keys**.
- Distinguishes between main number row (`'1'`, `'2'`, etc.) and numpad input.
---
### 🎛️ 9. System & Media Keys
```c
#define KEY_PRTSCR    0xC0
#define KEY_PAUSE     0xC1
#define KEY_SYSRQ     0xFD

#define KEY_LWIN      0xC2
#define KEY_RWIN      0xC3
#define KEY_MENU      0xC4

#define KEY_POWER     0xC5
#define KEY_SLEEP     0xC6
#define KEY_WAKE      0xC7

#define KEY_MUTE      0xC8
#define KEY_VOLUP     0xC9
#define KEY_VOLDOWN   0xCA
#define KEY_PLAY      0xCB
#define KEY_STOP      0xCC
#define KEY_NEXT      0xCD
#define KEY_PREV      0xCE
#define KEY_MAIL      0xCF
#define KEY_WWW       0xD0
```
#### Categories:
- **System Control**: `Print Screen`, `Pause`, `SysRq`
- **Windows/Meta Keys**: `LWIN`, `RWIN`, `MENU`
- **Power Management**: `POWER`, `SLEEP`, `WAKE`
- **Media Controls**: `PLAY`, `STOP`, `NEXT`, `VOLUP`, `MUTE`, etc.

These are derived from **extended scancodes** (`E0` prefix in PS/2 protocol).

---
### 🧩 10. Driver API
At the end, the header defines the public API used by other parts of the kernel:
```c
void keyboard_init(void);
int  keyboard_getchar(void);
void keyboard_irq_handler(void);
```
#### 🔹 `keyboard_init()`

Initializes the PS/2 keyboard driver, resets internal buffers,
and configures the hardware interface.

#### 🔹 `keyboard_getchar()`

Returns the next character (or key code) from the keyboard buffer.
If no key is available, it returns `-1`.

#### 🔹 `keyboard_irq_handler()`

Called automatically when **IRQ1** fires (keyboard interrupt).
It reads the scancode from port `0x60`, decodes it, and stores it in the input buffer.

---
### 🔗 11. Dependencies
This header cooperates with:
- `ps2.c` → actual implementation of logic and IRQ handling.
- `arch/x86_64/IRQ/port.h` → low-level I/O (`inb`, `outb`).
- `HAL/console/print.h` → optional debug/print output.
- `IDT subsystem` → connects `keyboard_irq_handler` to IRQ1.
---
### 🧮 12. Summary Table
| Category         | Description                                           |
| ---------------- | ----------------------------------------------------- |
| 🔡 ASCII keys    | Basic printable or control characters                 |
| 🧭 Cursor keys   | Navigation and editing controls                       |
| ⚙️ Function keys | F1–F24 for shortcuts and advanced input               |
| 🧷 Modifiers     | Shift, Ctrl, Alt, Lock keys                           |
| 🔢 Numpad        | Numeric keypad-specific keys                          |
| 🎵 Media keys    | Multimedia and system control                         |
| ⚙️ API           | Functions for init, IRQ handling, and input retrieval |
---
### 🧠 13. Concept Overview

`ps2.h` acts as a bridge between hardware-level keyboard logic
and higher-level input systems (like the kernel shell or GUI).
It exposes simple, human-readable constants and a clean API, hiding
low-level scancode handling and hardware interrupts.

---
### ✅ In short:
`ps2.h` provides a unified interface for PS/2 keyboard handling —
offering readable key codes, clear function prototypes,
and cross-module compatibility throughout the OS kernel.

---
# COSMOS-C Documentation Summary

## 📁 Folder Structure

| Folder         | Purpose                                                                 |
|----------------|-------------------------------------------------------------------------|
| `buildenv/`    | Environment setup for launching the `.iso` system.                      |
| `build/`       | Stores compiled object files (`.o`).                                    |
| `COSMOS-C/`    | Main system files — add new features here.                              |
| `dist/`        | Contains compiled `.bin` and bootable `.iso` after build.               |
| `src/`         | Place your system source files here.                                    |
| `targets/`     | Contains `linker.ld`, `grub.cfg`, and a copy of `kernel.bin`.           |

---

## 🛠️ Makefile

- Automates compilation using `make`.
- Handles C and ASM files from `src/`, `Core/arch/x86_64/`, and `HAL/Drivers/`.
- Produces `kernel.bin` and `kernel.iso`.
- Manual edits required when adding new files.
- Main command: `make build-x86_64`.

---

## 📂 .gitignore

- Prevents tracking of unnecessary files.
- COSMOS-C ignores:
  - `boot/kernel.bin`
  - Build artifacts, temp files, editor configs, secrets.

---

## 🧩 linker.ld

- Linker script for GNU `ld`.
- Defines memory layout:
  - `.boot` → Multiboot2 header
  - `.text` → Kernel code
- Used for low-level memory control.

---

## 🧨 grub.cfg

- GRUB 2 bootloader configuration.
- Sets:
  - Timeout
  - Default entry
  - Menu entry name
  - Boot command (`multiboot2 /boot/kernel.bin`)

---

## 🐳 Dockerfile

- Creates reproducible build environment.
- Based on `randomdude/gcc-cross-x86_64-elf`.
- Installs:
  - `nasm`, `xorriso`, `grub-pc-bin`, `grub-common`

---

## 🧠 main.c

- Entry point: `kernel_main()`
- Functions:
  - `kernel_init()` → clears screen, prints boot message
  - `hardwaresetup()` → sets up IDT, PIC, keyboard, IRQ
  - `kernel_update()` → main loop
  - `kb_update()` → keyboard input and cursor

---

## 🧬 Core/arch/x86_64

- Architecture-specific code.
- Subfolders:
  - `boot/` → early ASM code
  - `IDT/` → interrupt descriptor table
  - `PIC/` → programmable interrupt controller
  - `IRQ/` → interrupt requests

---

## ⚙️ header.asm

- Multiboot2 header for GRUB.
- Contains:
  - Magic number
  - Architecture ID
  - Header length
  - Checksum
  - End tag

---

## 🚀 main.asm

- Boot code executed after GRUB.
- Verifies:
  - Multiboot2
  - CPUID support
  - Long Mode support
- Initializes:
  - Page tables
  - GDT
  - Jumps to `long_mode_start`

---

## 🌐 main64.asm

- 64-bit entry point.
- Clears segment registers.
- Calls `kernel_main()`
- Halts if kernel returns.

---

## 🧩 IDT (idt.c / idt.h)

- Sets up interrupt descriptor table.
- Structures:
  - `idt_entry` → describes each interrupt vector
  - `idt_ptr` → passed to CPU via `lidt`
- Functions:
  - `set_idt_gate()` → configures vector
  - `idt_init()` → initializes and loads IDT
- Declares external ISR and IRQ stubs.

---

## ✅ System Boot Flow

1. GRUB loads `kernel.bin` via `grub.cfg`.
2. `header.asm` ensures Multiboot2 compliance.
3. `main.asm` checks environment and switches to Long Mode.
4. `main64.asm` calls `kernel_main()`.
5. `main.c` initializes system and enters main loop.
6. IDT handles interrupts and IRQs.
7. Everything runs in Docker or native environment.

---
## And that's it for now. Publish, experiment, comment, and report bugs. 
# See you soon!
