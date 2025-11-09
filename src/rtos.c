// rtos.c
#include "rtos.h"
#include "xil_printf.h"

static scheduler sysScheduler;

void RTOS_Init(void) {
    xil_printf("Initializing RTOS...\n\r");
    initScheduler(&sysScheduler, NULL, NULL);
}

void RTOS_CreateTask(void (*taskFunc)(void *), void *param, int priority) {
    createTask(taskFunc, param, priority);
}

void RTOS_Start(void) {
    xil_printf("Starting scheduler...\n\r");
    // For now, run the first task manually
    TCB *t = getNextTask();
    if (t) {
        t->taskFunc(t->param);
    }
    loadTimer(1000000); // Just to test timer interrupt
}
