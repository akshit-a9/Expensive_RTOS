// // rtos.c
// #include "rtos.h"
// #include "xil_printf.h"
// #include <string.h>


// static int rrCursor[MAX_PRIORITY];

// /* Return next runnable task index:
//    - pick highest non-empty priority
//    - round-robin within that priority using rrCursor[]        */
// static int pick_next_task_index(void)
// {
//     for (int pr = 0; pr < MAX_PRIORITY; ++pr) {
//         // Count and remember first index of this priority
//         int firstIdx = -1, count = 0;

//         for (int i = 0; i < g_taskCount; ++i) {
//             if (g_tasks[i].state == TASK_READY && g_tasks[i].priority == pr) {
//                 if (firstIdx < 0) firstIdx = i;
//                 count++;
//             }
//         }
//         if (count == 0) continue;

//         // Compute the rr-th element among tasks with this priority
//         int pick = rrCursor[pr] % count;
//         int seen = 0;
//         for (int i = 0; i < g_taskCount; ++i) {
//             if (g_tasks[i].state == TASK_READY && g_tasks[i].priority == pr) {
//                 if (seen == pick) {
//                     rrCursor[pr] = (rrCursor[pr] + 1) % count;
//                     return i;
//                 }
//                 seen++;
//             }
//         }
//     }
//     return -1; // if nothing runnable
// }

// /* -------- Scheduler / Timer wiring -------- */

// static scheduler g_sched;

// // static scheduler sysScheduler;

// void RTOS_Init(void) {
// //     xil_printf("Initializing RTOS...\n\r");

//     memset(rrCursor, 0, sizeof(rrCursor));

//     // Register our tick handler as the scheduler callback
//     // NOTE: initScheduler expects a (void *(*)(void *)) signature in your header

// //  initScheduler(&sysScheduler, NULL, NULL);
//     initScheduler(&g_sched, RTOS_Tick, NULL);

//     xil_printf("TinyRTOS: Init done\n\r");
// }

// void RTOS_CreateTask(void (*taskFunc)(void *), void *param, int priority) {
// //     createTask(taskFunc, param, priority);
//         int id = createTask(taskFunc, param, priority);
//         if (id >= 0) {
//         xil_printf("TinyRTOS: Task %d created (pr=%d)\n\r", id, priority);
//     }
// }

// void RTOS_Start(void) {
// //     xil_printf("Starting scheduler...\n\r");
// //     // For now, run the first task manually
// //     TCB *t = getNextTask();
// //     if (t) {
// //         t->taskFunc(t->param);
// //     }
//        xil_printf("TinyRTOS: Start (timeslice=%d us)\n\r", (int)TIMESLICE_US);
// //     loadTimer(1000000); // Just to test timer interrupt
//     // Immediately arm the timer for preemption
//     loadTimer(TIMESLICE_US);
// }

// /* Called from timer ISR context (via scheduler.c). Keep it fast. */
// void *RTOS_Tick(void *arg)
// {
//     (void)arg;

//     int idx = pick_next_task_index();
//     if (idx >= 0) {
//         g_tasks[idx].state = TASK_RUNNING;
//         g_tasks[idx].runCount++;
//         g_tasks[idx].taskFunc(g_tasks[idx].param);
//         g_tasks[idx].state = TASK_READY;
//     }

//     // Re-arm the timer for the next timeslice
//     loadTimer(TIMESLICE_US);

//     return NULL;
// }


#include "rtos.h"
#include "xil_printf.h"
#include <string.h>

/* ------------ Scheduler state (priority + RR from Phase 2) ------------ */

static int rrCursor[MAX_PRIORITY];
static int currentIdx = -1;

static int pick_next_task_index(void)
{
    for (int pr = 0; pr < MAX_PRIORITY; ++pr) {
        int count = 0;
        for (int i = 0; i < g_taskCount; ++i)
            if (g_tasks[i].state == TASK_READY && g_tasks[i].priority == pr)
                count++;

        if (count == 0) continue;

        int target = rrCursor[pr] % count;
        int seen = 0;
        for (int i = 0; i < g_taskCount; ++i) {
            if (g_tasks[i].state == TASK_READY && g_tasks[i].priority == pr) {
                if (seen == target) {
                    rrCursor[pr] = (rrCursor[pr] + 1) % count;
                    return i;
                }
                seen++;
            }
        }
    }
    return -1;
}

/* ------------ Context switch glue used by ASM ------------ */

void rtos_save_sp_current(uint32_t *sp)
{
    if (currentIdx >= 0 && currentIdx < g_taskCount) {
        g_tasks[currentIdx].sp = sp;
        g_tasks[currentIdx].state = TASK_READY;
    }
}

uint32_t *rtos_get_next_sp(void)
{
    int next = pick_next_task_index();
    if (next < 0) return 0;

    currentIdx = next;
    g_tasks[currentIdx].state = TASK_RUNNING;
    g_tasks[currentIdx].runCount++;
    return g_tasks[currentIdx].sp;
}

/* On first restore, LR points here (set in createTask). */
void rtos_task_entry_trampoline(void)
{
    // Run the entry of the "current" task
    if (currentIdx >= 0 && currentIdx < g_taskCount) {
        TCB *t = &g_tasks[currentIdx];
        t->taskFunc(t->param);
    }
    // If task returns, mark finished and switch away
    rtos_task_exit();
}

void rtos_task_exit(void)
{
    if (currentIdx >= 0) {
        g_tasks[currentIdx].state = TASK_FINISHED;
    }
    // Trigger a context switch immediately
    extern void rtos_context_switch(void);
    rtos_context_switch();
    // No return
    while (1) {}
}

/* ------------ Timer ISR binding (preemption) ------------ */

static scheduler g_sched;

/* ISR callback (registered via initScheduler). Runs in IRQ context. */
static void *tick_isr(void *arg)
{
    (void)arg;
    extern void rtos_context_switch(void);

    // Preempt to next runnable task
    rtos_context_switch();

    // Re-arm timeslice
    loadTimer(TIMESLICE_US);
    return NULL;
}

/* ------------ Public APIs ------------ */

void RTOS_Init(void)
{
    memset(rrCursor, 0, sizeof(rrCursor));
    currentIdx = -1;
    initScheduler(&g_sched, tick_isr, NULL);
    xil_printf("TinyRTOS: Init (timeslice=%d us)\n\r", (int)TIMESLICE_US);
}

void RTOS_CreateTask(void (*taskFunc)(void *), void *param, int priority)
{
    (void)createTask(taskFunc, param, priority);
}

void RTOS_Start(void)
{
    // Pick the very first task and start restoring it
    (void)rtos_get_next_sp();

    // Kick the timer for periodic preemption
    loadTimer(TIMESLICE_US);

    // Jump into the first task context
    extern void rtos_start_first_task(void);
    rtos_start_first_task();

    // Should never return
    while (1) {}
}

void RTOS_Yield(void)
{
    extern void rtos_context_switch(void);
    rtos_context_switch();
}
