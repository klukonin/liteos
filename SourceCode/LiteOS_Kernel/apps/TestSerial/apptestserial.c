/** @file apptestserial.c
	@brief The functional implementation of testing serial port. 

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
#include "../../libraries/commonapp.h"

uint8_t printserialbuffer[250]; 
volatile uint8_t i; 

uint32_t counter;

void printserialmain()
{
	 

   for (i =0; i< 1000; i++)
   	{
     counter = get_current_timestamp();
	 lib_red_toggle(); 
     lib_printf_string("This is a message\n");
     lib_printf_uinteger32(counter);
     lib_printf_ln();
	 lib_sleep_thread(1000);
    }
   
	return; 
}

