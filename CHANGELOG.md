# Official changelog for `COSMOS pre-alpha version 0.0.2`

## WHAT'S NEW????

1. **PIT Timer**  
   - Implemented a Programmable Interval Timer (PIT) for precise timing.  
   - Generates interrupts at a fixed frequency (1 kHz = 1ms tick).  
   - Enables sleeping, uptime measurement, and callback timers.

2. **`sleep_ms(int milliseconds)`**  
   - Provides a blocking sleep function for the kernel.  
   - Uses the PIT ticks to pause execution for the specified milliseconds.  
   - Ensures CPU halts (`hlt`) while waiting, conserving cycles.

3. **`setTimeout(function name (process), int milliseconds)`**  
   - Introduces software timer callbacks.  
   - Allows scheduling a function to run asynchronously after a delay.  
   - Supports multiple concurrent callbacks with a fixed-size array.

4. **Colorful kernel logs**  
   - Enhanced console printing with foreground and background colors.  
   - Supports 16-color palette for easier visual debugging.  
   - Functions include `print_set_color`, `print_str`, and `print_char`.

5. **Changelog**  
   - Added a dedicated changelog documenting new features and fixes.  
   - Helps track kernel development progress.  
   - Serves as a reference for future contributors.

6. **`documentation/` folder**  
   - Contains Markdown files documenting kernel modules and functions.  
   - Each major module has its own `.md` file (e.g., `timer.md`, `idt.md`).  
   - Provides detailed usage instructions and architecture explanations.

7. **`timer_uptime_ms(void)`**  
   - Returns the total elapsed time since kernel boot in milliseconds.  
   - Uses PIT ticks to calculate precise uptime.  
   - Useful for profiling, logging, or timing events in the kernel.

8. **`timer_print_uptime(void)`**  
   - Prints the kernel uptime in `HH:MM:SS.ms` format.  
   - Converts milliseconds into hours, minutes, seconds, and remaining milliseconds.  
   - Provides an easy way to monitor system time from the console.

9. **ROADMAP.md**
---
**🧠 Author:** COSMOS-C Kernel Team  
**📦 Module:** `CHANGELOG.md`  
**🕒 Last Updated:** 11 October 2025