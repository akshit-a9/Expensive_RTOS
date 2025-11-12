// // task.h
// #ifndef TASK_H
// #define TASK_H

// #define MAX_TASKS 10

// typedef enum {
//     TASK_READY,
//     TASK_RUNNING,
//     TASK_WAITING
// } TaskState;

// typedef struct {
//     void (*taskFunc)(void *); // Function pointer
//     void *param;              // Task argument
//     int priority;             // Priority (lower number = higher priority)
//     TaskState state;
// } TCB;

// extern TCB taskList[MAX_TASKS];
// extern int taskCount;

// void createTask(void (*taskFunc)(void *), void *param, int priority);
// TCB *getNextTask(void);

// #endif

#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define MAX_TASKS     10
#define MAX_PRIORITY  8   // 0 = highest priority

typedef enum {
    TASK_READY = 0,
    TASK_RUNNING,
    TASK_WAITING
} TaskState;

typedef struct {
    void (*taskFunc)(void *); // task entry
    void *param;              // argument
    int   priority;           // 0..MAX_PRIORITY-1 (0 = highest)
    TaskState state;

    // simple stats for visibility
    uint32_t runCount;
} TCB;

/* Public task API */
int   createTask(void (*taskFunc)(void *), void *param, int priority);

/* Internal (used by scheduler/rtos) */
extern TCB     g_tasks[MAX_TASKS];
extern int     g_taskCount;

#endif
