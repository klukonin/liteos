#include "threadkernel.h"
#include "threadtools.h"
#include "threaddata.h"
#include "scheduling.h"
#include "threadmodel.h"
#include "../io/radio/packethandler.h"
#include "../types/string.h"
#include "../timer/generictimer.h"
#include "../hardware/avrhardware.h"
#include "../timer/globaltiming.h"
 


//stores the maximum thread bound updated when adding thread
//maxrambound is set whenever a new thread is created. the upper bound of the thread is compared against to find out
//whether the maxthreadrambound should be updated or not.

volatile uint16_t *maxthreadrambound;

//Our thread table
thread thread_table[LITE_MAX_THREADS];


//point to the current thread
volatile thread *current_thread;

//kernel thread stack pointer. the stack beginning is always the return address
volatile uint16_t *old_stack_ptr;

//stack interrupt ptr. used to store the thread stack and go back to the kernel thread temporarily when interrupt occurs 
volatile uint16_t *stackinterrupt_ptr;

//This is simply a way to track whether our task is running
volatile uint8_t thread_task_active;

 
 
 uint32_t timediff;
 
 //resolution
 uint32_t switchFromThreadTime , switchToThreadTime; 
 
 //time 
 uint16_t counterFrom, counterTo; 


//-------------------------------------------------------------------------
void thread_init()
{
 
  _atomic_t currentatomic;

  currentatomic = _atomic_start();
  
  //initilize the thread table
  nmemset(thread_table, 0, sizeof(thread) *LITE_MAX_THREADS);
    
  //init all the variables 
  current_thread = 0;
  old_stack_ptr = 0;
  stackinterrupt_ptr = 0;
  thread_task_active = 0;
  maxthreadrambound = 0;
  _atomic_end(currentatomic);
  //    TimerM_Timer_start(9, TIMER_REPEAT, 1000);
}



//--------------------------------------------------------------------------
//this is still working as it checks if the upperfound of threads are larger than the spvalue or not. 
uint8_t is_thread()
{
  uint16_t SPvalue;
  asm volatile("in %A0, 0x3d""\n\t""in %B0, 0x3e""\n\t": "=r"(SPvalue): );

  if (SPvalue > (uint16_t)maxthreadrambound)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}




//--------------------------------------------------------------------------
//Now adds the support for kernel built-in memory corrupt search and find 
//updated the romstart and romsize parameter in version 0.31 to support rom image loading conflict detection 


int create_thread(void(*fcn)(), uint16_t *ram_start, uint16_t *stack_ptr,
  uint16_t staticdatasize, uint8_t priority, char *threadName, uint16_t
  romstart, uint16_t romsize)
{
  int i;
  _atomic_t currentatomic;

  //used for handling the built-in memory corrupt detection
  uint16_t *kernelptr;

  //we do not allow threads context to create new threads
  if (is_thread())
  {
    return (0);
  }
  if (stack_ptr > maxthreadrambound)
  {
    maxthreadrambound = stack_ptr;
  }
  //First loop all the way through the table and find an empty slot 
  //computation time for space here 
  for (i = 0; i < LITE_MAX_THREADS; i++)
  {
    if (thread_table[i].state == STATE_NULL)
    {
      break;
    }
  }
  //Cannot create thread, no resources... 
  if (i == LITE_MAX_THREADS)
  {
    return (0);
  }
  //Add the thread
  currentatomic = _atomic_start();
  //Populate the thread table
  current_thread = &thread_table[i];
  current_thread->sp = stack_ptr;
  current_thread->state = STATE_ACTIVE;
  current_thread->data.tp = fcn;
  current_thread->priority = priority;
  current_thread->remaincredits = priority;
  current_thread->ramstart = ram_start;
  current_thread->ramend = stack_ptr;
  current_thread->thread_clear_function = NULL; 

  //if the thread is created by the kernel directly, then the following are all 0. 
  current_thread->sizeofBss = staticdatasize;
  current_thread->romstart = romstart;
  current_thread->romsize = romsize;

 //COPY file name 
  {
    uint8_t templen;

    templen = mystrlen(threadName);
    mystrncpy((char*)current_thread->threadName, (char*)threadName, templen + 1)
      ;
  }
  
  //set up the corruption detection 
  
  kernelptr = (uint16_t*)((uint8_t*)ram_start + staticdatasize);
  *kernelptr = 0xeeff;
  *(kernelptr + 1) = 0xeeff;
  
  #ifdef PLATFORM_AVR
    //Prepare the fcn pointer on the new stack, so it can be 
    //prepare set the beginning as the function then registers as 0. 
    PREPARE_REG_FOR_STACK();
  #endif 
  
  
  current_thread = 0;
  
  //current_thread->energycontrolblock.energycost = 0;
  //current_thread->energycontrolblock.energyremain = 0; 
  
  if (!thread_task_active)
  {
    postTask(thread_task, 2);
    thread_task_active = 1;
  }
  _atomic_end(currentatomic);
  
  #ifdef TRACE_ENABLE
    #ifdef TRACE_ENABLE_THREADCREATE
      addTrace(TRACE_THREADCREATE, 100);
    #endif 
  #endif 
  return (1);
}



/* Set up the destroy thread function call */
void setThreadTerminateFunction(uint8_t currentindex, void(*fp)())
{
  thread_table[currentindex].thread_clear_function = fp;
}


/* destroy_user_thread
 * This routine is only called when a users thread returns.
 * It removes it from the thread table.
 */
void destroy_user_thread()
{
  uint8_t *start,  *end;
  _atomic_t currentatomic;
  int indexofthread;

  currentatomic = _atomic_start();
  current_thread->state = STATE_NULL;
  start = (uint8_t*)current_thread->ramstart;
  end = (uint8_t*)current_thread->ramend;
  deleteThreadRegistrationInReceiverHandles(start, end);
  
  indexofthread = getThreadIndexAddress();
  
  
  if (thread_table[indexofthread].thread_clear_function != NULL)
  {
    (*thread_table[indexofthread].thread_clear_function)();
    thread_table[indexofthread].thread_clear_function = NULL;
  }
  
  #ifdef TRACE_ENABLE
    #ifdef TRACE_ENABLE_THREADDESTROY
      addTrace(TRACE_THREADDESTROY, 100);
    #endif 
  #endif 
  thread_yield();
  _atomic_end(currentatomic);
}




// These are new routines
/* lite_switch_to_user_thread()
 * This routine swaps the stack and allows a thread to run.
 */
void __attribute__((noinline))lite_switch_to_user_thread() /* __attribute__(
(naked)) */
{
  #ifdef TRACE_ENABLE
    #ifdef TRACE_ENABLE_CONTEXTSWITCH
      addTrace(TRACE_CONTEXTSWITCHTOUSERTHREAD, 100);
    #endif 
  #endif 
  
  //these are the current times for the switch to user thread
  
  counterTo = getCurrentCounterHigh();
  switchToThreadTime = getCurrentResolution();
  
   //printfstr("Now switching to user.  \n"); 
  #ifdef PLATFORM_AVR
    PUSH_REG_STATUS();
    PUSH_GPR();
    SWAP_STACK_PTR(old_stack_ptr, current_thread->sp);
    POP_GPR();
    POP_REG_STATUS();
  #endif 
  
  //_avr_enable_interrupt(); 
  return ;
}

//-------------------------------------------------------------------------
void __attribute__((noinline))thread_yield() /* __attribute__((noinline)) */
{
  #ifdef PLATFORM_AVR
    PUSH_REG_STATUS();
    PUSH_GPR();
    //Now swap the stacks back
    SWAP_STACK_PTR(current_thread->sp, old_stack_ptr);
    POP_GPR();
    POP_REG_STATUS();


  #endif 
  #ifdef TRACE_ENABLE
    #ifdef TRACE_ENABLE_CONTEXTSWITCH
      addTrace(TRACE_CONTEXTSWITCHFROMUSERTHREAD, 100);
    #endif 
  #endif 
}


/* thread_sleep
 * This routine puts the current thread into a sleeping state.
 * It will not ever wake up until another task or thread wakes it up
 */
void sleepThread(uint16_t milli)
{
  if (!is_thread())
  {
    return ;
  }
  //this is insid the thread!
  current_thread->state = STATE_PRESLEEP;
  current_thread->data.sleepstate.sleeptime = milli;
  thread_yield();
}

//This routine is called to perform system level utility change and schedules thread_task again 






//this executes and cleans up a thread
//Make sure that no variables are allocated
// also make sure no functions are called with attributes
void thread_func_dispatcher()__attribute__((naked));
void thread_func_dispatcher()
{
  //    (*current_thread->data.tp)();
  call_fcn_ptr(current_thread->data.tp);
  destroy_user_thread();
}






//-------------------------------------------------------------------------
void thread_task()
{
  int i;
  uint8_t thread_presleep;
  _atomic_t currentatomic;
   

  currentatomic = _atomic_start();
  thread_presleep = 0;

  for (i = 0; i < LITE_MAX_THREADS; i++)
  {
    if (thread_table[i].state == STATE_PRESLEEP)
    {
      break;
    }
  }
  thread_presleep = (i != LITE_MAX_THREADS);

  if (thread_presleep)
  {
    postTask(thread_task, 3);
    
    thread_table[i].state = STATE_SLEEP;
    
    _atomic_end(currentatomic);
    // TimerM_Timer_start( i, TIMER_ONE_SHOT, thread_table[ i ].data.sleepstate.sleeptime );
    GenericTimerStart(i, TIMER_ONE_SHOT,
      thread_table[i].data.sleepstate.sleeptime);
    
    thread_presleep = 0;
    return ;
  }
  
  _atomic_end(currentatomic);
  
  i = thread_get_next();
  if (i < 0)
  {
    //here is the exit      
    return ;
  }
  postTask(thread_task, 3);
  
  i = check_for_memory_corrupt(i);
  if (i < 0)
  {
    //here is the exit      
    return ;
  }

  //thread sequence logging 
   
   /*
  write8uint(nextwriteaddr, i);
  nextwriteaddr++; 
  if (nextwriteaddr == 3001)
   nextwriteaddr = 2701; 
  write8uint(nextwriteaddr, 0xFF); 
   */

  current_thread = &(thread_table[i]);
  
  //printfintegeru32(i);
  //printfstr(" thread index\n");
  
  lite_switch_to_user_thread();    
 
  //printfstr("now switching out\n");
 

  
  
  counterFrom = getCurrentCounterHigh(); 
  switchFromThreadTime = getCurrentResolution(); 
  
  if (counterFrom == counterTo)
	  timediff = switchFromThreadTime - switchToThreadTime;
	  else
	  timediff = ((uint32_t)(counterTo-counterFrom))*50000*50000 + switchFromThreadTime - switchToThreadTime; 
     
	 /*
   if (current_thread != &thread_table[0])
     {

     for (threadindex =1;threadindex <LITE_MAX_THREADS; threadindex++)
     
      { if (current_thread == &thread_table[threadindex])
     	     break;
     	}
     	
     } */

   //current_thread->energycontrolblock.energycost += (timediff * (uint32_t)CPU_PER_THOUSAND) /1000; 
   current_thread = 0;
   return ;
}


/* thread_wakeup
 * This routine wakes up a thread that was put to sleep.
 */
void thread_wakeup(uint8_t id)
{
  if (id >= LITE_MAX_THREADS)
  {
    return ;
  }
  if (thread_table[id].state == STATE_SLEEP)
  {
    thread_table[id].state = STATE_ACTIVE;
  }
  if (thread_task_active == 0)
  {
    postTask(thread_task, 2);
    thread_task_active = 1; 
  }
}


//-------------------------------------------------------------------------
void postNewThreadTask()
{
  if (thread_task_active == 0)
  {
    postTask(thread_task, 2);
    thread_task_active = 1; 
  }
}



//----------------------------------------------------------------------------

//-------------------------------------------------------------------------
//breakpoint
/*
void break_point_function() {
   int i;
   uint16_t index;
   index = 0;
   for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
      if ( current_thread == & ( thread_table[ i ] )) {
         index = i;
         break;
      }
   }
   thread_table[ index ].state = STATE_BREAK;
   thread_yield();
}
*/
