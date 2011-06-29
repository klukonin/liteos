/**  @file scheduling.h
        @brief The header for the scheduling module. 

        @author Qing Charles Cao (cao@utk.edu)
        
        
*/


#ifndef SCHEDULING
#define SCHEDULING
#include "../types/types.h"

/**@defgroup scheduling Thread sheduling and task scheduling. */

/** @{ */

/** @brief Init the scheduler.
	@return Void. 
*/
void initScheduling(void);

/** @brief Post a new task with a priority.
	@param tp The function pointer.
	@param priority The priority.
	@return Whether it is successful. 
*/
bool postTask(void (*tp) (void), uint8_t priority);

/** @brief Run the next task in the queue.
	@return Whether the next task exists.
*/
bool runNextTask(void);

/** @} */
#endif
