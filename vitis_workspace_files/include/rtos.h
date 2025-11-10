// rtos.h
#ifndef RTOS_H
#define RTOS_H

#include "task.h"
#include "scheduler.h"

void RTOS_Init(void);
void RTOS_CreateTask(void (*taskFunc)(void *), void *param, int priority);
void RTOS_Start(void);

#endif
