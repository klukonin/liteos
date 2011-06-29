/**  @file threadtools.h
        @brief The header for the thread thread tools module. 

        @author Qing Charles Cao (cao@utk.edu)
*/


#ifndef THREADTOOLSH
#define THREADTOOLSH

#include "threaddata.h"

/** @addtogroup scheduling */

/** @{ */
/** @brief Conflict detection.
	@param createflashromstart Rom start. 
	@param createflashromsize Rom size. 
	@param ramstackstart The start of the ram.
	@param ramstackend The end of the ram. 
	@return Whether there is conflict. 
*/
uint8_t memory_conflict_detect(uint16_t createflashromstart, uint16_t
  createflashromsize, uint16_t ramstackstart, uint16_t ramstackend);


/** @brief Check memory corrupt. 
	@param i The thread id. 
	@return Whethere there is corrupt. 
*/
int check_for_memory_corrupt(int i);

/** @brief Service timer fire. 
	@param id The index of the fire.
	@return Void. 
*/
void ServiceTimerFired(uint8_t id);

/** @brief Get kernel stack address.
	@return Void pointer to the address. 
*/
void **getKernelStackAddress();

/** @brief Get thread index address.
	@return Thread index address. 
*/
int getThreadIndexAddress();

/**	@brief Unblock a barrier. 
	@param type The type of the barrier. 
	@param id The id of the barrier.
	@return Void. 
*/
void barrier_unblock(uint8_t type, uint8_t id); 

/** @} */
 
#endif
