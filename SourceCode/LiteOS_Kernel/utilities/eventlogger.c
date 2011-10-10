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

uint32_t time;

volatile int indexofrecords; 
event_record records[2][64];

uint8_t prevcount, endingcount; 

uint16_t interval; 

Radio_Msg eventmsg; 

uint32_t prevtiming; 

void initTrace(){
	
	//Tune the channel and power	
	prevcount = endingcount = 0; 
	prevtiming = 0; 
	indexofrecords = 0; 
}


//void addTrace(uint16_t eventid, uint8_t threadid){}
	
	
void addTrace(uint16_t eventid, uint8_t threadid){
	
	
	
	//bug fix: sometmies r20 and r21 are being used. So they must be saved and restored. 
	
	 asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
	time = getCurrentResolution();
	
	#ifdef VER_DEBUG
	{
		
		if (time > prevtiming*10)
		{
		 time = getCurrentResolution();			
		}
		
		if (eventid > 1000)
		{
		 time = getCurrentResolution();			
		}

        

	}
	
		
	prevtiming = time; 
	#endif 
	records[indexofrecords][endingcount].id = threadid; 
	records[indexofrecords][endingcount].timestamp = time;
	records[indexofrecords][endingcount].type = eventid; 
	endingcount ++;
	if (endingcount == 64)
	 {endingcount = 0; 
	  indexofrecords = 1-indexofrecords; 
	  postTask(reportTrace, 10); 
	 }	 
	 
   asm volatile("pop r21" "\n\t"
                "pop r20" "\n\t"
                ::);
}


void reportTrace()
{
   	 
    uint8_t *p; 
     
	 
	p = (uint8_t*)eventmsg.data;
    *p = prevcount; 
	p++; 
	
    mystrncpy((char *)p, (unsigned char *)&records[1-indexofrecords][prevcount],  64);
			   
	AMStandard_SendMsg_send(10, 0xFFFF, 65, &eventmsg);
	
	if (prevcount + 8 < 64)
		{
        prevcount += 8;
		GenericTimerStart(14, TIMER_ONE_SHOT, 20);
		}
	else
	   prevcount = 0; 	 
	 
}

#endif 