/**  @file threadtools.c
        @brief The implementation for the thread thread tools module. 

        @author Qing Charles Cao (cao@utk.edu)
*/



#include "threadkernel.h"
#include "threadtools.h"
#include "threaddata.h"
#include "scheduling.h"
 


//stores the maximum thread bound updated when adding thread
//maxrambound is set whenever a new thread is created. the upper bound of the thread is compared against to find out
//whether the maxthreadrambound should be updated or not.

extern volatile uint16_t *maxthreadrambound;

//Our thread table
extern thread thread_table[LITE_MAX_THREADS];


//point to the current thread
extern volatile thread *current_thread;

//kernel thread stack pointer. the stack beginning is always the return address
extern volatile uint16_t *old_stack_ptr;

//stack interrupt ptr. used to store the thread stack and go back to the kernel thread temporarily when interrupt occurs 
extern volatile uint16_t *stackinterrupt_ptr;

//This is simply a way to track whether our task is running
extern volatile uint8_t thread_task_active;


//-------------------------------------------------------------------------
uint8_t memory_conflict_detect(uint16_t createflashromstart, uint16_t
  createflashromsize, uint16_t ramstackstart, uint16_t ramstackend)
{
  uint8_t i;
  uint16_t userthreadromstart;
  uint16_t userthreadromend;
  uint16_t userthreadramstart;
  uint16_t userthreadramend;

  for (i = 0; i < LITE_MAX_THREADS; i++)
  {
    if (thread_table[i].state != STATE_NULL)
    {
      if (thread_table[i].romstart == 0)
      {
        continue;
      }
      userthreadromstart = thread_table[i].romstart;
      userthreadromend = thread_table[i].romsize / 2+userthreadromstart;
      userthreadramstart = (uint16_t)thread_table[i].ramstart;
      userthreadramend = (uint16_t)thread_table[i].ramend;
      if (!((createflashromstart > userthreadromend + 2) || 
        (createflashromstart + createflashromsize / 2 < userthreadromstart - 2))
        )
      {
        return 1;
      }
      if (!((ramstackstart > userthreadramend + 1) || (ramstackend <
        userthreadramstart - 1)))
      {
        return 1;
      }
    }
  }
  return 0;
}


/* thread_task
 */
int check_for_memory_corrupt(int i)
{
  uint16_t *kernelptr;
  uint16_t *ram_start,  *ram_end;
  uint16_t sizeofBss;

  ram_start = thread_table[i].ramstart;
  ram_end = thread_table[i].ramend;
  sizeofBss = thread_table[i].sizeofBss;
  kernelptr = (uint16_t*)((uint8_t*)ram_start + sizeofBss);
  if ((*kernelptr != 0xeeff) || (*(kernelptr + 1) != 0xeeff))
  {
    thread_table[i].state = STATE_MEM_ERROR;
    return  - 1;
  }
  return i;
  
}



//-------------------------------------------------------------------------
void ServiceTimerFired(uint8_t id)
{
  if (id == 9)
  {
    if (thread_task_active == 0)
    {
      postTask(thread_task, 2);
    }
  }
  else
  {
    thread_wakeup(id);
  }
}





//-------------------------------------------------------------------------
void **getKernelStackAddress()
{
  void **addr;

  addr = (void **) &old_stack_ptr;

  return addr;
}

//void getThreadIndexAddress() __attribute__((naked));
int getThreadIndexAddress()
{
  int i;
  uint16_t index;
  asm volatile("push r20""\n\t""push r21""\n\t"::);

  index = 0;
  for (i = 0; i < LITE_MAX_THREADS; i++)
  {
    if (current_thread ==  &(thread_table[i]))
    {
      index = i;
      break;
    }
  }
  asm volatile("pop r21""\n\t""pop r20""\n\t"::);

  return index;
  //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( index ) );
  //asm volatile("ret"::); 
}



/* This unblocks an IO bound thread.
 * This routine can be called from any context.
 */
void barrier_unblock(uint8_t type, uint8_t id)
{
  uint8_t i;
  _atomic_t currentatomic;

  currentatomic = _atomic_start();
  for (i = 0; i < LITE_MAX_THREADS; i++)
  {
    //Look for a thread waiting on this IO
    if ((thread_table[i].state == STATE_IO) && 
      (thread_table[i].data.iostate.type == type) && 
      (thread_table[i].data.iostate.id == id))
    {
      //Mark that thread as active
      thread_table[i].state = STATE_ACTIVE;
      postNewThreadTask();
    }
  }
  _atomic_end(currentatomic);
}


