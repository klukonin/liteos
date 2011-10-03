#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../libraries/liteoscommon.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libstring.h"
#include "../../libraries/libfile.h"
#include "../../libraries/libeeprom.h"
#include "../../libraries/libadc.h"
 
#include "../../types/types.h"



#define BUFFER_SIZE 10

typedef struct 
{
    uint16_t sourceMoteID;
    uint16_t lastSampleNumber;
    uint16_t channel;
    uint16_t data[BUFFER_SIZE];
} OscopeMsg;


uint8_t oscilloscopebuffer[300];


enum {
  AM_OSCOPEMSG = 10,
};


uint16_t reading;
uint16_t counternumber; 
uint16_t lastreading; 


OscopeMsg testmsg; 

int oscilloscope()
{ 

   __asm__ __volatile__("sei" ::);
  
   counternumber = 0; 
   lastreading = 0; 
   
   testmsg.sourceMoteID = lib_get_node_id();
   testmsg.channel = 11; 
   
   while (1)
   {
   	  lib_sleep_thread(125);
      reading = lib_get_light(); 	  
	  testmsg.data[counternumber++] = reading; 
	  testmsg.lastSampleNumber = lastreading;
	  lastreading++; 
	  
	  if (counternumber == BUFFER_SIZE) 
	  {
		  lib_radio_send_msg(0x7d0a, 0x007e, sizeof(OscopeMsg), (uint8_t*) &testmsg);
		  counternumber = 0; 
	  }
 	      
    }

   	
return 0;    	
}

