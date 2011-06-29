/** @file socketradiodata.h
	@brief The functional prototype of the socket radio data.

	@author Qing Cao (cao@utk.edu)
*/


#ifndef SOCKETRADIODATAH
#define SOCKETRADIODATAH
#include "../types/types.h"


/** \addtogroup socket 
\par Overview
To operate radio, LiteOS provides the following key functions to send, receive and manage radio hardware. Note that, however,
LiteOS follows a split-model where the kernel development and user side applications follow different way to invoke radio functions. This section discusses how 
such split model is organized and how different functions are organized together. 
\par Initilization
LiteOS provides simple radio initilization, by calling the SocketInit() function. 
\par Send Operations
To send a packet, use the StandardSocketSend() function. To invoke it, it must be served with port number and next hop address. If the parameter 
msglength is set as 0, then the msg will be read to find the exact number of bytes, ending with 0. If the address parameter is set as 0, then this packet is assumed 
to be local, and will be directly sent to the Standard_Receive_Packet() function for processing. 
While the kernel may directly invoke StandardSocketSend(), for the user application, it must use a different function, SocketRadioSend(). To invoke this, however, it
must first use getRadioInfo() to get the internal RadioInfoType data structure address, then populate this piece of memory, then call SocketRadioSend(). It is suggested that, 
after each send operation, the function disableSocketRadioState() should be called (this function is already called in StandardSocketSend()). 
\par Receive Operations
The LiteOS radio module implements a registering based receive structure. The following functions are provided to achieve this goal. To receive a packet, the function
initRadioHandle() must first be called to initilize the radio handle for receiving. Then, the registerEvent() function is called to reigster. The application may also call
deRegisterEvent() to stop registering for a particular packet. 
If the user application runs outside the kernel, then the function getHandleInfo() is provided to return the internal address of the receive function handle. Furthermore, 
the function registerReceiverHandle_syscall() is provided to register the handle for user side applications. 
\note the function Standard_Receive_Packet is not intended to be used by user. It is called by the radio stack to initiate a packet scheduling procedure to deliver packet to 
the corresponding handler that registers this packet.       
*/

/** @ingroup socket 
@brief The radio information internal representation for user applications.
*/

typedef struct
{
    uint16_t socket_port;
    uint16_t socket_addr;
    uint8_t socket_msg_len;
    uint8_t *socket_msg;
    
} radiosockettype;
 
extern radiosockettype radiosocketdata;


/** @ingroup socket
	@brief Standard radio send interface.
	@param port The port number.
	@param address The destination address.
	@param msglength The total length of the message.
	@param msg The message.
	@return Void. 
*/
void StandardSocketSend(uint16_t port, uint16_t address, uint8_t msglength,
                        uint8_t * msg);

/** @ingroup socket
	@brief send task
	@return Void.
*/

void send_task(); 

#endif
