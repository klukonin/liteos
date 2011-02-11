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

uint8_t hellobuffer[250]; 
char *string = "hello, world"; 

volatile uint16_t countersum; 


static uint8_t buffersum[MAX_MSG_LENGTH];

int hello()
{
         
	uint16_t *p;
  countersum = 0; 
  
	while (1)
	{
   
  p = (uint16_t *)&buffersum[0];
   *p = countersum;   
  lib_yellow_toggle(); 
  lib_radio_send_msg(10, 0xffff, 6, buffersum); 
  //lib_radio_send_integer_u16(countersum);
  countersum++; 
  lib_sleep_thread(100);
	}
	return 0; 
}


