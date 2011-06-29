/** @file math.h
	@brief The functional prototypes for math functions.

	@author Qing Charles Cao (cao@utk.edu)
*/

#ifndef MATHH
#define MATHH
#include "../types/types.h"
#if defined(PLATFORM_AVR)
#include <stdlib.h>
#endif

/**\defgroup math Mathematics.*/

/** \ingroup math 

 	@brief Returns a 16-bit random number.
 	@return 16-bit random number. 
*/
uint16_t getRandomNumber();
#endif
