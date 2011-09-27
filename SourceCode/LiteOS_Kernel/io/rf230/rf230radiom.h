/** @file rf230radiom.h
	@brief The headers for rf230 radio. 
*/

 
#ifndef RF230RADIOMH
#define RF230RADIOMH

#include "../../io/radio/amcommon.h"
#include "../../types/types.h"

/** @defgroup rf230send RF230 Radio Operations. */

/** @{ */
/** @brief Send a message. 
	@param pMsg The radio message to be sent. 
*/
result_t rf230radio_Send_send(Radio_MsgPtr pMsg); 

/** @brief Init the transmission.
	@return Whether it is successful. 
*/

inline bool2 trx_init( void ); 

void restorerf230state();
/** @} */
#endif 

