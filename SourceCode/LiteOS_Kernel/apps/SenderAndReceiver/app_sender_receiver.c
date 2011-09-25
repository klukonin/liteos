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

uint16_t thisnodeid;
uint32_t counter; 

uint8_t protocolbuffer[300];

//Pointer to the current thread data structure. Useful for waking up the current thread. 
lib_thread **thiscurrentthread;
lib_thread *currentthread;
 
//-------------------------------------------------------------------------
static uint8_t incomingMsg[32];
static uint8_t incomingDataLength;
static uint8_t incomingPacketInfo[4];

  

//handles the incoming message from a neighbor. This usually scanns the neighborhood table and sets up stuff 
void wakeupMeOnDataMsg()
{
    int i;
    lib_green_toggle();
	counter = get_current_timestamp();
	lib_printf_string("A message is received, first three bytes: ");
	lib_printf_uinteger32(incomingMsg[0]);
	lib_printf_uinteger32(incomingMsg[1]);
	lib_printf_uinteger32(incomingMsg[2]);
    lib_printf_ln();
    return;
}

//-------------------------------------------------------------------------
void registerIncomingDataMsg()
{
    _atomic_t currentatomic;
    radiohandletype *radiohandleaddr;
    void (*getaddrfp) (void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;

    radiohandleaddr = lib_get_current_radio_receive_handle_addr();
    incomingDataLength = 0;
    //set up the radiohandleaddr data structures
    radiohandleaddr->port = 10;
    radiohandleaddr->maxLength = 32;
    radiohandleaddr->dataReady = &incomingDataLength;
    radiohandleaddr->data = incomingMsg;
    radiohandleaddr->packetinfo = incomingPacketInfo;
    radiohandleaddr->handlefunc = wakeupMeOnDataMsg;
    //close the interrupt     
    currentatomic = _atomic_start();
    //call the radio handle set to store the data structure into the handle vectors 
    getaddrfp();
    //open the interrupt 
    _atomic_end(currentatomic);
    return;
}

 
//Assume two nodes, one programmed with ID of 1, and the second programmed with ID of 2. 

void protocol()
{
     
    thisnodeid = lib_get_node_id();
    thiscurrentthread = lib_get_current_thread();
    currentthread = *thiscurrentthread;
  
    
    while (1)
    {
		lib_sleep_thread(1000); 
        if (thisnodeid == 1)         
        {
              int i;
			  
			  for (i=0; i<32; i++)
			  incomingMsg[i] = i; 	
			  
			  incomingDataLength = 32; 
			  lib_get_radio_lock();
			  lib_radio_send_msg(10, 2, incomingDataLength, incomingMsg);
		      lib_release_radio_lock();
			  lib_red_toggle();
        }
        else
        {
              registerIncomingDataMsg(); 
			 
        }
    }
}
