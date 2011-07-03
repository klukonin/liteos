/** @file apptestradio.c
	@brief The functional implementation of "hello, world" application which sends out radio messages periodically. 

	@author Qing Cao (cao@utk.edu)
*/



#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../types/types.h"
#include "../../libraries/libradio.h"
#include "../../libraries/liteoscommon.h"
#include "../../libraries/libfile.h"
#include "../../libraries/libstring.h"
#include "../../libraries/libadc.h"

uint8_t blinkbuffer[200]; 

char *string = "hello,world"; 

volatile uint16_t countersum; 

static uint8_t buffersum[50];

void blink()
{
         
	uint16_t *p;
	uint8_t i;
	
 	for (i=0;i<11;i++)
	  buffersum[i+2] = string[i]; 
	  
    buffersum[0] = 0; 
	 
	while (1)
	{
   
    //buffersum[0]++; 
	//buffersum[1] = getNodeID();
  //*p = countersum;   
  lib_sleep_thread(500);
    lib_yellow_toggle();
	lib_radio_send_integer_u16(23);
    
	}
	return; 
}


