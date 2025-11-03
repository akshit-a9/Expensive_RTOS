# TinyRTOS for Zedboard (Cortex-A9)

A minimal real-time operating system being developed for the **Zedboard (ARM Cortex-A9)** platform. This is a course project for G515 - Real Time Embedded Systems at BPGC. 

The OS will provide basic task creation and scheduling, using a priority-based preemptive scheduler with round-robin arbitration for equal-priority tasks. It will rely on the **FPGA timer interrupt** for its system tick.

## Current Status
Initial setup phase — core design and team roles to be finalized.

## Build Target
- **Platform:** Zedboard (ARM Cortex-A9)
- **Toolchain:** Vitis (Standalone BSP)
- **Timer:** FPGA Timer (bitstream & API provided)