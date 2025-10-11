# Makefile
## Definition

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

## Makefile structure
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