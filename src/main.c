#include "rtos.h"
#include "xil_printf.h"

void TaskA(void *param) {
    xil_printf("Task A running with param %d\n\r", *((int *)param));
}

int main() {
    xil_printf("RTOS test start\n\r");

    RTOS_Init();
    static int p = 5;
    RTOS_CreateTask(TaskA, &p, 1);
    RTOS_Start();  // Will later trigger scheduler

    while (1);
}
