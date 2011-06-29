/** @file libleds.h
       @brief The functional prototypes for the Leds API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/

#ifndef LIBLEDSH
#define LIBLEDSH

/** @addtogroup api
*/

/** @{
*/


/** @brief Toggle the green LED. 
       @return Void.
*/

void lib_green_toggle();

/** @brief Toggle the red LED. 
       @return Void.
*/

void lib_red_toggle();


/** @brief Toggle the yellow LED. 
       @return Void.
*/

void lib_yellow_toggle();

/** @brief Turn on the green LED. 
       @return Void.
*/


void lib_green_on();

/** @brief Turn off the green LED. 
       @return Void.
*/

void lib_green_off();

/** @brief Turn on the red LED. 
       @return Void.
*/

void lib_red_on();

/** @brief Turn off the red LED. 
       @return Void.
*/


void lib_red_off();

/** @brief Turn on the yellow LED. 
       @return Void.
*/



void lib_yellow_on();

/** @brief Turn off the yellow LED. 
       @return Void.
*/

void lib_yellow_off();


/** @} */

#endif
