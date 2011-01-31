#include "threadmodel.h"
#include "threaddata.h"
#include "../types/types.h"


//This function uses the remaining credits to find out the appropriate next thread and returns it 

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


 int thread_get_next()
{
  int i;
  int credits;
  int currentcandidate;
  _atomic_t currentatomic;

  currentcandidate =  - 1;
  credits =  - 1;
  currentatomic = _atomic_start();
  for (i = 0; i < LITE_MAX_THREADS; i++)
  {
    if (thread_table[i].state == STATE_ACTIVE)
    {
      if (credits < thread_table[i].remaincredits)
      {
        credits = thread_table[i].remaincredits;
        currentcandidate = i;
      }
    }
  }
  if (credits < 0)
  {
    thread_task_active = 0;
  }
  _atomic_end(currentatomic);
  if (credits > 0)
  {
    currentatomic = _atomic_start();
    thread_table[currentcandidate].remaincredits--;
    _atomic_end(currentatomic);
    return currentcandidate;
  }
  else if (credits == 0)
  {
    currentatomic = _atomic_start();
    for (i = 0; i < LITE_MAX_THREADS; i++)
    {
      if (thread_table[i].state == STATE_ACTIVE)
      {
        thread_table[i].remaincredits = thread_table[i].priority;
      }
    }
    thread_table[currentcandidate].remaincredits--;
    _atomic_end(currentatomic);
    return currentcandidate;
  }
  else if (credits < 0)
  {
    return  - 1;
  }
  return 0;
}




