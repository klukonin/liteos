/**
   @file packethandler.h
   @brief Standard sockdet for receiving packets. 
   
*  @author Qing Charles Cao (cao@utk.edu)
*/


#ifndef PACKETHANDLERH
#define PACKETHANDLERH
#include "amcommon.h"
#include "amradio.h"
//For the receving handles. Whenever a packet is received, content of this vector is compared and executed

/** @addtogroup radio
*/
/** @{ */

enum
{
    RECEIVE_HANDLE_NUM = 5,
};


/** @brief The packet receiving buffer. */
 typedef struct 
{
    uint16_t port;
    uint8_t maxLength;
    uint8_t *dataReady;
    uint8_t *data;
    uint8_t *packetinfo;
    uint8_t handlevalid;
    void (*handlefunc) (void);
} radio_receiving_buffer;



/** @brief Init the radio handle data structure.
	@return Void.
*/
void initRadioHandle();

/** @brief Allow the user appliation to register a port by providing necessary info.
	@param port The port number the application listening to.
	@param maxLength The maximum number of bytes provided in RAM for the incoming packet.
	@param dataReady The actual number of bytes will be received. No more than maxLength.
	@param packetinfo The packet lqi and rssi.
	@param data The actual storage for the data packet.
	@param handlefunc The task to be called once the packet comes.
	@return Void. 
*/
void registerEvent(uint16_t port, uint8_t maxLength, uint8_t * dataReady,
                   uint8_t * data, uint8_t * packetinfo,
                   void (*handlefunc) (void));

/** @brief Allow the user appliation to register a port by providing necessary info.
	@param port The previous port listening to.
	@return Void. 
*/
void deRegisterEvent(uint16_t port);

/** @brief  Called by the Received() which is the called by the coming packet trigger. This is NOT part of the user API.
	@param port The port number.
	@param packet The packet. 
	@return The received pacekt. 
	
 */
Radio_MsgPtr Standard_Receive_Packet(uint16_t port, Radio_MsgPtr packet);


/**	@brief Delete all receive handles.
	@param start The starting address.
	@param end The ending address.
	@return Void. 
*/

void deleteThreadRegistrationInReceiverHandles(uint8_t * start, uint8_t * end);

/** @} */
#endif
