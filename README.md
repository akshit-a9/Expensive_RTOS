# Expensive_RTOS : A real time OS

This repository contains the implementation of **Expensive_RTOS**, a minimal real-time operating system built for the **ARM Cortex-A9** processor on the **Zedboard** platform.  
It is developed as part of the *Real-Time Embedded Systems* coursework at **BITS Pilani, K.K. Birla Goa Campus**.

---

## Overview
Expensive_RTOS is a lightweight, custom operating system that provides:
- Task creation and management APIs
- Priority-based scheduling with round-robin arbitration
- Timer-driven preemption
- Standalone BSP support

The OS is designed to run on top of **Xilinx’s standalone environment**, using some provided **bitstream (`.bit`)** and **XSA (`.xsa`)** files for timer integration.

---

## Project Structure
```
TinyRTOS/
│
├── hardware/
│   ├── design_1_wrapper.xsa          # Hardware handoff file for Vitis
│   └── design_1_wrapper.bit          # Bitstream with timer peripheral
│
├── src/
│   ├── main.c                        # Example test task
│   ├── rtos.c                        # Core RTOS logic
│   ├── task.c                        # Task creation & management
│   ├── scheduler.c                   # Timer ISR & scheduling base
│   └── context_switch.S              # (Future) Context switch routines
│
├── include/
│   ├── rtos.h
│   ├── task.h
│   └── scheduler.h
│
└── vitis_workspace/                  # Generated automatically by Vitis
```

---

## Build & Run (Vitis)
1. **Open Vitis IDE** → *Create Application Project*  
   - Hardware platform: `hardware/design_1_wrapper.xsa`  
   - OS: **standalone** (not FreeRTOS)  
2. Replace the auto-generated `src/` and `include/` folders with those from this repo.
3. Add `include/` to project include paths.
4. Build
5. Program the FPGA with `design_1_wrapper.bit`
6. Launch the ELF on hardware (Zedboard).

---

##  Development Phases
| Phase | Description |
|--------|-------------|
| 1 | Base skeleton – single task, timer interrupt test |
| 2 | Priority scheduling + round-robin logic |
| 3 | Context switching (assembly) |
| 4 | Task delay / yield and timing refinement |

We have succesfully completed Phase 1. Currently, added some files for Phase 2. 

##  Team Members
| Enrollment No. | Name |
|--------|-------------|
| 2025H1400065G | Ayushman Muduli |
| 2025H1400069G | Akshit Sharma |
| 2025H1400076G | Sukrutha T S |

