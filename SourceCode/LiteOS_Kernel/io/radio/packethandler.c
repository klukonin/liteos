/**
   @file packethandler.c
   @brief Implementatio for standard sockdet for receiving packets. 
   
*  @author Qing Charles Cao (cao@utk.edu)
*/

 
#include "../../kernel/scheduling.h"
#include "packethandler.h"

 
 

//the handles definition
radio_receiving_buffer receivehandles[RECEIVE_HANDLE_NUM];

Radio_MsgPtr debug; 

//init all handles by setting their handle valid indicator as 0
void initRadioHandle()
{
    uint8_t i;

    for (i = 0; i < RECEIVE_HANDLE_NUM; i++)
    {
        receivehandles[i].handlevalid = 0;
    }
	 
}

//Register A New Handle
//supply the following :
//the port listening to
//The max length of the data
//The dataready byte for the actual number of bytes receicved 
//The data for the received packet
//packetinfo for storing the received packet info on rssi and lqi 
//handle function to be posted 
void registerEvent(uint16_t port, uint8_t maxLength, uint8_t * dataReady,
                   uint8_t * data, uint8_t * packetinfo,
                   void (*handlefunc) (void))
{
    uint8_t i;

    for (i = 0; i < RECEIVE_HANDLE_NUM; i++)
        if (receivehandles[i].handlevalid == 0)
        {
            receivehandles[i].port = port;
            receivehandles[i].maxLength = maxLength;
            receivehandles[i].dataReady = dataReady;
            receivehandles[i].data = data;
            receivehandles[i].packetinfo = packetinfo;
            receivehandles[i].handlefunc = handlefunc;
            receivehandles[i].handlevalid = 1;
            return;
        }
}

//-------------------------------------------------------------------------
void deRegisterEvent(uint16_t port)
{
    uint8_t i;

    for (i = 0; i < RECEIVE_HANDLE_NUM; i++)
        if ((receivehandles[i].handlevalid == 1) && (receivehandles[i].port
                                                     == port))
        {
            receivehandles[i].handlevalid = 0;
            receivehandles[i].port = 0;
            return;
        }
}


 


//Now this OS has a new packet, needs to deliver it to the correct thread for processing, and return the packet as soon as possible 
Radio_MsgPtr Standard_Receive_Packet(uint16_t port, Radio_MsgPtr packet)
{
    uint8_t i;

  
		 
    for (i = 0; i < RECEIVE_HANDLE_NUM; i++)
    {
		debug = packet; 
        if ((receivehandles[i].port == port)
            && (receivehandles[i].handlevalid == 1))
        {
            uint8_t temp;
            uint8_t j;
            uint8_t *buf;

            buf = (uint8_t *) receivehandles[i].data;
            temp =
                (packet->length >
                 (receivehandles[i].maxLength) ? (receivehandles[i].
                                                  maxLength) : packet->length);
            for (j = 0; j < temp; j++)
            {
                *buf = packet->data[j];
                buf++;
            }
            if (receivehandles[i].packetinfo != NULL)
            {
                buf = (uint8_t *) receivehandles[i].packetinfo;
                *buf = packet->strength;
                buf++;
                *buf = packet->lqi;
            }
            if (receivehandles[i].handlefunc != NULL)
            {
                postTask(receivehandles[i].handlefunc, 6);
            }
            //if (*(receivehandles[i].dataReady) == 0) 
            *(receivehandles[i].dataReady) = temp;
            return packet;
        }
    }
    return packet;
}

//-------------------------------------------------------------------------
void deleteThreadRegistrationInReceiverHandles(uint8_t * start, uint8_t * end)
{
    uint8_t i;

    for (i = 0; i < RECEIVE_HANDLE_NUM; i++)
    {
        if ((receivehandles[i].handlevalid == 1)
            && (receivehandles[i].dataReady <= end)
            && (receivehandles[i].dataReady >= start))
        {
            receivehandles[i].handlevalid = 0;
            receivehandles[i].port = 0;
        }
    }
}
