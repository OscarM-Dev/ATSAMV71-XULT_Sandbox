#include "SchM.h"

/* Minimal cooperative scheduler implementation to satisfy undefined references. */

/* Pointer to active schedule configuration */
static SchMCfgType* s_SchMCfg = 0;
/* Bitmask of pending tasks by TaskId (supports up to 32 IDs) */
static volatile unsigned long s_pendingMask = 0UL;
/* Optional started flag */
static volatile unsigned char s_started = 0U;

void SchM_Init(SchMCfgType *SchMCfg)
{
    s_SchMCfg = SchMCfg;
    s_pendingMask = 0UL;
    s_started = 0U;
}

void SchM_Start(void)
{
    s_started = 1U;
}

void SchM_Stop(void)
{
    s_started = 0U;
}

void SchM_ActivateTask(SchMTaskIdType TaskId)
{
    if ((unsigned long)TaskId < 32UL) {
        s_pendingMask |= (1UL << (unsigned long)TaskId);
    }
}

void SchM_SchedulePoint(void)
{
    if (!s_SchMCfg) {
        return;
    }

    /* Iterate configured tasks; if their TaskId is pending, run and clear it. */
    unsigned int i;
    for (i = 0U; i < s_SchMCfg->NoOfTasks; i++) {
        SchMTaskType* t = &s_SchMCfg->TaskCfg[i];
        unsigned long mask = (1UL << (unsigned long)t->TaskId);
        if (s_pendingMask & mask) {
            /* Clear first to avoid re-entrancy issues */
            s_pendingMask &= ~mask;
            if (t->TaskFcnPtr) {
                t->TaskFcnPtr();
            }
        }
    }
}

void SchM_Scheduler(void)
{
    /* Simple cooperative loop: run pending tasks when started */
    while (1) {
        if (s_started) {
            SchM_SchedulePoint();
        }
    }
}
