/** @file Leds.h
       @brief The functional prototypes for the LED driver.

       @author Qing Charles Cao (cao@utk.edu)
       
*/

 
#ifndef LEDSH
#define LEDSH
#include "../types/types.h"

/** \defgroup leds LED driver.
*/

/** @{ */

/** @brief Initalize the Leds.
	@return Void.
*/

void Leds_Leds();

/** @brief Turn on the red LED.
	@return Void.
*/
void Leds_redOn();

/** @brief Turn off the red LED.
	@return Void.
*/
void Leds_redOff();

/** @brief Toggle the red LED. 
	@return Void.
*/
void Leds_redToggle();

/** @brief Turn on the green LED. 
	@return Void.
*/
void Leds_greenOn();

/** @brief Turn off the green LED. 
	@return Void.
*/
void Leds_greenOff();

/** @brief Toggle the green LED. 
	@return Void.
*/
void Leds_greenToggle();

/** @brief Turn on the yellow LED. 
	@return Void.
*/
void Leds_yellowOn();

/** @brief Turn off the yellow LED. 
	@return Void.
*/
void Leds_yellowOff();

/** @brief Toggle the yellow LED. 
	@return Void.
*/
void Leds_yellowToggle();

/** @} */

#endif
