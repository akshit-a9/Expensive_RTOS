// // task.c
// #include "task.h"
// // #include <stdio.h>
// #include "xil_printf.h"

// // TCB taskList[MAX_TASKS];
// TCB g_tasks[MAX_TASKS];
// // int taskCount = 0;
// int g_taskCount = 0;

// // void createTask(void (*taskFunc)(void *), void *param, int priority) {
// int createTask(void (*taskFunc)(void *), void *param, int priority)
// {
//     if (g_taskCount >= MAX_TASKS) {
//         xil_printf("TinyRTOS: Max tasks reached\n\r");
//         return -1;
//     }

//     if (priority < 0 || priority >= MAX_PRIORITY) {
//         xil_printf("TinyRTOS: Invalid priority %d\n\r", priority);
//         return -2;
//     }

// //     taskList[taskCount].taskFunc = taskFunc;
// //     taskList[taskCount].param = param;
// //     taskList[taskCount].priority = priority;
// //     taskList[taskCount].state = TASK_READY;
// //     taskCount++;

//     g_tasks[g_taskCount].taskFunc = taskFunc;
//     g_tasks[g_taskCount].param    = param;
//     g_tasks[g_taskCount].priority = priority;
//     g_tasks[g_taskCount].state    = TASK_READY;
//     g_tasks[g_taskCount].runCount = 0;
//     g_taskCount++;

// // TCB *getNextTask(void) {
// //     if (taskCount == 0) return NULL;
// //     return &taskList[0];
// // }

//     return g_taskCount - 1;
//}

#include "task.h"
#include "xil_printf.h"

/* Provided by rtos.c */
void rtos_task_entry_trampoline(void);

TCB g_tasks[MAX_TASKS];
int g_taskCount = 0;

/* Initialize the initial stack frame so that
   restoring {r4-r11, lr} then BX LR jumps to the trampoline. */
static void init_task_stack(TCB *t)
{
    // Clear stack for sanity (optional)
    for (int i = 0; i < STACK_WORDS; ++i) t->stack[i] = 0;

    // Build a fake frame: r4..r11, then LR (9 regs total)
    uint32_t *top = &t->stack[STACK_WORDS]; // one past last
    top -= 9;                                // space for r4-r11, lr
    for (int i = 0; i < 8; ++i) top[i] = 0;  // r4..r11
    top[8] = (uint32_t)rtos_task_entry_trampoline; // LR
    t->sp = top;                              // SP points to saved regs
}

int createTask(void (*taskFunc)(void *), void *param, int priority)
{
    if (g_taskCount >= MAX_TASKS) {
        xil_printf("TinyRTOS: Max tasks reached\n\r");
        return -1;
    }
    if (priority < 0 || priority >= MAX_PRIORITY) {
        xil_printf("TinyRTOS: Invalid priority %d\n\r", priority);
        return -2;
    }

    TCB *t = &g_tasks[g_taskCount];
    t->taskFunc = taskFunc;
    t->param    = param;
    t->priority = priority;
    t->state    = TASK_READY;
    t->runCount = 0;

    init_task_stack(t);

    xil_printf("TinyRTOS: Task %d ready (pr=%d, sp=0x%08lx)\n\r",
               g_taskCount, priority, (unsigned long)t->sp);

    return g_taskCount++;
}
