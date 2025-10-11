# main.c
## Definition
The **`main.c`** file is usually the **starting point of a C program**.  
It contains the (in COSMOS-C)`kernel_main()` function, which is the first function executed when a program starts running.

In the context of an **operating system kernel**, `main.c` often serves as the **entry point for initializing the system**.  
Here, instead of being a normal application, it may:
- Initialize hardware drivers (keyboard, screen, etc.).
- Set up memory management.
- Start the kernel loop that keeps the OS running.

---
## main.c in COSMOS-C
>**NOTE: Remember that main.c is already a file in your system, so you can modify it however you want (just like everything else), but I recommend leaving the functions and includes that are already there.**

**Let's now discuss in detail everything that is contained in the main.c file.**

---
**First, let's include all the necessary files from COSMOS-C:**
```c
#include "arch/x86_64/TIMER/timer.h"
#include "arch/x86_64/IDT/idt.h"
#include "arch/x86_64/PIC/pic.h"
#include "Drivers/PS2/keyboard/ps2.h"
#include "HAL/console/print.h"
#include "Core/arch/x86_64/TIMER/callback/callback.h"
#include <stdint.h>
```
>**NOTE: We will discuss all of the files mentioned later.**

---
**Now let's define all the functions we need in our kernel:**
```c
void kernel_init(void);
void hardwaresetup(void);
void kernel_update(void);
void enable_irq(void);
void kb_update(void);
void load_logs(void);
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

6. **`void load_logs()`**

**Loads the kernel logs.**

---
**✅ Summary in simple terms:**
- `kernel_init()` → system startup.
- `hardwaresetup()` → hardware preparation.
- `kernel_update()` → continuous system operation.
- `enable_irq()` → enabling interrupt response.
- `kb_update()` → keyboard and cursor handling.
- `load_logs()` → loads the kernel logs.

---
**Now let's see what happens in the `kernel_main()` function, which executes immediately after the kernel is loaded:**
```c
// Main kernel function
void kernel_main() {    
    hardwaresetup(); // set IDT, remap PIC, init keyboard, init timer, enable interrupts   
    kernel_init();   // Init 
    load_logs();
    while (1) {
        kernel_update();
    }
}
```
1. **Load the `hardware_setup()` function, which configures interrupts and devices**
2. **Load the `kernel_init()` function, which contains information about kernel startup**
3. **Load the `load_logs` function, which displays kernel logs**
4. **Load the main kernel loop (`kernel_update()`)**

**And that's it for `kernel_main()` function.**

---
**Let's move on to the next function: `hardware_setup()`.**
```c
// Called once to set up interrupts + devices
void hardwaresetup(void) {    
    idt_init();            // 1) initialize IDT (sets up interrupt gates)    
    pic_remap(0x20, 0x28); // 2) remap PIC so IRQs 0..15 map to vectors 0x20..0x2F   
    keyboard_init();       // 3) initialize keyboard driver (buffers, state)
    timer_init();          // 4) initialize timer
    enable_irq();          // 5) enable interrupts globally   
}
```
**Function `hardwaresetup()` is called to configure interrupts and devices. In this function:**
1. **`idt_init()` initialize IDT (sets up interrupt gates).**
2. **`pic_remap(0x20, 0x28)` remap PIC so IRQs 0..15 map to vectors 0x20..0x2F.**
3. **`keyboard_init()` initialize PS/2 keyboard driver.**
4. **`timer_init` initialize timer.**
4. **`enable_irq()` enable interrupts globally.**

---
**Now let's see what happens in `kernel_init()`!**
```c
// Init
void kernel_init() {
    print_clear();
    print_set_color(WHITE, BLACK);
    print_str("COSMOS-C booted successfully!\n");
}
```
**As you can see, the `kernel_main()` function is the first function loaded in the kernel, but this function loads the `kernel_init()` function, which is where the kernel actually starts. In the `kernel_init()` function:**
1. First, we clean the screen.
2. Then, using the `print_set_color()` function, we set the text color to white and the background color to black.
3. Finally, it prints **COSMOS-C booted successfully!** on the screen.

---
**The next function, which is somewhat important to me, is `load_logs()`.**
```c
// This function loads logs
void load_logs(void){
    /* IDT */        print_set_color(LIGHT_GREEN, BLACK); print_str("[IDT] ");          print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(500);  print_str("\n");
    /* PIC */        print_set_color(LIGHT_GREEN, BLACK); print_str("[PIC] ");          print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(500);  print_str("\n");
    /* KEYBOARD */   print_set_color(LIGHT_GREEN, BLACK); print_str("[PS/2 DRIVER] ");  print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(500);  print_str("\n");
    /* TIMER */      print_set_color(LIGHT_GREEN, BLACK); print_str("[PIT] ");          print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(500);  print_str("\n");
    /* INTERRUPTS */ print_set_color(LIGHT_GREEN, BLACK); print_str("[INTERRUPTS] ");   print_set_color(WHITE, BLACK);  print_str("initialized");  sleep_ms(500);  print_str("\n");
}
```
---
**We are left with the last kernel function, its main loop: `kernel_update()`, which runs every frame.**
```c
void kernel_update(void) {
    kb_update();
}
```
**And for now, we are only executing the `kb_update()` function.**

**And that's it for `main.c`.**
