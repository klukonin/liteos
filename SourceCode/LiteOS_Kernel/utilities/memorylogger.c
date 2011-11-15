/** @file memorylogger.c
	@brief The functional implementation of the memory reporting service. 

	@author Qing Cao (cao@utk.edu)
*/



#include "memorylogger.h"
#include "../libraries/commonapp.h"
#include "../storage/filesys/fsapi.h"
#include "../timer/globaltiming.h"
#include "../timer/generictimer.h"
#include "../io/radio/amcommon.h"
#include "../types/types.h"
#include "../sensors/leds.h"
#include "../io/serial/stdserial.h"

#ifdef TRACE_MEMORY_CONTENTS


static uint16_t totalround; 

 
	 
   

#ifdef PLATFORM_AVR_MICAZ
static int8_t counter_memory_report = 50;
static int8_t counter_memory_init = 50;
static uint16_t total_report = 4000; 
#endif

#ifdef PLATFORM_AVR_IRIS
static int8_t counter_memory_report = 100;
static int8_t counter_memory_init = 100;
static uint16_t total_report = 8000; 
#endif
 

void initMemoryReporting(uint16_t reportinterval){
	
	//Tune the channel and power	
	totalround = 0; 	
     
	GenericTimerStart(13, TIMER_REPEAT, reportinterval);
}



void memoryReportTimerFired()
{
   	 
      uint16_t *p;
	  
	  lib_green_on();
	  lib_red_on();
	  lib_yellow_on(); 

	  totalround++; 
	 
	 // printfuinteger32(totalround);
	  printfmemory((volatile unsigned char *)(0x100),
              8000);

      lib_green_off();
	  lib_red_off();
	  lib_yellow_off();      
	return; 	 
}
 
 
#endif