/** @file stdserial.h
       @brief The functional prototypes of printing over the serial port. 

	This file implements the generic layer for serial port printing. 
	@author Qing Charles Cao (cao@utk.edu)
*/

 
#ifndef STDSERIALH
#define STDSERIALH
#include "../../types/types.h"

/** @defgroup serial Serial port operations 
\par Overview
The serial port operations contain sending packets over the serial port. The receiving over serial port is no longer being supported. This is useful for debugging purposes. 
\par Initilization
There is no explicit initilization needed. The usart hardware is initilized the first time a byte is sent over it. 
*/

/** @{ */
/** @brief Print a string to the serial port. 
       @param str The string.
       @return Void. 
*/
void printfstr(char *str);

/** @brief Print a string with a fixed  length through the serial port. 
	@param str The string.
	@param len The length.
	@return Void. 
*/
void printfstrlen(char *str, uint8_t len);

/** @brief Print a 8-bit value to the serial port. 
	@param val The 8-bit value.
	@return Void. 
*/
void printfval(uint8_t val);

/** @brief Print a 32-bit value to the serial port. 
	@param val The 32-bit value.
	@return Void. 
*/

void printfinteger32(int32_t val);

/** @brief Print a 32-bit value to the serial port. 
	@param val The 32-bit value.
	@return Void. 
*/
void printfuinteger32(uint32_t val);

/** @brief Print a line break. 
	@return Void. 
*/

void printfstrln();


/** @} */
#endif
