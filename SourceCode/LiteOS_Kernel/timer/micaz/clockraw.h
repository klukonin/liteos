/** @file clockraw.h
	@brief The declarations of the clock module. 

	@author Qing Charles Cao (cao@utk.edu)
*/

#ifndef CLOCKH
#define CLOCKH

#include "../../hardware/avrhardware.h"
#include "../../types/types.h"

/**\defgroup timer Timing related operations.

   This module defines the data structures and operations to control the hardware timers to implement clocks.
 */
 
/** @{ */

uint8_t HPLClock_set_flag;
uint8_t HPLClock_mscale;
uint8_t HPLClock_nextScale;
uint8_t HPLClock_minterval;

/** @brief This function reads the counter of the clock. 
	@return The current counter value. 
*/
inline uint8_t HPLClock_Clock_readCounter(void);

/** @brief This function gets the clock interval. 
	@return The current interval value. 
*/
inline uint8_t HPLClock_Clock_getInterval(void);

/** @brief This function is an event that occurs when the clock fires. 
	@return The status byte. 
*/
inline result_t HPLClock_Clock_fire(void);

/** @brief This function sets the interval in the clock module. 
	@return Void. 
*/
inline void HPLClock_Clock_setInterval(uint8_t value);

/** @brief This function sets the rate of the clock. 
	@return The status byte. 
*/
inline result_t HPLClock_Clock_setRate(char interval, char scale);

/** @brief This function starts timer 3 on HPL clock. 
 	@return Void. 
*/
void HPLClock_Timer3_Start();

/** @brief This function stops the timer 3 on HPL clock. 
	@return Void. 
*/
void HPLClock_Timer3_Stop();

/** @brief This function reads the high counter on the HPL clock for timing purpose. 
	@return Void.
*/
uint16_t HPLClock_readTimeCounterHigh();

/** @brief This function reads the low counter on the HPL clock for timing purpose.

	@return Void. 
*/
uint32_t HPLClock_readTimeCounterLow();

/**@} */
#endif
