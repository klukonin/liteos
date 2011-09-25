/**
   @file amradio.c
   @brief Standard radio implementations. 
   
*  @author Qing Charles Cao (cao@utk.edu)
*/


#include "../../kernel/scheduling.h"
#include "amcommon.h"
#include "amradio.h"
#include "../../timer/generictimer.h"
#include "../../types/byteorder.h"
#include "../../types/types.h"
#include "packethandler.h"
#include "../../config/nodeconfig.h"




#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
#include "../cc2420/cc2420const.h"
#include "../cc2420/cc2420controlm.h"
#include "../cc2420/cc2420radiom.h"
#include "../cc2420/hplcc2420fifom.h"
#include "../cc2420/hplcc2420interruptm.h"
#include "../cc2420/hplcc2420m.h"
#include "../cc2420/hpltimer1.h"
#endif

#if defined(PLATFORM_AVR) && defined(RADIO_RF230)
#include "../rf230/rf230radiom.h"
#endif



bool AMStandard_state;
Radio_MsgPtr AMStandard_buffer;
 
uint16_t AMStandard_receive_counter;

volatile uint16_t global_radio_lock; 

#ifdef VER_DEBUG
Radio_MsgPtr temp; 
#endif 

//-------------------------------------------------------------------------
inline bool AMStandard_Control_init(void)
{
    result_t ok2;
	ok2 = 0; 
    global_radio_lock = 0; 

#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
    ok2 = cc2420radiom_SplitControl_init();
#endif

#if defined(PLATFORM_AVR) && defined(RADIO_RF230)
    ok2 = trx_init();
#endif 

    AMStandard_state = FALSE;
 
    AMStandard_receive_counter = 0;
    return ok2;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_socketradiocontrol_start(void)
{
    unsigned char result;
	result = 0; 

#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
    result = cc2420radiom_StdControl_start();
#endif
    return result;
}

//-------------------------------------------------------------------------
inline bool AMStandard_Control_start(void)
{
    result_t ok2 = AMStandard_socketradiocontrol_start();

    AMStandard_state = FALSE;
    return ok2;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_RadioSend_send(Radio_MsgPtr arg_0xa3c31f8)
{
    unsigned char result;

    //Every time send, restore first. 
    
    restoreRadioState();
    
#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
    result = cc2420radiom_Send_send(arg_0xa3c31f8);
#endif

#if defined(PLATFORM_AVR)&& defined(RADIO_RF230)
    result = rf230radio_Send_send(arg_0xa3c31f8);
#endif

    return result;
}

//-------------------------------------------------------------------------
inline void AMStandard_sendTask(void)
{
    result_t ok;
    Radio_MsgPtr buf;

    buf = AMStandard_buffer;
    ok = AMStandard_RadioSend_send(buf);
    if (ok == FAIL)
    {
        AMStandard_reportSendDone(AMStandard_buffer, FAIL);
    }
}

							 

								 
//addr means the current broadcast address, et. id is the port 
//Send out a message and takes a while to complete 
result_t AMStandard_SendMsg_send(uint16_t port, uint16_t addr, uint8_t length,
                                 Radio_MsgPtr data)
{
#ifdef TRACE_ENABLE
#ifdef TRACE_ENABLE_RADIOEVENT
    addTrace(TRACE_RADIOEVENT_SENDPACKET, 100);
#endif
#endif
    if (!AMStandard_state)
    {
        AMStandard_state = TRUE;
        if (length > DATA_LENGTH)
        {
            AMStandard_state = FALSE;
            return FAIL;
        }
        if (!postTask(AMStandard_sendTask, 20))
        {
            {
            }
            ;
            AMStandard_state = FALSE;
            return FAIL;
        }
        else
        {
            //length is the first one that means the actual data length
            //adr is the next hop id
            //type is the port
            //group is manmade result 
            AMStandard_buffer = data;
            data->length = length;
            data->addr = addr;
            data->port = port;
        }
        return SUCCESS;
    }
    return FAIL;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_SendMsg_default_sendDone(uint8_t id, Radio_MsgPtr
                                                    msg, result_t success)
{
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_SendMsg_sendDone(uint16_t arg_0xa3b8f90,
                                            Radio_MsgPtr arg_0xa31a0a0,
                                            result_t arg_0xa31a1f0)
{
    return 0;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_default_sendDone(void)
{
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_sendDone(void)
{
    unsigned char result;

    result = AMStandard_default_sendDone();
    return result;
}

//-------------------------------------------------------------------------
inline Radio_MsgPtr AMStandard_ReceiveMsg_default_receive(uint8_t id,
                                                          Radio_MsgPtr msg)
{
    return msg;
}

//Another critical function that bridges to the application 
inline Radio_MsgPtr AMStandard_ReceiveMsg_receive(uint16_t port, Radio_MsgPtr
                                                  msg)
{
    return msg;
}

//This function is really really critical to the correct behaviro of the radio stack 
//Basically it returns a radio message pointer that must be reused 
//and the content of the packet is the correct packet parsed, and is useful 
//The content starts with a length that is the actual payload length and all information are correct
//Must copy this content to the user supplied buffer, put the user action into a task, and then return this buffer to the recevie module 
Radio_MsgPtr received(Radio_MsgPtr packet)
{
    uint16_t addr = CURRENT_NODE_ID;

#ifdef TRACE_ENABLE
#ifdef TRACE_ENABLE_RADIOEVENT
    addTrace(TRACE_RADIOEVENT_RECEIVEPACKET, 100);
#endif
#endif

 
    AMStandard_receive_counter++;
	
	#if defined(PLATFORM_AVR_MICAZ)
	
    if (packet->crc == 1 && (packet->addr == BCAST_ADDRESS || packet->addr ==
                             addr))
    {
        uint16_t port = packet->port;
        Radio_MsgPtr tmp;

        tmp = Standard_Receive_Packet(port, packet);
        if (tmp)
        {
            packet = tmp;
        }
    }
	
	#elif defined(PLATFORM_AVR_IRIS)
	
	 if ((packet->addr == BCAST_ADDRESS || packet->addr ==
                             addr))
    {
        uint16_t port = packet->port;
        Radio_MsgPtr tmp;

       #ifdef VER_DEBUG
		temp = packet; 
	   #endif
        tmp = Standard_Receive_Packet(port, packet);
        if (tmp)
        {
            packet = tmp;
        }
    }
	
	#endif 
	
    return packet;
}






//-------------------------------------------------------------------------
inline Radio_MsgPtr AMStandard_RadioReceive_receive(Radio_MsgPtr packet)
{
   
   return received( packet );
   

     
}

//-------------------------------------------------------------------------
inline result_t AMStandard_RadioSend_sendDone(Radio_MsgPtr msg, result_t
                                              success)
{
    return AMStandard_reportSendDone(msg, success);
}

//-------------------------------------------------------------------------
result_t AMStandard_reportSendDone(Radio_MsgPtr msg, result_t success)
{
    AMStandard_state = FALSE;
    //  AMStandard_SendMsg_sendDone(msg->port, msg, success);
    //  AMStandard_sendDone();
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline void restoreRadioState()
{
    AMStandard_state = FALSE;
#if defined(PLATFORM_AVR) && defined (RADIO_CC2420)
    restorecc2420state();
#endif
}


//---------------------------------------------------------------------------

inline result_t AMStandard_TuneChannel(uint8_t channel)
{
	#ifdef RADIO_CC2420
	return cc2420controlm_CC2420Control_TuneChannel(channel);
    #endif 
	
	#ifdef RADIO_RF230
	return tat_set_operating_channel(channel); 
	#endif
}


//---------------------------------------------------------------------------

inline result_t AMStandard_TunePower(uint8_t powerlevel)
{
	#ifdef RADIO_CC2420
	return cc2420controlm_CC2420Control_TunePower(powerlevel);
	#endif 
}

inline uint16_t AMStandard_getLock(){
	return global_radio_lock; 
}

inline void AMStandard_setLock(){
	global_radio_lock = 1; 
}

inline void AMStandard_releaseLock(){
	global_radio_lock = 0; 
}