// task.c
#include "task.h"
#include <stdio.h>

TCB taskList[MAX_TASKS];
int taskCount = 0;

void createTask(void (*taskFunc)(void *), void *param, int priority) {
    if (taskCount >= MAX_TASKS) {
        printf("Max tasks reached!\n");
        return;
    }
    taskList[taskCount].taskFunc = taskFunc;
    taskList[taskCount].param = param;
    taskList[taskCount].priority = priority;
    taskList[taskCount].state = TASK_READY;
    taskCount++;
}

TCB *getNextTask(void) {
    // Temporary: just return first task for sanity check
    if (taskCount == 0) return NULL;
    return &taskList[0];
}
