/** @file socketeeprom.h
	@brief The functional declaration for socket eeprom.

	@author Qing Cao (cao@utk.edu)
*/


#ifndef SOCKETEEPROMH
#define SOCKETEEPROMH

#include "../types/types.h"


/** @addtogroup socket */
/** @{ */

/** 
	@brief The generic byte storage task structure.
*/

typedef struct
{
    uint16_t addr;
    uint16_t nBytes;
    uint8_t *buffer;
} genericByteStorageTaskNode;


/** @brief Get generic storage node address.
	@return Void pointer.
*/
void *getGenericStorageNodeAddress();

/** @brief Generic read task.
	@return Void. 
*/
void genericReadTask();

//-------------------------------------------------------------------------
/** @brief Generic write task.
	@return Void. 
*/
void genericWriteTask();


/** @} */
#endif 
