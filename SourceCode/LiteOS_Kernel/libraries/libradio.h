/** @file libradio.h
       @brief The functional prototypes for the radio API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/



#ifndef LIBRADIOH
#define LIBRADIOH

#include "../types/types.h"
#include "libradio.h"
#include "libsystem.h"
#include "liteoscommon.h"
#include "libstring.h"
#include "libthread.h"


/** @addtogroup api
*/

/** @{
*/

/**  @brief Get the internal radio handle address.
        @return The address of radio handle. 

 */

radiohandletype *lib_get_current_radio_receive_handle_addr();



/**  @brief Send radio message. 
        @param port The port number of the radio sending operation.
        @param address The next hop address.
        @param length The length of the message to be sent out.
        @param msg The start address of the message to be sent out.
        @return Void. 

 */


void lib_radio_send_msg(uint16_t port, uint16_t address, uint8_t length, uint8_t *msg);


/**  @brief Send radio message. 
        @param msg The radio string. 
        @return Void. 

 */


void lib_radio_send_string(uint8_t *msg);


/**  @brief Send radio message containing an integer. 
        @param value The integer value. 
        @return Void. 

 */

void lib_radio_send_integer_u16(uint16_t value);



/**  @brief Receive a radio message.  
        @param port The port number.
        @param maxlength The maximum receive length.
        @param msg The message buffer. 
        @return Void. 
*/
int lib_radio_receive(uint16_t port, uint8_t maxlength, uint8_t *msg);


/**  @brief Receive a radio message within time.
        @param port The port number.
        @param maxlength The maximum receive length.
        @param msg The message buffer. 
        @param time The waiting time. 
        @return Void. 
*/


int lib_radio_receive_timed(uint16_t port, uint8_t maxlength, uint8_t *msg, uint16_t time);

/**  @brief Set radio frequency. 
        @param freq The frequency. 
        @return Void. 
*/

void lib_radio_set_freq(uint16_t freq);


/**  @brief Set radio channel. 
        @param channel The channel. 
        @return Void. 
*/

void lib_radio_set_channel(uint8_t channel);


/**  @brief Set radio power. 
        @param power The power. 
        @return Void. 
*/


void lib_radio_set_power(uint8_t power);


/**  @brief Get current radio address
        @return Radio send address. 
*/

radioinfotype *lib_get_current_radio_send_addr(); 



/**  @brief Send a radio message.
        @return Void. 
*/


void lib_radio_send_msg_syscall();

/** @} */
#endif
