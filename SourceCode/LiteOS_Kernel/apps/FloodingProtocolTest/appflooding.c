#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libadc.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libserial.h"
#include "../../libraries/libfile.h"
#include "../../libraries/libsystem.h"

uint8_t msg[20];
uint8_t appfloodingbuffer[200]; 
uint8_t fromnode;
uint8_t tonode; 
uint16_t nodeid; 
uint16_t sig; 
uint16_t incomingnode; 
uint8_t floodingcounter; 


//Uses port 90. Basically the port numbers will increase in different protocols. 

void appflooding()
{
  fromnode = 2; 
  __asm__ __volatile__("sei" ::); 
 
  nodeid = lib_get_node_id();
  while (1)
 	{
      if (nodeid == fromnode)
      {    
      sig = lib_random();
      msg[0] = nodeid%256;
	  msg[1] = nodeid/256;
	  msg[2] = sig%256;
	  msg[3] = sig/256;
	  msg[4] = 90; 
	  msg[5] = 0xff;
	  msg[6] = 0xff;
	  msg[7] = 'h';
	  msg[8] = 'e';
	  msg[9] = 'l';
	  msg[10] = 'l';
	  msg[11] = 'o';
	  msg[12] = floodingcounter++; 
	 
	  lib_radio_send_msg(20, 0, 16, msg);
	  
	   if (floodingcounter%3==0)
	   { lib_red_on();
		 lib_green_off();
		 lib_yellow_off();
	   }
	   else if (floodingcounter%3==1)
	    {lib_red_off();
		 lib_green_on();
		 lib_yellow_off();
	    }
		else 
	   {
		 lib_red_off();
		 lib_green_off();
		 lib_yellow_on();
	   }				
	   
      sleepThread(2000);
      }
    else 
 	  {
	  lib_radio_receive(90, 16, msg); 
      incomingnode = (uint16_t)msg[0]+ (((uint16_t)msg[1]) *256); 
	  floodingcounter = msg[12]; 
	  if (floodingcounter%3==0)
	   { lib_red_on();
		 lib_green_off();
		 lib_yellow_off();
	   }
	   else if (floodingcounter%3==1)
	    {lib_red_off();
		 lib_green_on();
		 lib_yellow_off();
	    }
		else 
	   {
		 lib_red_off();
		 lib_green_off();
		 lib_yellow_on();
	   }				
	  }
	
	  
  	}	 
	  
	//should never reach here.
	return; 
}

