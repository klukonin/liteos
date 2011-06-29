/**  @file threadkernel.h
        @brief The header for the thread kernel module. 

        @author Qing Charles Cao (cao@utk.edu)
*/



#ifndef THREADKERNELH
#define THREADKERNELH
#include "threaddata.h"
 

/** @addtogroup scheduling */

/** @brief Common thread table. */
extern thread thread_table[LITE_MAX_THREADS];


/** @brief Pointer to the current thread. */
extern volatile thread *current_thread;


/** @brief Init a thread.
	@return Void.
*/
void thread_init();

/** @brief Whether currently is in thread.
	@return Yes or no. 
*/
uint8_t is_thread();


/** @brief Create thread.
	@param fcn Function pointer.
	@param ram_start The starting of ram allocation.
	@param stack_ptr The end of the ram allocation, or the start of the stack. 
	@param staticdatasize  The size of static data. 
	@param priority The priority of the thread. 
	@param threadName The name of the thread. 
	@param romstart The start of the rom.
	@param romsize The size of the rom. 
	@return Whether the thread is created successfully. 
*/

int create_thread(void (*fcn) (), uint16_t * ram_start, uint16_t * stack_ptr,
                  uint16_t staticdatasize, uint8_t priority, char *threadName,
                  uint16_t romstart, uint16_t romsize);


/** @brief Set the thread terminate function.
	@param currentindex The index of the thread. 
	@param fp The functional pointer to the terminate function.
	@return Void. 
*/

void setThreadTerminateFunction(uint8_t currentindex, void(*fp)());

/** @brief Destroy a thread.
	@return Void. 
*/

void destroy_user_thread();

/** @brief Switch to a user thread.
	@return Void. 
*/

void __attribute__((noinline))lite_switch_to_user_thread();

/** @brief Yield a user thread.
	@return Void. 
*/


void __attribute__((noinline))thread_yield();

/** @brief Sleep a thread for a certain period of time. 
	@param milli The total length of sleep. 
	@return Void. 
*/

void sleepThread(uint16_t milli);

/** @brief Handle service of threads. 
	@return Void. 
*/

void threads_handle_service();

/** @brief Thread functional dispatcher. 
	@return Void. 
*/


void thread_func_dispatcher()__attribute__((naked));

/** @brief  Get the next thread. 
	@return The thread index. 
*/

inline int thread_get_next();


/** @brief The thread task to be posted. 
	@return Void. 
*/

void thread_task();


/** @brief Wakeup a thread. 
	@param id The thread index. 
	@return Void.
*/

void thread_wakeup(uint8_t id);

/** @brief Post a thread task. 
	@return Void. 
*/

void postNewThreadTask();

/** @} */


#endif
