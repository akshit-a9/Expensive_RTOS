// task.h
#ifndef TASK_H
#define TASK_H

#define MAX_TASKS 10

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_WAITING
} TaskState;

typedef struct {
    void (*taskFunc)(void *); // Function pointer
    void *param;              // Task argument
    int priority;             // Priority (lower number = higher priority)
    TaskState state;
} TCB;

extern TCB taskList[MAX_TASKS];
extern int taskCount;

void createTask(void (*taskFunc)(void *), void *param, int priority);
TCB *getNextTask(void);

#endif
