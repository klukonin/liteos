/** @file sounder.h
	@brief The functional prototypes for sounder driver.

	@author Qi Mi (qm8e@virginia.edu)
*/

#ifndef SounderH
#define SounderH


#include "../types/types.h"

/** \defgroup sounder Sounder driver. 
*/

/** @{ */

/** @brief Initiates the sounder. 
	@return Void.
*/
void sounder_init();

/** @brief Turn on the sounder. 
	@return Void.
*/
void sounderOn();

/** @brief Turn off the sounder.
	@return Void.
*/
void sounderOff();

/** @} */
#endif
