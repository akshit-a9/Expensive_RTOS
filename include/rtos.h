// rtos.h
#ifndef RTOS_H
#define RTOS_H

#include "task.h"
#include "scheduler.h"

// Timeslice for round-robin (in microseconds)
#ifndef TIMESLICE_US
#define TIMESLICE_US 100000   // 100 ms
#endif

void RTOS_Init(void);
void RTOS_CreateTask(void (*taskFunc)(void *), void *param, int priority);
void RTOS_Start(void);

/* Exposed only for unit testing; not required by user code */
void *RTOS_Tick(void *arg);

#endif





