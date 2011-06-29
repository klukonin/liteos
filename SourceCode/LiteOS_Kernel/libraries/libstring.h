/** @file libstring.h
       @brief The functional prototype for the string API. 

       @author  Qing Cao (cao@utk.edu) 
       
*/


#ifndef STRINGUTILH
#define STRINGUTILH






#include "../types/types.h"

/** @addtogroup api
*/

/** @{
*/


/** @brief   Get the length of the string.
	@param s The string.
	@return The length of the string. 
*/

int lib_string_length(char* s);

/** @brief  Copy string from source to destination.
	@param dest The destination.
	@param src The source.
	@param n Total number of characters copied. 
	@return Void. 
*/

void lib_mystrncpy(char *dest, const char *src, uint16_t n);

/** @brief  Copy string from source to destination.
	@param dest The destination.
	@param src The source.
	@return Void. 
*/

void lib_mystrcpy(char *dest, const char *src); 


/** @brief  Append string to the base. 
	@param base The base. 
	@param string The appended string.
	@return Void. 
*/

void lib_string_append(char *base, char *string);

/** @brief  Split a string.
	@param string The string. 
	@param delimiter The delimiter.
	@return The delimited string. 
*/

char* lib_string_split(char ** string, char delimiter);


/** @brief Convert integer to a string. 
	@param num The number.
	@return The string.
*/
char *lib_string_int_to_string(int num);


/** @brief Convert hex to value. 
	@param hex The hex.
	@return The value. 
*/
uint16_t lib_hex2value(uint8_t hex);

/** @} */
#endif
