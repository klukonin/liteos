/**  @file scheduling.c
        @brief The implementation for the scheduling module. 


        @author Qing Charles Cao (cao@utk.edu)
        
        
*/


#include "scheduling.h"
#include "../hardware/avrhardware.h"

typedef struct
{
    void (*tp) (void);
    uint8_t priority;
} LITE_sched_entry_T;
enum
{
    LITE_MAX_TASKS = 16, LITE_TASK_BITMASK = (LITE_MAX_TASKS - 1)
};
volatile LITE_sched_entry_T LITE_queue[LITE_MAX_TASKS];
volatile uint8_t LITE_sched_num;

#ifdef PLATFORM_CPU_MEASURE
uint32_t cpucounter;
#endif
void initScheduling(void)
{
    int i;

    LITE_sched_num = 0;
    for (i = 0; i < LITE_MAX_TASKS; i++)
    {
        LITE_queue[i].tp = (void *)0;
    }
#ifdef PLATFORM_CPU_MEASURE
    cpucounter = 0;
#endif
}

//-------------------------------------------------------------------------
bool postTask(void (*tp) (void), uint8_t priority)
{
    _atomic_t fInterruptFlags;
    uint8_t tmp;

    fInterruptFlags = _atomic_start();
    if (LITE_sched_num == LITE_MAX_TASKS)
    {
        _atomic_end(fInterruptFlags);
        return FALSE;
    }
    for (tmp = 0; tmp < LITE_MAX_TASKS; tmp++)
    {
        if (LITE_queue[tmp].tp == NULL)
        {
            LITE_queue[tmp].tp = tp;
            LITE_queue[tmp].priority = priority;
            LITE_sched_num++;
            _atomic_end(fInterruptFlags);
            return TRUE;
        }
    }
    _atomic_end(fInterruptFlags);
    return FALSE;
}

//-------------------------------------------------------------------------
bool runNextTask()
{
    _atomic_t fInterruptFlags;
    uint8_t currentpriority;
    uint8_t tmp;
    uint8_t reserved;
    void (*func) (void);

    reserved = 0;
    fInterruptFlags = _atomic_start();
    currentpriority = 0;
    func = NULL;
    if (LITE_sched_num == 0)
    {
        _atomic_end(fInterruptFlags);
        _avr_enable_interrupt();
#ifdef PLATFORM_CPU_MEASURE
        cpucounter++;
#else
        _avr_sleep();
        //   printfstr("ABOUT TO SLEEP!!\n");
#endif
        return FALSE;
    }
    for (tmp = 0; tmp < LITE_MAX_TASKS; tmp++)
    {
        if ((LITE_queue[tmp].tp != NULL) && (LITE_queue[tmp].priority >
                                             currentpriority))
        {
            func = LITE_queue[tmp].tp;
            reserved = tmp;
            currentpriority = LITE_queue[tmp].priority;
        }
    }
    if (func != NULL)
    {
        LITE_sched_num--;
        LITE_queue[reserved].tp = NULL;
    }
    _atomic_end(fInterruptFlags);
    if (func != NULL)
    {
        func();
    }
    return TRUE;
}
