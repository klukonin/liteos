/** @file generictimer.c
	@brief The detailed implementation of the generic timer module. 

	@author Qing Charles Cao (cao@utk.edu)
*/


#include "micaz/clockraw.h"
#include "timerraw.h"
#include "../kernel/scheduling.h"
#include "../kernel/threadkernel.h"

//used for blink only. 
#include "../sensors/leds.h"

#include "../kernel/threadtools.h"
#include "../io/cc2420/hplcc2420interruptm.h"

#include "../io/radio/amcommon.h"
#include "../io/radio/amradio.h"
#include "../io/cc2420/cc2420controlm.h"

#include "../kernel/threadmodel.h"


#ifdef PLATFORM_CPU_MEASURE 

  extern uint32_t cpucounter_history[20];
  extern uint8_t loop; 
  extern uint32_t cpucounter;

#endif 
/*Radio_Msg datamsg; 

typedef struct {
	uint16_t index;
	uint8_t power;
} datapayload; 

uint16_t currentcounter;
uint8_t currentpower;

bool enabled; 
*/
 
//Implementing platform related modules 
//This part assumes that LITE_MAX_THREAD is 8
void (*timercallback[8]) ();

//debugging for iris
//struct Radio_Msg Packetd;
//struct Radio_Msg *pktptr=&Packetd;


inline result_t GenericTimerInit(void)
{
    uint8_t i;

  //  currentcounter = 0;
  // currentpower = 3; 
  // enabled = true;
    for (i = 0; i < 8; i++)
    {
        timercallback[i] = NULL;
    }
#ifdef PLATFORM_AVR
    return TimerM_StdControl_init();
#endif

    
}

//-------------------------------------------------------------------------
inline result_t GenericTimerStart(uint8_t id, char type, uint32_t interval)
{
#ifdef PLATFORM_AVR
    return TimerM_Timer_start(id, type, interval);
#endif
}

//-------------------------------------------------------------------------
inline result_t GenericTimerStop(uint8_t id)
{
#ifdef PLATFORM_AVR
    return TimerM_Timer_stop(id);
#endif
}

//-------------------------------------------------------------------------
void setTimerCallBackFunction(uint8_t currentthreadindex, uint16_t period,
                              uint16_t type, void (*fp) ())
{
    timercallback[currentthreadindex] = fp;
    GenericTimerStart(currentthreadindex + 18, type, period);
}

//-------------------------------------------------------------------------
void timercallbackinvoke(uint8_t id)
{
    uint8_t index = id - 18;

    if (timercallback[index] != NULL)
    {
        (*timercallback[index]) ();
    }
}

//This function is called from the particular implementation!
//This function also contains platform related defintions 
//0, 1, 2, AND 10, 11 are reserved. User is suggested to start with id 20. 
inline result_t GenericTimerFired(uint8_t id)
{


    unsigned char result;
    
    //0, 1, 2 for threads
    switch (id)
    {
    case 0:
        ServiceTimerFired(0);
        break;
    case 1:
        ServiceTimerFired(1);
         break;
    case 2:
        ServiceTimerFired(2);
        break;
    case 3:
        ServiceTimerFired(3);
        break;
    case 4:
        ServiceTimerFired(4);
        break;
    case 5:
        ServiceTimerFired(5);
        break;
    case 6:
        ServiceTimerFired(6);
        break;
    case 7:
        ServiceTimerFired(7);
        break;
    case 9:

#ifdef PLATFORM_CPU_MEASURE
        
		cpucounter_history[loop++] = cpucounter; 
		cpucounter = 0; 
		if (loop == 20)
		loop = 0; 
             
        
#endif


        break;
#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
    case 10:
        hplcc2420interruptm_FIFOTimer_fired();
        break;
    case 11:
        hplcc2420interruptm_CCATimer_fired();
        break;
#endif
   case 12:
        #ifdef ENERGY_SHARE_SCHEDULING
        
         {energy_manager_increase_round();   
		  postTask(thread_task, 9);  
           
         }  
        #endif 		
			
		
	   break;  	  
    
	case 13:
        #ifdef TRACE_MEMORY_CONTENTS
          memoryReportTimerFired();
        #endif 
		
	    break; 
		
	case 14:
	    #ifdef TRACE_ENABLE
		 reportTrace();
		#endif 
		
		#ifdef BASE_MODE		
		restoreRadioState();
		#endif
	    
		break; 
		
	case 15:
	
		
		break; 	
		
	case 16:
	    
		#ifdef TRACE_MEMORY_CONTENTS
		  AMStandard_releaseLock();
		  lib_radio_set_channel(21); 
		 #endif 
		break;
		
		
    default:
        timercallbackinvoke(id);
        result = SUCCESS;
    }
    return SUCCESS;
}
