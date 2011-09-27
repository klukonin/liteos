/** @file eventlogger.c
	@brief The functional implementation of the event logger headers. 

	@author Qing Cao (cao@utk.edu)
*/



#include "eventlogger.h"
#include "../storage/filesys/fsapi.h"
#include "../timer/globaltiming.h"
#include "../timer/generictimer.h"


#ifdef TRACE_ENABLE
static uint8_t base_event_logger_freq, reporting_event_logger_freq; 

typedef struct 
{
	 uint16_t type;
	 uint16_t  id;
	 uint32_t timestamp; 
} 
event_record;

event_record records[64];

uint8_t prevcount, endingcount; 

uint16_t interval; 



void initTrace(uint8_t basechannel, uint8_t reportingchannel, uint16_t reportinterval){
	
	//Tune the channel and power	
	base_event_logger_freq = basechannel;
	reporting_event_logger_freq = reportingchannel;
	prevcount = endingcount = 0; 
	interval = reportinterval; 
	GenericTimerStart(14, TIMER_ONE_SHOT, 1000);
}

//Tracing functionality uses timer resources 14 and 15 for its purpose. Do not conflict!

void GenericInitTimerFired()
{
	GenericTimerStart(15, TIMER_REPEAT, interval);
}
//void addTrace(uint16_t eventid, uint8_t threadid){}
	
	
void addTrace(uint16_t eventid, uint8_t threadid){
	
	uint32_t time;
	time = getCurrentResolution();
	records[endingcount].id = threadid; 
	records[endingcount].timestamp = time;
	records[endingcount].type = eventid; 
	endingcount ++;
	if (endingcount == 64)
	 endingcount = 0; 
	
}


void reportTrace()
{
   	 
     
     
	if (prevcount == endingcount)
	 return; 
	 
	 
	AMStandard_SendMsg_send(10, 0xFFFF, 64, &records[prevcount]);
	
	if (prevcount + 8 <= endingcount)
		prevcount += 8;
	else
	if ((prevcount + 8 >endingcount )&& (endingcount < prevcount))
		{
		  if (prevcount +8 >=64)
			prevcount = 0;
			else
			 prevcount+= 8; 			
		}
	else
	 if ((prevcount + 8 > endingcount)&& (endingcount > prevcount))
	  prevcount = endingcount; 
	
	 
	 
}

#endif 