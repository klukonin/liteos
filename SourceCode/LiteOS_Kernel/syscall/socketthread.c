/** @file socketthread.c
	@brief The functional implementation for socket thread.

	@author Qing Cao (cao@utk.edu)
*/


#include "../kernel/threadkernel.h"
#include "socketthread.h"
#include "../kernel/threadtools.h"
#include "../kernel/threaddata.h"
#include "../kernel/scheduling.h"
#include "../timer/generictimer.h"
#include "../io/radio/packethandler.h"
#include "../io/serial/stdserial.h"
#include "../types/string.h"
#include "../config/nodeconfig.h"
#include "../syscall/socketradiodata.h"
#include "../timer/globaltiming.h"
#include "../hardware/avrhardware.h"



//Our thread table
extern thread thread_table[LITE_MAX_THREADS];

//point to the current thread
extern volatile thread *current_thread;




//Return the address pointer of the current_thread, (its address, not its value)
thread **getCurrentThreadAddress()
{
  thread **addr;

  addr = (thread **) &current_thread;
  return addr;
  //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( addr ) );
  // asm volatile("ret"::); 
}


//Return the address pointer of the current_thread, (its address, not its value)
thread *getThreadTableStart()
{
  thread *addr;

  addr = &thread_table[0];
  
  return addr;
  //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( addr ) );
  // asm volatile("ret"::); 
}

