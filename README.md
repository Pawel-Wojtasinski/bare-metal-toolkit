# AVR Event-Driven Cooperative Scheduler (Lightweight RTOS Alternative)

A minimalist, cooperative event-driven scheduler designed specifically for 8-bit AVR microcontrollers (tested on ATmega328P / Arduino Pro Mini). 
This project aims to be a clean, robust alternative to heavy RTOS solutions and unstructured `while(1)` bare-metal code, offering deterministic execution, low RAM overhead, and built-in power saving.

## 🚀 Key Features
* **Event-Driven & Cooperative:** Predictable *Run-to-Completion* execution model. No complex task preemption or heavy per-task RAM stacks.
* **Power Efficient (Sleep Mode):** Automatically enters AVR `Idle` sleep mode when no events are pending, reducing power consumption.
* **Context Protection:** Robust interrupt/user context verification using `SREG` flag checking to catch architectural design bugs early.
* **Zero Dynamic Allocation:** Everything is statically allocated using clean X-Macros and configuration headers.
* **Safe Ring Buffers:** Thread-safe circular buffers for UART communication with advanced deadlock prevention and error handling.
* **Software Timers:** Integrated multi-instance software timers mapped directly to system ticks.

## 📄 License
This project is open-source and available under the MIT License.
