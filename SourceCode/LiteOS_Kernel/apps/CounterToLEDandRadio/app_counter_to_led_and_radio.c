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

uint8_t testcounterbuffer[300]; 

//volatile uint16_t countersum; 

uint16_t counter; 

uint16_t buffersum[4];

void testcounter()
{
         
	
	
	uint8_t ledcounter; 
	counter = 0;
	 
	
 	
	 
	while (1)
	{
   
    lib_sleep_thread(500);
    
	buffersum[0] = getNodeID();
    
	lib_get_radio_lock();
	lib_radio_set_channel(19); 
	buffersum[1] = counter; 
	lib_radio_send_msg(0xffff, 0xffff, 8, (uint8_t *)buffersum); 
    lib_release_radio_lock(); 
	counter++;
	
	
	if (counter&0x1)
	lib_red_on();
	else
	lib_red_off();
	
	if (counter&0x2)
	lib_green_on();
	else
	lib_green_off();

    if (counter&0x04)
	lib_yellow_on();
	else
	lib_yellow_off();	
	
	}	
	
	return; 
}
