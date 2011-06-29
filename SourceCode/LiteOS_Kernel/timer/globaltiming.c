/** @file globaltiming.c
	@brief The detailed implementations of the global timing module. 

	@author Qing Charles Cao (cao@utk.edu)
*/

#include "globaltiming.h"
#include "timerraw.h"
#include "clockraw.h"



static currentTimeUnit internaltime1, internaltime2;
static uint8_t timeselect;

currentTimeUnit *getCurrentTimeStamp()
{

    uint16_t counter1, counter2;
    uint32_t resolution;

    counter1 = HPLClock_readTimeCounterHigh();
    resolution = HPLClock_readTimeCounterLow();
    counter2 = HPLClock_readTimeCounterHigh();
    if (counter1 == counter2)
    {
    }
    else
    {
        counter1 = HPLClock_readTimeCounterHigh();
        resolution = HPLClock_readTimeCounterLow();
        counter2 = HPLClock_readTimeCounterHigh();
    }
    if (timeselect == 1)
    {
        internaltime1.counter = counter1;
        internaltime1.resolution = resolution;
        timeselect = 2;
        return &internaltime1;
    }
    if (timeselect == 2)
    {
        timeselect = 1;
        internaltime2.counter = counter1;
        internaltime2.resolution = resolution;
        return &internaltime2;
    }

    return NULL;
}

//-------------------------------------------------------------------------
uint16_t getCurrentCounterHigh()
{

    return HPLClock_readTimeCounterHigh();

}

//-------------------------------------------------------------------------
uint32_t getCurrentResolution()
{
{

    uint32_t test;
    //printfstr("test0\n");
    
    test= HPLClock_readTimeCounterLow();
    //printfintegeru32(test);
    return test;

}
}

//-------------------------------------------------------------------------
void GenericTimingStart()
{

    timeselect = 1;
    HPLClock_Timer3_Start();

}

//-------------------------------------------------------------------------
void GenericTimingStop()
{

    HPLClock_Timer3_Stop();

}
