# `targets/` 
- There you will find the linker file (`linker.ld`), the file `grub.cfg` and the raw system binary file too.
---
# `kernel.bin`
- the raw system binary file
---
# `linker.ld`

## Definition
**`linker.ld` is a script file used by GNU Linker (`ld`) to define how data and code are laid out in an execuative file or library. In practice, it is a configuration for the linker that allows the programmer to specify where individual sections of code and data should be located (e. g., in memory), which librares to use, and how to link object files to create the final program.**

## What is a linker?

- **A linker is a program that combines previosly compiled modules (object files) and librares into a**
**single execuatle file or shared library.**
- **This is a one of the stages of program creation, following compilation, which translates source code** **into machine language.**
- **In UNIX systems, the `.ld` program, which is a part of the GNU Binutils project, is often used.**

## The role of the linker.ld file
- **A file with the `.ld` (or similar) extention is a configuration file or linker script that contains instructions for the `.ld` program (e.g., text code, data, stack) in memory.** 
- **The programmer can specify which librares are to be included in the final file and which are to remain as dynamic librares to be loaded during program execution.**

## Why is needed?
**`linker.ld` is used when more complex control over the linking process is needed, beyond the default linker settings. It is particularly useful when developing embedded systems or when manual memory managment is required.**

## linker.ld in COSMOS-C
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
---
# `grub.cfg`
## Definition
`grub.cfg` – the main configuration file for the **GRUB 2 boot loader**.
It specifies how GRUB should behave when the computer starts up, i.e.:
- **the wait time** (how many seconds GRUB should wait for the system to be selected),
- **the default entry** (which system should start automatically),
- **menuentry** – entries in the boot menu, each of which describes an operating system or kernel that can be booted,
- **boot commands** – e.g., multiboot2 (for a multiboot2-compatible kernel), linux (for Linux), initrd (for initramfs), or boot (start command).
**Thanks to grub.cfg, the user can boot different operating systems or different configurations of the same system.**

---
## grub.cfg in COSMOS-C
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
---