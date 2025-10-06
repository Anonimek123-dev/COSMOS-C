# kernel C files
kernel_source_files := $(shell find src/ -name '*.c')
kernel_object_files := $(patsubst src/%.c, build/kernel/%.o, $(kernel_source_files))

# x86_64 C files
x86_64_c_source_files := $(shell find COSMOS-C/Core/arch/x86_64 -name '*.c')
x86_64_c_object_files := $(patsubst COSMOS-C/Core/arch/x86_64/%.c, build/x86_64/%.o, $(x86_64_c_source_files))

# x86_64 ASM files
x86_64_asm_source_files := $(shell find COSMOS-C/Core/arch/x86_64 -name '*.asm')
x86_64_asm_object_files := $(patsubst COSMOS-C/Core/arch/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

# drivers C files
drivers_c_source_files := $(shell find COSMOS-C/HAL -name '*.c')
drivers_c_object_files := $(patsubst COSMOS-C/HAL/%.c, build/x86_64/%.o, $(drivers_c_source_files))


# all x86_64 object files
x86_64_object_files := $(x86_64_c_object_files) $(x86_64_asm_object_files)

build/kernel/%.o: src/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I src/ -I COSMOS-C -I COSMOS-C/Core -I COSMOS-C/HAL -ffreestanding $< -o $@

build/x86_64/%.o: COSMOS-C/Core/arch/x86_64/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I COSMOS-C -I COSMOS-C/Core -I COSMOS-C/HAL -ffreestanding $< -o $@

build/x86_64/%.o: COSMOS-C/Core/arch/x86_64/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

build/x86_64/%.o: COSMOS-C/HAL/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I COSMOS-C -I COSMOS-C/Core -I COSMOS-C/HAL -ffreestanding $< -o $@

.PHONY: build-x86_64
build-x86_64: $(kernel_object_files) $(x86_64_object_files) $(drivers_c_object_files)
	mkdir -p dist/x86_64
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld \
		$(kernel_object_files) $(x86_64_object_files) $(drivers_c_object_files)
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso
