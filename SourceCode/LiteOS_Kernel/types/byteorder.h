/** @file byteorder.h
	@brief The functional prototypes of byte order handling.

	@author Qing Charles Cao 
*/

#ifndef BYTEORDERH
#define BYTEORDERH
#include "types.h"

/** \defgroup type Type definitions and data structures.

These definitons collectively define how to do some bytes and type information operations 
*/

/**@{*/

/** @brief Find whether the host machine is LSB. 
*     @return  If it is LSB, return 1, otherwise 0.
*/
inline int is_host_lsb(void);

/** @brief Change a variable into LSB 16 bit.
*     @param a The parameter to be changed.
*     @return The value in LSB.
*/
inline uint16_t toLSB16(uint16_t a);

/** @brief Change a variable FROM LSB 16 bit.
*     @param a The parameter to be changed.
*     @return The value in MSB.
*/
inline uint16_t fromLSB16(uint16_t a);

/**@}*/
#endif
