/** @file socketradiocontrol.h
	@brief The functional prototypes for socket radio control.

	@author Qing Cao (cao@utk.edu)
*/

#ifndef SOCKETRADIOCONTROLH
#define SOCKETRADIOCONTROLH

#include "../io/cc2420/cc2420controlm.h"
#include "../types/types.h"
#include "../syscall/socketradiodata.h"
#include "../io/radio/packethandler.h"
#include "../kernel/scheduling.h"

/** @ingroup socket
       @brief The radio information internal representation for user applications.
*/
typedef struct
{
    uint16_t socket_port;
    uint16_t socket_addr;
    uint8_t socket_msg_len;
    uint8_t *socket_msg;
} radio_sending_buffer;
 
//-------------------------------------------------------------------------

/** @ingroup socket
	@brief Set radio frequency.
	@return Void. 
*/
void setRadioFrequencyTask();
//-------------------------------------------------------------------------

/** @ingroup socket
	@brief Set radio channel.
	@return Void. 
*/

void setRadioChannelTask();
//-------------------------------------------------------------------------

/** @ingroup socket
	@brief Set radio power.
	@return Void. 
*/

void setRadioPowerTask();

//-------------------------------------------------------------------------

/** @ingroup socket
	@brief Send radio packet. 
	@return Void. 
*/

void SocketRadioSend();


/** @ingroup socket
	@brief Get sending buffer address. 
	@return Void pointer. 
*/

void *getRadioSendingBufferAddr();


/** @ingroup socket
	@brief Get receiving buffer address. 
	@return Void pointer. 
*/

void *getRadioReceivingBufferAddr();

/** @ingroup socket
	@brief Registering an event. 
	@return Void.
*/

void registerReceiverHandle_syscall();

#endif
