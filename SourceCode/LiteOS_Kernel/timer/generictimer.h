/** @file generictimer.h
	@brief The declarations of the generic timer module. 

	@author Qing Charles Cao (cao@utk.edu)
*/


#ifndef GENERICTIMERH
#define GENERICTIMERH


#include "../types/types.h"

/** @addtogroup timer
	@{
*/
enum
{
    TIMER_REPEAT = 0, TIMER_ONE_SHOT = 1,
};

/** @brief Init the timer. 
	@return Status byte.  
*/
inline result_t GenericTimerInit(void);

/** @brief Start the timer.
	@param id The timer id. 
	@param type The timer type.
	@param interval The timer interval. 
	@return Status byte.
*/
inline result_t GenericTimerStart(uint8_t id, char type, uint32_t interval);

/** @brief Event of firing the timer. 
	@param id The id of the fired timer.
	@return The status byte. 
*/
inline result_t GenericTimerFired(uint8_t id);

/**	@brief Setting up the timer callback function.
	@param currentthreadindex The thread that calls this function.
	@param period The cycle of the timer callback function.
	@param type The type of the timer.
	@param fp The callback function pointer. 
	@return Void. 
*/
void setTimerCallBackFunction(uint8_t currentthreadindex, uint16_t period,
                              uint16_t type, void (*fp) ());

/**	@brief Stop a generic timer.
	@param id The id of the stopped timer.
	@return The status byte. 
*/
inline result_t GenericTimerStop(uint8_t id);

/** @} */
#endif
