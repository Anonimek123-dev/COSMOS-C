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