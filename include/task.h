// // task.h
// #ifndef TASK_H
// #define TASK_H

// #include <stdint.h>

// #define MAX_TASKS 10
// #define MAX_PRIORITY  8   // 0 = highest priority

// typedef enum {
//     TASK_READY = 0,
//     TASK_RUNNING,
//     TASK_WAITING
// } TaskState;

// typedef struct {
//     void (*taskFunc)(void *); // task entry
//     void *param;              // argument
//     int   priority;           // 0..MAX_PRIORITY-1 (0 = highest)
//     TaskState state;

//     // simple stats for visibility
//     uint32_t runCount;
// } TCB;

// // extern TCB taskList[MAX_TASKS];
// // extern int taskCount;

// /* Public task API */
// int   createTask(void (*taskFunc)(void *), void *param, int priority);

// /* Internal (used by scheduler/rtos) */
// extern TCB     g_tasks[MAX_TASKS];
// extern int     g_taskCount;

// #endif

#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define MAX_TASKS      10
#define MAX_PRIORITY   8          // 0 = highest
#define STACK_WORDS    256        // 1KB per task stack (adjust as needed)

typedef enum {
    TASK_READY = 0,
    TASK_RUNNING,
    TASK_WAITING,
    TASK_FINISHED
} TaskState;

typedef struct {
    // Scheduling
    int        priority;          // 0..MAX_PRIORITY-1
    TaskState  state;
    uint32_t   runCount;

    // Entry & arg
    void     (*taskFunc)(void *);
    void      *param;

    // Context
    uint32_t   *sp;               // saved stack pointer
    uint32_t    stack[STACK_WORDS];
} TCB;

/* API */
int  createTask(void (*taskFunc)(void *), void *param, int priority);

/* Globals (used by RTOS core) */
extern TCB  g_tasks[MAX_TASKS];
extern int  g_taskCount;

#endif









