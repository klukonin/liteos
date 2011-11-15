/**  @file serialprint.h
        @brief The header of the avr serial printing module. 

        This contains the prototypes of any functions in the serial printing module. 

        @author Qing Charles Cao (cao@utk.edu)
        
        
*/

#ifndef TESTUSART
#define TESTUSART
#include "../../types/types.h"


/** @addtogroup serial */

/** @{ */

/** @brief Printing multiple integers.
	@param a Integer.
	@param b Integer.
	@param c Integer.
	@param d Integer.
	@return Void. 
*/
void printInteger(int32_t a, int32_t b, int32_t c, int32_t d);

/** @brief Print string through the serial port.
	@param p String.
	@return Void.
*/
void printString(char *p);

/** @brief Print string with a number of bytes through the serial port.
	@param p The string.
	@param n The length.
	@return Void.
*/
void printStringN(char *p, uint8_t n);

/** @brief Print a 32-bit integer.
	@param a The integer. 
	@return Void.
*/

void printInteger32(int32_t a);

/** @brief Print a 32-bit integer.
	@param l The integer. 
	@return Void.
*/

void printIntegerU32(uint32_t l);

/** @brief Print a char.
	@param c The integer. 
	@return Void.
*/

void usartPrint(uint8_t c);

/** @brief Init USART.
	@return Void.
*/

void initUSART();

/** @brief Print a long value.
	@param l The value. 
	@return Void.
*/
void usartPutLong(uint32_t l);

/** @brief Print a line break. 
	@return Void.
*/

void printStringLn();


/** @brief Print a chunk of memory.
	@param p The starting address. 
	@param count The total number of bytes. 
	@return Void.
*/

void printMemory(char *p, uint16_t count);


/** @} */
#endif
