# OFFICIAL COSMOS-C ROADMAP

## pre-alpha versions:
- **`pre-alpha 0.0.2`**
    - **PIT Timer,**
    - **`sleep_ms()`,**
    - **`setTimeout()`,**
    - **`timer_uptime_ms()`,**
    - **colorful kernel logs,**
    - **changelog,**
    - **`documentation/` folder,**
    - **`timer_uptime_ms(void)`,**
    - **`timer_print_uptime(void)`****
- **`pre-alpha 0.0.3`**
    - **input**
    - **logo**
    - **Physical memory (PMM)**
    - **IDE/NVMe**
    - **VFS (Virtual File System)**
- **`pre-alpha 0.0.4`**
    - **PCI**
    - **LAPIC Timer (Local APIC Timer)**
    - **RTC (Real-Time Clock)**
    - **Do not initialize `gs` and `fs` in main64.asm and `cli` before `hlt`.**
    - **Use Limine to have everything set up right away for paging.**
    - **Create pointer arrays for ISR_xx functions and use them in the C array.**