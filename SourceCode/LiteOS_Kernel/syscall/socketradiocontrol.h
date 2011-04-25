#ifndef RADIOCONTROLH
#define RADIOCONTROLH

#include "../io/cc2420/cc2420controlm.h"
#include "../types/types.h"
#include "../io/radio/socket.h"
#include "../io/radio/packethandler.h"
#include "../kernel/scheduling.h"

/** \ingroup radio 
The radio information internal representation for user applications 
*/
typedef struct
{
    uint16_t socket_port;
    uint16_t socket_addr;
    uint8_t socket_msg_len;
    uint8_t *socket_msg;
} radio_sending_buffer;
 






//-------------------------------------------------------------------------
void setRadioFrequencyTask();
//-------------------------------------------------------------------------
void setRadioChannelTask();
//-------------------------------------------------------------------------
void setRadioPowerTask();

//-------------------------------------------------------------------------
void SocketRadioSend();

void *getRadioSendingBufferAddr();

void *getRadioReceivingBufferAddr();

//system call interface for registering an event 
void registerReceiverHandle_syscall();

#endif
