// task.c
#include "task.h"
// #include <stdio.h>
#include "xil_printf.h"

// TCB taskList[MAX_TASKS];
TCB g_tasks[MAX_TASKS];
// int taskCount = 0;
int g_taskCount = 0;

// void createTask(void (*taskFunc)(void *), void *param, int priority) {
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

//     taskList[taskCount].taskFunc = taskFunc;
//     taskList[taskCount].param = param;
//     taskList[taskCount].priority = priority;
//     taskList[taskCount].state = TASK_READY;
//     taskCount++;

    g_tasks[g_taskCount].taskFunc = taskFunc;
    g_tasks[g_taskCount].param    = param;
    g_tasks[g_taskCount].priority = priority;
    g_tasks[g_taskCount].state    = TASK_READY;
    g_tasks[g_taskCount].runCount = 0;
    g_taskCount++;

// TCB *getNextTask(void) {
//     if (taskCount == 0) return NULL;
//     return &taskList[0];
// }

    return g_taskCount - 1;
}
