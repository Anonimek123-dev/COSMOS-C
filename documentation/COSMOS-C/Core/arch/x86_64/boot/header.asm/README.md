# header.asm
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