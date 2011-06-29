/** @file globaltiming.h
	@brief The declarations of the global timing module. 

	@author Qing Charles Cao (cao@utk.edu)
*/

#ifndef GLOBALTIMINGH
#define GLOBALTIMINGH
#include "../types/types.h"

/** @addtogroup timer */
/** @{  */


/**  The data structure used to store the 48 bit cycle accurate timer. */
typedef struct
{
    uint16_t counter;
    uint32_t resolution;
} currentTimeUnit;


/** @brief Starting timing. 
	@return Void. 
*/
void GenericTimingStart();


/** @brief Stopping timing. 
	@return Void. 
*/
void GenericTimingStop();


/** @brief Read the current time stamp including both the high level counter and the low level counter.

	 A total of 48 bits are read out. The 48-bit value will increase by 1 for every CPU cycle. 

	 @return Pointer to the structure for read-out values. 
*/

currentTimeUnit *getCurrentTimeStamp();

/** @brief Read the low counter which is 32 bits.
	@return The read-out value. 
*/
uint32_t getCurrentResolution();

/** @brief Read the high level counter which is 16 bits. 
	@return The read-out value. 
*/
uint16_t getCurrentCounterHigh();

/** @} */
#endif
