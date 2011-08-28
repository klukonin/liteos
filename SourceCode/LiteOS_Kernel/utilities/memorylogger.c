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


#ifdef TRACE_MEMORY_CONTENTS

Radio_Msg currentMemoryMsg; 
static uint16_t totalround; 

 
	 
   

static uint8_t base_memory_logger_freq, reporting_memory_logger_freq; 

static int8_t counter_memory_report = 50; 

void initMemoryReporting(uint8_t basechannel, uint8_t reportingchannel, uint16_t reportinterval){
	
	//Tune the channel and power	
	base_memory_logger_freq = basechannel;
	reporting_memory_logger_freq = reportingchannel;
	totalround = 0; 	
     
	GenericTimerStart(12, TIMER_REPEAT, reportinterval);
}

void memoryInitTimerFired()
{
	totalround++; 
	GenericTimerStart(13, TIMER_ONE_SHOT, 50);
}
//void addTrace(uint16_t eventid, uint8_t threadid){}
	
	 


void memoryReportTimerFired()
{
   	 
      uint16_t *p;
	  
	  
	  
	  uint16_t index = 4000 - counter_memory_report*80; 
	  
	  _atomic_t flag;
	   
	  uint16_t lock; 
	  
	  flag = _atomic_start();
	  
	  lock = AMStandard_getLock();
	  if (lock == 1)
	   {
	    _atomic_end(flag);
		GenericTimerStart(13, TIMER_ONE_SHOT, 50); 
		return; 		   
	   }
	  
	   AMStandard_setLock();
	  _atomic_end(flag);
      
	  
	  if (counter_memory_report > 0)
	    counter_memory_report--;
	  else
	   {
		   counter_memory_report = 50;
		   AMStandard_releaseLock();
		   return; 
	   }
	 
	p = (uint16_t*)currentMemoryMsg.data;
	*p = totalround;
	p++;
	*p = index; 
	p++; 
	
	
    mystrncpy((char *)p, (volatile unsigned char *)(0x100 + index),
              80);
    AMStandard_TuneChannel(19); 
    AMStandard_SendMsg_send(10, 0xFFFF, 84, &currentMemoryMsg);
	GenericTimerStart(13, TIMER_ONE_SHOT, 50); 
	GenericTimerStart(16, TIMER_ONE_SHOT, 20); 
	
	return; 	 
}
 
 
#endif