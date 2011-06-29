/** @file libmutex.h
       @brief The functional prototypes for the mutex API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/


#ifndef MUTEXH
#define MUTEXH

#include "liteoscommon.h"

/** @addtogroup api
*/

/** @{
*/


/** @brief Block at a barrier.
	@param type The type of the barrier. 
	@param id The id of the barrier.
	@return Void. 
*/
void lib_file_barrier_block(uint8_t type, uint8_t id);

/** @}
*/

#endif 
