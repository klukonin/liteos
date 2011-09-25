/** @file appblink.c
	@brief The functional implementation of blink. 

	@author Qing Cao (cao@utk.edu)
*/


#include <math.h>
#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../types/types.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libfile.h"

 
uint8_t testblinkbuffer[200]; 

uint16_t counter = 0; 

volatile float test; 

void testblink()
{
 while(1)
 {
  test = 0.123; 
  for (counter =0; counter<5; counter++)
   test = test*2; 
  if (test > 10)
   lib_red_toggle(); 
 } 
  return; 
}



