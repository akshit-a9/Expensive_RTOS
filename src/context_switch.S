/* TinyRTOS context switching for ARM Cortex-A9 (AArch32)
   - Saves r4-r11, lr of the running task
   - Persists SP through rtos_save_sp_current()
   - Selects next task's SP via rtos_get_next_sp()
   - Restores r4-r11, lr of the next task and branches into it
*/

    .syntax unified
    .cpu cortex-a9
    .fpu softvfp
    .text

    .global rtos_context_switch
    .type   rtos_context_switch, %function

    .global rtos_start_first_task
    .type   rtos_start_first_task, %function

    .extern rtos_save_sp_current
    .extern rtos_get_next_sp

/* void rtos_context_switch(void) */
rtos_context_switch:
    /* Save callee-saved regs + lr */
    stmdb   sp!, {r4-r11, lr}

    /* r0 = sp ; save it in current TCB */
    mov     r0, sp
    bl      rtos_save_sp_current

    /* r0 = next task SP (or 0 if none) */
    bl      rtos_get_next_sp
    /* If no runnable tasks, just restore and return */
    cmp     r0, #0
    beq     1f

    /* Switch to next task stack */
    mov     sp, r0

1:  /* Restore and resume */
    ldmia   sp!, {r4-r11, lr}
    bx      lr

/* Jump into the very first selected task.
   Assumes rtos_get_next_sp() has already run. */
rtos_start_first_task:
    bl      rtos_get_next_sp
    cmp     r0, #0
    beq     2f
    mov     sp, r0
    ldmia   sp!, {r4-r11, lr}
    bx      lr

2:  /* No task: spin */
    b       2b
