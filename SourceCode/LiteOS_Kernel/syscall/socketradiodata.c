/** @file socketradiodata.c
	@brief The functional implementation of the socket radio data.

	@author Qing Cao (cao@utk.edu)
*/


#include "socketradiodata.h"

#include "../io/radio/amradio.h"
#include "../io/radio/amcommon.h"

#include "../types/string.h"
#include "../types/byteorder.h"

#include "../io/radio/packethandler.h"

#include "../kernel/scheduling.h"
#include "../kernel/threadkernel.h"

//defined as global variables so that these variables are visible to the rest components 
//These are for external usage, like system calls 
//for radio used by user threads
Radio_Msg currentMsg;

//In this part, socket_port, socket_addr specifies the next hop node information 
//msg_len specifices the message length to be broadcast over the air
//msg is the content of the data message to be broadcast 
//socket_send_action means the actual action of sending the packet is performed by the socket 

/** \ingroup radio */
radiosockettype radiosocketdata;

//-------------------------------------------------------------------------
//If the destination is another node, send the packet through radio. 
//If the destination is the current node, then send the packet locally by calling Standard_Receive_Packet 

void send_task()
{
    struct msgData *dataPayloadPtr_currentMsg;

  
	 
    dataPayloadPtr_currentMsg = (struct msgData *)currentMsg.data;
    if (radiosocketdata.socket_msg_len == 0)
    {
        radiosocketdata.socket_msg_len = mystrlen((char *)radiosocketdata.socket_msg);
    }
    mystrncpy((char *)dataPayloadPtr_currentMsg, (char *)radiosocketdata.socket_msg,
              radiosocketdata.socket_msg_len);


  
    if (radiosocketdata.socket_addr != 0)
    {
        AMStandard_SendMsg_send(radiosocketdata.socket_port, radiosocketdata.socket_addr, radiosocketdata.socket_msg_len, &currentMsg);
    }
    else
    {
        currentMsg.length = radiosocketdata.socket_msg_len;
        currentMsg.addr = radiosocketdata.socket_addr;
        currentMsg.port = radiosocketdata.socket_port;
        currentMsg.strength = 0;
        currentMsg.lqi = 0; 
        Standard_Receive_Packet(radiosocketdata.socket_port, &currentMsg);
    }
//#endif
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void StandardSocketSend(uint16_t port, uint16_t address, uint8_t msglength,
                        uint8_t * msg)
{   
//    #ifdef PLATFORM_AVR_IRIS
//     sleepThread(20);
//    #endif 

    radiosocketdata.socket_port = port;
    radiosocketdata.socket_addr = address;
    radiosocketdata.socket_msg_len = msglength;
    radiosocketdata.socket_msg = msg;
    postTask(send_task, 9);
    sleepThread(20);
    restoreRadioState();
    return;
}
 

 
 

