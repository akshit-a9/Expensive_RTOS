// // rtos.h
// #ifndef RTOS_H
// #define RTOS_H

// #include "task.h"
// #include "scheduler.h"

// // Timeslice for round-robin (in microseconds)
// #ifndef TIMESLICE_US
// #define TIMESLICE_US 100000   // 100 ms
// #endif

// void RTOS_Init(void);
// void RTOS_CreateTask(void (*taskFunc)(void *), void *param, int priority);
// void RTOS_Start(void);

// /* Exposed only for unit testing; not required by user code */
// void *RTOS_Tick(void *arg);

// #endif

#ifndef RTOS_H
#define RTOS_H

#include "task.h"
#include "scheduler.h"

#ifndef TIMESLICE_US
#define TIMESLICE_US 100000  // 100 ms
#endif

void RTOS_Init(void);
void RTOS_CreateTask(void (*taskFunc)(void *), void *param, int priority);
void RTOS_Start(void);

/* Called only from ASM or ISR */
void rtos_save_sp_current(uint32_t *sp);
uint32_t *rtos_get_next_sp(void);

/* Helpers used by task.c initial frame */
void rtos_task_entry_trampoline(void);
void rtos_task_exit(void);

/* Optional: cooperative yield from tasks */
void RTOS_Yield(void);

#endif




