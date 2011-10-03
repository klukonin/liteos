/** @file eventlogger.c
	@brief The functional implementation of the event logger headers. 

	@author Qing Cao (cao@utk.edu)
*/



#include "eventlogger.h"
#include "../storage/filesys/fsapi.h"
#include "../timer/globaltiming.h"
#include "../timer/generictimer.h"
#include "../io/radio/amcommon.h"


#ifdef TRACE_ENABLE

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

Radio_Msg eventmsg; 


void initTrace(uint16_t reportinterval){
	
	//Tune the channel and power	
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
	
	//bug fix: sometmies r20 and r21 are being used. So they must be saved and restored. 
	
	 asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
	time = getCurrentResolution();
	records[endingcount].id = threadid; 
	records[endingcount].timestamp = time;
	records[endingcount].type = eventid; 
	endingcount ++;
	if (endingcount == 64)
	 endingcount = 0; 
   asm volatile("pop r21" "\n\t"
                "pop r20" "\n\t"
                ::);
}


void reportTrace()
{
   	 
    uint8_t *p; 
     
	if (prevcount == endingcount)
	 return; 
	 
	
	p = (uint8_t*)eventmsg.data;

    *p = prevcount;
	p++;
	
	*p = endingcount; 
	p++; 
	
    mystrncpy((char *)p, (unsigned char *)&records[prevcount],  64);
			   
	AMStandard_SendMsg_send(10, 0xFFFF, 64, &eventmsg);
	
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