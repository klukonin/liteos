#ifndef SERIALPRINTH
#define SERIALPRINTH


#include "../types/types.h"  


//No need to be called from main 
//This function is called when the system is being initilized and prints system start 
void lib_init_usart();

//-------------------------------------------------------------------------
void lib_print_usart_char(uint8_t c);

//-------------------------------------------------------------------------
void lib_print_usart_long(uint32_t l);

//-------------------------------------------------------------------------
void lib_printf_integer32(int32_t a);

//-------------------------------------------------------------------------
void lib_printf_uinteger32(uint32_t l);

void lib_printf_ln();
 
void lib_printf_string(char *p);

#endif
