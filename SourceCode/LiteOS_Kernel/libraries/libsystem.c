/** @file libsystem.c
       @brief The functional implementation for the system configuration API. 

       @author  Qing Cao (cao@utk.edu) 
       
*/


#include "libsystem.h"
#include "liteoscommon.h"
#include "../types/types.h"
#include <avr/io.h>
#include "../types/types.h"

volatile lib_thread ** current_thread;


volatile uint16_t * old_stack_ptr ;

_atomic_t flag; 


int lib_random()
{

	 int ret;
   void (*getrandomfp)(void) = (void (*)(void))GET_RANDOM_NUMBER_FUNCTION;
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getrandomfp();
   asm volatile(" mov %A0, r20" "\n\t"
	              "mov %B0, r21" "\n\t"
				 :"=r" (ret)
				 :
                );
   asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return ret;

}



uint16_t lib_get_node_id()
	{

	   int ret;
	   void (*fp)(void) = (void (*)(void))GET_NODE_ID_FUNCTION;
	   asm volatile("push r20" "\n\t"
					"push r21" "\n\t"
					::);
	   fp();
	   asm volatile(" mov %A0, r20" "\n\t"
					  "mov %B0, r21" "\n\t"
					 :"=r" (ret)
					 :
					);
	   asm volatile("pop r21" "\n\t"
					 "pop r20" "\n\t"
					  ::);
	   return ret;

	}


void lib_set_node_id(uint16_t nodeid)
	{



	   void (*fp)(void) = (void (*)(void))SET_NODE_ID_FUNCTION;
	   asm volatile("push r20" "\n\t"
					"push r21" "\n\t"
					::);

	   asm volatile(" mov r20, %A0" "\n\t"
					  "mov r21, %B0" "\n\t"
					 :
					 :"r" (nodeid)
					);
	   fp();
	   asm volatile("pop r21" "\n\t"
					 "pop r20" "\n\t"
					  ::);
	   return;

	}

uint32_t get_current_timestamp()
{
	   uint32_t counter; 
       void (*fp)(void) = (void (*)(void))GET_CPU_COUNT_FUNCTION;
	   asm volatile("push r22" "\n\t"
					"push r23" "\n\t"
					"push r24" "\n\t"
					"push r25" "\n\t"
					::);
	   fp();

	   asm volatile("mov  %A0, r22" "\n\t"
					  "mov %B0, r23" "\n\t"
					  "mov  %C0, r24" "\n\t"
					  "mov  %D0, r25" "\n\t"
					 :
					 :"r" (counter)
					);
	  
	   asm volatile("pop r22" "\n\t"
					 "pop r23" "\n\t"
					 "pop r24" "\n\t"
					 "pop r25" "\n\t"
					  ::);
	   return counter;

    	
}


#ifdef PLATFORM_CPU_MEASURE


uint16_t getKernelUtilization()
{
   uint16_t index;
   void (*getaddrfp)(void) = (void (*)(void))GET_CPU_UTILIZATION; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (index)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return index; 
}

#endif 
 


