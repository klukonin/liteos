/** @file stdserial.c
       @brief The functional implementation of printing over the serial port. 

	This file implements the generic layer for serial port printing by calling avr specific code. 
	@author Qing Charles Cao (cao@utk.edu)
*/ 
 
#include "stdserial.h"
#include "../../types/types.h"
#include "../../config/nodeconfig.h"

#ifdef PLATFORM_AVR
#include "../avrserial/serialprint.h"
#endif


void printfstr(char *str)
{
	
	_atomic_t currentatomic;

    currentatomic = _atomic_start();
#ifdef PLATFORM_AVR
		#ifdef PRINT_SOURCE_ENABLED
	  printString(node_readnodestring());
	  printString(": ");
	  #endif
    printString(str);
#endif
  _atomic_end(currentatomic);
}

void printfstrln()
 {
 	_atomic_t currentatomic;

    currentatomic = _atomic_start();
  #ifdef PLATFORM_AVR
     printStringLn();
  #endif
  _atomic_end(currentatomic);
  
 }
 

 
//-------------------------------------------------------------------------
void printfinteger32(int32_t val)
{
	_atomic_t currentatomic;

    currentatomic = _atomic_start();
#ifdef PLATFORM_AVR
	#ifdef PRINT_SOURCE_ENABLED
	  printString(node_readnodestring());
	  printString(": ");
	#endif
    printInteger32(val);
#endif
   _atomic_end(currentatomic); 
}

//-------------------------------------------------------------------------
void printfuinteger32(uint32_t val)
{
	_atomic_t currentatomic;

    currentatomic = _atomic_start();
    
#ifdef PLATFORM_AVR
  #ifdef PRINT_SOURCE_ENABLED
	  printString(node_readnodestring());
	  printString(": ");
	#endif
    printIntegerU32(val);
#endif
_atomic_end(currentatomic); 
}
