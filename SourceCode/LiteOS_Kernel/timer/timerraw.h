/** @file timerraw.h
	@brief The declarations of the raw timer module on top of clock. 

	@author Qing Charles Cao (cao@utk.edu)
*/


#ifndef TIMERH
#define TIMERH
#include "../types/types.h"

/** @addtogroup timer */

/** @{ */
enum
{
    NUM_TIMERS = 20
};


enum
{
    DEFAULT_SCALE = 3, DEFAULT_INTERVAL = 127
};

/** @brief Init the timer module. 
	@return Status byte. 
*/
inline result_t TimerM_StdControl_init(void);

/** @brief Start the timer module. 
	@param id The timer id.
	@param type The timer type.
	@param interval The timer interval. 
	@return Status byte. 
*/
inline result_t TimerM_Timer_start(uint8_t id, char type, uint32_t interval);

/** @brief Set timer rate. 
	@param interval The interval of the setting operation.
	@param scale The scale of the setting operation.
	@return Status byte.
*/
inline result_t TimerM_Clock_setRate(char interval, char scale);


/** @brief Adjust timer interval.
	@return Void.
*/
void TimerM_adjustInterval(void);

/** @brief enqueue operation for the timer data structure.
	@return Void.
*/
inline void TimerM_enqueue(uint8_t value);

/** @brief The default fire operation of the timer. 
	@return Status byte.
*/
inline result_t TimerM_Timer_default_fired(uint8_t id);

/** @brief The event of the fire operation. 
	@return Status byte.
*/
inline result_t TimerM_Timer_fired(uint8_t id);

/** @brief The dequeue operation of the timer.
	@return Status byte.
*/
inline uint8_t TimerM_dequeue(void);

/** @brief Signal timer once in the timerM module. 
	@return Void.
*/
inline void TimerM_signalOneTimer(void);

/** @brief Handling the fire event.
	@return Void.
*/
inline void TimerM_HandleFire(void);

/** @brief Fire event of the clock. 
	@return Status byte.
*/

inline result_t TimerM_Clock_fire(void);

/** @brief The start of the timer control.
	@return Status byte.
*/
inline result_t TimerM_StdControl_start(void);

/** @brief Read the timer counter.
	@return The current counter. 
*/
inline uint8_t TimerM_Clock_readCounter(void);

/** @brief Get the interval of the timer. 
	@return The current interval.
*/
inline uint8_t TimerM_Clock_getInterval(void);

/** @brief Set the interval of the timer. 
	@return Void.
*/
inline void TimerM_Clock_setInterval(uint8_t interval);

/** @brief Stop the timer. 
	@return Status byte. 
*/
inline uint8_t TimerM_Timer_stop(uint8_t id);

/** @} */
#endif
