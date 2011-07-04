/** @file appblink.c
	@brief The functional implementation of blink. 

	@author Qing Cao (cao@utk.edu)
*/



#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../types/types.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libfile.h"
 
uint8_t blinkbuffer[200]; 

uint16_t counter = 0; 

void blink()
{
	while (1)
	{
   lib_sleep_thread(500);  
   lib_red_toggle();
   
   counter++; 
  }
  
  return; 
}



