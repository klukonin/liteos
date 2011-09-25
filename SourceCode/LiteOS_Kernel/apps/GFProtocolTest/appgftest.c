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

uint8_t gftestbuffer[100];

uint8_t msg[20];
uint8_t fromnode;
uint8_t tonode; 


volatile uint16_t counter; 

void gftest()
{
 uint16_t nodeid; 
 uint16_t *p;
 
 uint8_t destx, desty; 
 counter = 0; 
 
 fromnode= 1;
 tonode = 5;
 nodeid = lib_get_node_id();
 counter = 0; 
 lib_sleep_thread(10000);

  
 while (1)
  	{
  	 if (nodeid == fromnode)
      {    

            destx = tonode%256;          
			desty = tonode/256;
			counter++; 
			msg[0] = destx;
			msg[1] = desty; 
			msg[2] = fromnode; 
			msg[3] = tonode;
			msg[4] = 80;
			msg[5] = 0xff;
			msg[6] = 0xff;
			msg[7] = counter; 
		
			msg[8] = 'h';
			msg[9] = 'e';
			msg[10] = 'l';
			msg[11] = 'l';
			msg[12] = 'o';
			msg[13] = 0;
			
			if (counter<=300)
			   { lib_radio_send_msg(10, 0, 16, msg);
			//lib_red_toggle(); 
			//lib_yellow_toggle(); 
        } 
      }
	else if (nodeid == tonode)
	  {
			lib_radio_receive(80, 16, msg); 
		  //  lib(16, msg); 
			//lib_red_toggle();
			lib_red_toggle();
				
	  }
	  lib_sleep_thread(10000);
  	}	 
	return; 
}
