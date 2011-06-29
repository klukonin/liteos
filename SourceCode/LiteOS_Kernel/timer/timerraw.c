/** @file timerraw.c
	@brief The implementation of the raw timer module on top of clock. 

	@author Qing Charles Cao (cao@utk.edu)
*/


#include "timerraw.h"
#include "clockraw.h"
#include "../kernel/threadkernel.h"
#include "../kernel/scheduling.h"
#include "../hardware/avrhardware.h"
#include "generictimer.h"



uint32_t TimerM_mState;
uint8_t TimerM_setIntervalFlag;
uint8_t TimerM_mScale;
uint8_t TimerM_mInterval;
int8_t TimerM_queue_head;
int8_t TimerM_queue_tail;
uint8_t TimerM_queue_size;
uint8_t TimerM_queue[NUM_TIMERS];
volatile uint16_t TimerM_interval_outstanding;


struct TimerM_timer_s
{
    uint8_t type;
    int32_t ticks;
    int32_t ticksLeft;
} TimerM_mTimerList[NUM_TIMERS];
enum
{
    TimerM_maxTimerInterval = 230
};
inline result_t TimerM_StdControl_init(void)
{
    TimerM_mState = 0;
    TimerM_setIntervalFlag = 0;
    TimerM_queue_head = TimerM_queue_tail = -1;
    TimerM_queue_size = 0;
    TimerM_mScale = 3;
    TimerM_mInterval = TimerM_maxTimerInterval;
    return TimerM_Clock_setRate(TimerM_mInterval, TimerM_mScale);
}

//-------------------------------------------------------------------------
inline uint8_t TimerM_Timer_stop(uint8_t id)
{
    if (id > NUM_TIMERS)
    {
        return FAIL;
    }
    if (TimerM_mState & (0x1L << id))
    {
        _atomic_t _atomic = _atomic_start();

        TimerM_mState &= ~(0x1L << id);
        _atomic_end(_atomic);
        if (!TimerM_mState)
        {
            TimerM_setIntervalFlag = 1;
        }
        return SUCCESS;
    }
    return FAIL;
}

//-------------------------------------------------------------------------
inline result_t TimerM_Timer_start(uint8_t id, char type, uint32_t interval)
{
    uint8_t diff;

    if (id >= NUM_TIMERS)
    {
        return FAIL;
    }
    if (type > TIMER_ONE_SHOT)
    {
        return FAIL;
    }
    if (type == TIMER_REPEAT && interval <= 2)
    {
        return FAIL;
    }
    TimerM_mTimerList[id].ticks = interval;
    TimerM_mTimerList[id].type = type;
    {
        _atomic_t _atomic = _atomic_start();

        {
            diff = TimerM_Clock_readCounter();
            interval += diff;
            TimerM_mTimerList[id].ticksLeft = interval;
            TimerM_mState |= 0x1L << id;
            if (interval < TimerM_mInterval)
            {
                TimerM_mInterval = interval;
                TimerM_Clock_setInterval(TimerM_mInterval);
                TimerM_setIntervalFlag = 0;
            }
        }
        _atomic_end(_atomic);
    }
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t TimerM_Clock_setRate(char interval, char scale)
{
    unsigned char result;

    result = HPLClock_Clock_setRate(interval, scale);
    return result;
}

//-------------------------------------------------------------------------
void TimerM_adjustInterval(void)
{
    uint8_t i;
    uint8_t val = TimerM_maxTimerInterval;

    if (TimerM_mState)
    {
        for (i = 0; i < NUM_TIMERS; i++)
        {
            if (TimerM_mState & (0x1L << i) && TimerM_mTimerList[i].ticksLeft
                < val)
            {
                val = TimerM_mTimerList[i].ticksLeft;
            }
        }
        {
            _atomic_t _atomic = _atomic_start();

            {
                i = TimerM_Clock_readCounter() + 3;
                if (val < i)
                {
                    val = i;
                }
                TimerM_mInterval = val;
                TimerM_Clock_setInterval(TimerM_mInterval);
                TimerM_setIntervalFlag = 0;
            }
            _atomic_end(_atomic);
        }
    }
    else
    {
        {
            _atomic_t _atomic = _atomic_start();

            {
                TimerM_mInterval = TimerM_maxTimerInterval;
                TimerM_Clock_setInterval(TimerM_mInterval);
                TimerM_setIntervalFlag = 0;
            }
            _atomic_end(_atomic);
        }
    }
}

//-------------------------------------------------------------------------
inline void TimerM_enqueue(uint8_t value)
{
    if (TimerM_queue_tail == NUM_TIMERS - 1)
    {
        TimerM_queue_tail = -1;
    }
    TimerM_queue_tail++;
    TimerM_queue_size++;
    TimerM_queue[(uint8_t) TimerM_queue_tail] = value;
}

//-------------------------------------------------------------------------
inline result_t TimerM_Timer_default_fired(uint8_t id)
{
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t TimerM_Timer_fired(uint8_t id)
{
    GenericTimerFired(id);
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline uint8_t TimerM_dequeue(void)
{
    if (TimerM_queue_size == 0)
    {
        return NUM_TIMERS;
    }
    if (TimerM_queue_head == NUM_TIMERS - 1)
    {
        TimerM_queue_head = -1;
    }
    TimerM_queue_head++;
    TimerM_queue_size--;
    return TimerM_queue[(uint8_t) TimerM_queue_head];
}

//-------------------------------------------------------------------------
inline void TimerM_signalOneTimer(void)
{
    uint8_t itimer = TimerM_dequeue();

    if (itimer < NUM_TIMERS)
    {
        TimerM_Timer_fired(itimer);
    }
}

//-------------------------------------------------------------------------
inline void TimerM_HandleFire(void)
{
    uint8_t i;
    uint16_t int_out;

    TimerM_setIntervalFlag = 1;
    {
        _atomic_t _atomic = _atomic_start();

        {
            int_out = TimerM_interval_outstanding;
            TimerM_interval_outstanding = 0;
        }
        _atomic_end(_atomic);
    }
    if (TimerM_mState)
    {
        for (i = 0; i < NUM_TIMERS; i++)
        {
            if (TimerM_mState & (0x1L << i))
            {
                TimerM_mTimerList[i].ticksLeft -= int_out;
                if (TimerM_mTimerList[i].ticksLeft <= 2)
                {
                    if (postTask(TimerM_signalOneTimer, 7))
                    {
                        if (TimerM_mTimerList[i].type == TIMER_REPEAT)
                        {
                            TimerM_mTimerList[i].ticksLeft +=
                                TimerM_mTimerList[i].ticks;
                        }
                        else
                        {
                            TimerM_mState &= ~(0x1L << i);
                        }
                        TimerM_enqueue(i);
                    }
                    else
                    {
                        {
                        }
                        ;
                        TimerM_mTimerList[i].ticksLeft = TimerM_mInterval;
                    }
                }
            }
        }
    }
    {
        _atomic_t _atomic = _atomic_start();

        int_out = TimerM_interval_outstanding;
        _atomic_end(_atomic);
    }
    if (int_out == 0)
    {
        TimerM_adjustInterval();
    }
}

//-------------------------------------------------------------------------
inline result_t TimerM_Clock_fire(void)
{
    {
        _atomic_t _atomic = _atomic_start();

        {
            if (TimerM_interval_outstanding == 0)
            {
                postTask(TimerM_HandleFire, 12);
            }
            else
            {
            }
            ;
            TimerM_interval_outstanding += TimerM_Clock_getInterval() + 1;
        }
        _atomic_end(_atomic);
    }
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t TimerM_StdControl_start(void)
{
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline uint8_t TimerM_Clock_readCounter(void)
{
    unsigned char result;

    result = HPLClock_Clock_readCounter();
    return result;
}

//-------------------------------------------------------------------------
inline uint8_t TimerM_Clock_getInterval(void)
{
    unsigned char result;

    result = HPLClock_Clock_getInterval();
    return result;
}

//-------------------------------------------------------------------------
inline void TimerM_Clock_setInterval(uint8_t interval)
{
    HPLClock_Clock_setInterval(interval);
}
