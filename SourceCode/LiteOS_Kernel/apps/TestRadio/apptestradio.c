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

uint8_t testradiobuffer[300]; 

char *string = "hello,world"; 

//volatile uint16_t countersum; 

static uint8_t buffersum[50];

void testradio()
{
         
	uint16_t *p;
	uint8_t i;
	
 	for (i=0;i<11;i++)
	  buffersum[i+2] = string[i]; 
	  
    buffersum[0] = 0; 
	 
	while (1)
	{
   
    lib_sleep_thread(500);
    buffersum[0]++; 
	buffersum[1] = getNodeID();
    lib_green_toggle();
	lib_get_radio_lock();
	lib_radio_set_channel(19); 
	lib_radio_send_msg(10, 0xffff, 19, buffersum); 
    lib_release_radio_lock(); 
	}
	return; 
}
