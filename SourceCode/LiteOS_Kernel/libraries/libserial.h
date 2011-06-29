/** @file libserial.h
       @brief The functional prototype for the serial API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/


#ifndef SERIALPRINTH
#define SERIALPRINTH


#include "../types/types.h"  


/** @addtogroup api
*/

/** @{
*/

/** @brief Init usart.
	@return Void. 
*/
void lib_init_usart();


//-------------------------------------------------------------------------
/** @brief Print char. 
	@param c The char. 
	@return Void. 
*/

void lib_print_usart_char(uint8_t c);

//-------------------------------------------------------------------------

/** @brief Print long integer.
	@param l The long integer. 
	@return Void. 
*/
void lib_print_usart_long(uint32_t l);

//-------------------------------------------------------------------------

/** @brief Print 32-bit integer.
	@param a The 32-bit integer. 
	@return Void. 
*/

void lib_printf_integer32(int32_t a);

//-------------------------------------------------------------------------

/** @brief Print 32-bit integer.
	@param l The 32-bit integer. 
	@return Void. 
*/

void lib_printf_uinteger32(uint32_t l);

/** @brief Print line break. 
	@return Void. 
*/


void lib_printf_ln();

/** @brief Print string. 
	@param p The string. 
	@return Void.
*/

void lib_printf_string(char *p);

/** @} */
#endif
