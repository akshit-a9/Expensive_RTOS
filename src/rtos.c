// rtos.c
#include "rtos.h"
#include "xil_printf.h"
#include <string.h>


static int rrCursor[MAX_PRIORITY];

/* Return next runnable task index:
   - pick highest non-empty priority
   - round-robin within that priority using rrCursor[]        */
static int pick_next_task_index(void)
{
    for (int pr = 0; pr < MAX_PRIORITY; ++pr) {
        // Count and remember first index of this priority
        int firstIdx = -1, count = 0;

        for (int i = 0; i < g_taskCount; ++i) {
            if (g_tasks[i].state == TASK_READY && g_tasks[i].priority == pr) {
                if (firstIdx < 0) firstIdx = i;
                count++;
            }
        }
        if (count == 0) continue;

        // Compute the rr-th element among tasks with this priority
        int pick = rrCursor[pr] % count;
        int seen = 0;
        for (int i = 0; i < g_taskCount; ++i) {
            if (g_tasks[i].state == TASK_READY && g_tasks[i].priority == pr) {
                if (seen == pick) {
                    rrCursor[pr] = (rrCursor[pr] + 1) % count;
                    return i;
                }
                seen++;
            }
        }
    }
    return -1; // if nothing runnable
}

/* -------- Scheduler / Timer wiring -------- */

static scheduler g_sched;

// static scheduler sysScheduler;

void RTOS_Init(void) {
//     xil_printf("Initializing RTOS...\n\r");

    memset(rrCursor, 0, sizeof(rrCursor));

    // Register our tick handler as the scheduler callback
    // NOTE: initScheduler expects a (void *(*)(void *)) signature in your header

//  initScheduler(&sysScheduler, NULL, NULL);
    initScheduler(&g_sched, RTOS_Tick, NULL);

    xil_printf("TinyRTOS: Init done\n\r");
}

void RTOS_CreateTask(void (*taskFunc)(void *), void *param, int priority) {
//     createTask(taskFunc, param, priority);
        int id = createTask(taskFunc, param, priority);
        if (id >= 0) {
        xil_printf("TinyRTOS: Task %d created (pr=%d)\n\r", id, priority);
    }
}

void RTOS_Start(void) {
//     xil_printf("Starting scheduler...\n\r");
//     // For now, run the first task manually
//     TCB *t = getNextTask();
//     if (t) {
//         t->taskFunc(t->param);
//     }
       xil_printf("TinyRTOS: Start (timeslice=%d us)\n\r", (int)TIMESLICE_US);
//     loadTimer(1000000); // Just to test timer interrupt
    // Immediately arm the timer for preemption
    loadTimer(TIMESLICE_US);
}

/* Called from timer ISR context (via scheduler.c). Keep it fast. */
void *RTOS_Tick(void *arg)
{
    (void)arg;

    int idx = pick_next_task_index();
    if (idx >= 0) {
        g_tasks[idx].state = TASK_RUNNING;
        g_tasks[idx].runCount++;
        g_tasks[idx].taskFunc(g_tasks[idx].param);
        g_tasks[idx].state = TASK_READY;
    }

    // Re-arm the timer for the next timeslice
    loadTimer(TIMESLICE_US);

    return NULL;
}
