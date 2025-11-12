#include "rtos.h"
#include "xil_printf.h"

// void TaskA(void *param) {
//     xil_printf("Task A running with param %d\n\r", *((int *)param));
// }



/* Demo tasks:
   - Two medium-priority tasks (1) to show round-robin
   - One high-priority task (0) that preempts others             */

void TaskHigh(void *p)
{
    static int c = 0;
    xil_printf("[H pr=0] tick=%d param=%d\n\r", ++c, *((int*)p));
}

void TaskMedA(void *p)
{
    static int c = 0;
    xil_printf("  [M1 pr=1] tick=%d param=%d\n\r", ++c, *((int*)p));
}

void TaskMedB(void *p)
{
    static int c = 0;
    xil_printf("  [M2 pr=1] tick=%d param=%d\n\r", ++c, *((int*)p));
}

int main(void)
{
    // xil_printf("RTOS test start\n\r");
    xil_printf("=== TinyRTOS Phase 2 Demo ===\n\r");

    RTOS_Init();
    // static int p = 5;
    static int pa = 10, pb = 20, pc = 30;
    // RTOS_CreateTask(TaskA, &p, 1);
    RTOS_CreateTask(TaskHigh, &pa, 0);  // highest prio
    RTOS_CreateTask(TaskMedA, &pb, 1);  // medium
    RTOS_CreateTask(TaskMedB, &pc, 1);  // medium (same prio → round-robin)

    RTOS_Start();

    while (1) {
        // idle loop; all work happens on timer ticks in RTOS_Tick()
    }
}
