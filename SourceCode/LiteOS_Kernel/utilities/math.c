/** @file math.c 
	@brief The detailed implementation of math functions.

	@author Qing Charles Cao (cao@utk.edu)
*/

#include "math.h"
uint16_t getRandomNumber()
{
    uint16_t ret;

#ifdef PLATFORM_AVR
    ret = rand();
#endif
    return ret;
}
