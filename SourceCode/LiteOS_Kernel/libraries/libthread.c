/** @file libthread.c
       @brief The functional implementation for the thread API. 

       @author  Qing Cao (cao@utk.edu) 
       
*/


#include "libthread.h"
#include <avr/io.h>
#include "../types/types.h"




//Warning: this returns the address of the currentthread pointer. 

lib_thread **lib_get_current_thread()
{
   lib_thread **current_thread; 
   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_THREAD_ADDRESS_FUNCTION; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (current_thread)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return current_thread; 
}


void **lib_get_kernel_stack_ptr()
	{
   void **ptr;
   void (*getaddrfp)(void) = (void (*)(void))GET_KERNEL_STACK_PTR_ADDRESS; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (ptr)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return ptr; 
}







uint8_t lib_get_current_thread_index()
{
   
   uint16_t index; 
   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_THREAD_INDEX_FUNCTION; 
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
   return (uint8_t)index; 
}


lib_thread *lib_get_thread_table_start()
{
	 lib_thread *fp;
	 
   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_THREAD_TABLE_START; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (fp)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return fp; 
	
}

void lib_sleep_thread(uint16_t milliseconds)
{
/*
   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_THREAD_ADDRESS_FUNCTION; 

   asm volatile("push r24" "\n\t"
	             "push r25" "\n\t"
	              ::);
   getaddrfp();     

   asm volatile(" mov %A0, r24" "\n\t"
	             "mov %B0, r25" "\n\t"
				 :"=r" (current_thread)
				 :
                );
    asm volatile("pop r24" "\n\t"
	             "pop r25" "\n\t"
	              ::);
 */
 
   lib_thread **current_thread; 
   current_thread = lib_get_current_thread(); 
     
   (*current_thread)->state = 4;
   (*current_thread)->data.sleepstate.sleeptime = milliseconds;
   
   lib_yield(); 
}


void lib_hibernate_thread()
{
 
   lib_thread **current_thread; 
   current_thread = lib_get_current_thread(); 
     
   (*current_thread)->state = 5; 
   lib_yield(); 
}


void lib_yield()
{
 void (*yieldfp)(void) = (void (*)(void))YIELD_FUNCTION; 
 yieldfp();                              
}



void syscall_post_thread_task()
{
 void (*postthreadfp)(void) = (void (*)(void))POST_NEW_THREAD_TASK; 
 postthreadfp();                              
}



void lib_post_new_task(void (*tp) (void), uint16_t priority)
{
   void (*fp)(void) = (void (*)(void))POST_TASK_SYSCALL; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
				"push r22" "\n\t"
				"push r23" "\n\t"
                ::);
   
   asm volatile(" mov r20, %A0" "\n\t"
	             "mov r21, %B0" "\n\t"
				 :
				 :"r" (tp)
                );


   asm volatile(" mov r22, %A0" "\n\t"
	             "mov r23, %B0" "\n\t"
				 :
				 :"r" (priority)
                );

  fp(); 

  asm volatile("pop r23" "\n\t"
	           "pop r22" "\n\t"
	              ::);

  asm volatile("pop r21" "\n\t"
	           "pop r20" "\n\t"
	              ::);
  return; 


}



 



void syscall_post_thread_task_no_logging()
{
	void (*postthreadfp)(void) = (void (*)(void))POST_NEW_THREAD_TASK_NOLOG; 
  postthreadfp();  
	
}



 

void lib_set_thread_termination_handler(void (*tp) (void))
{
   
   lib_thread **current_thread; 
   current_thread = lib_get_current_thread(); 
     
   (*current_thread)->thread_clear_function = tp; 
   return; 
}

 



