/** @file libthread.h
       @brief The functional declaration for the thread API. 

       @author  Qing Cao (cao@utk.edu) 
       
*/

#ifndef THREADH
#define THREADH




#include "liteoscommon.h"

/** @addtogroup api
*/

/** @{
*/

 
/** @brief Sleep the current thread for a period of time.
	@param milliseconds The time period. 
	@return Void. 
*/

void lib_sleep_thread(uint16_t milliseconds);

/** @brief Yield the thread. 
	@return Void. 
*/


void lib_yield(); 

/** @brief Get current thread. 
	@return Current thread pointer. 
*/

lib_thread **lib_get_current_thread();


/** @brief Get current thread index. 
	@return Current thread index. 
*/

uint8_t lib_get_current_thread_index();


/** @brief Post new task.
	@param tp The pointer to a task.
	@param priority The priority of a task. 
	@return Void. 
*/

void lib_post_new_task(void (*tp) (void), uint16_t priority);


/** @brief Post new thread task.
	@return Void. 
*/

void syscall_post_thread_task(); 




/** @brief Hibernate the current thread until it is waken up.
	@return Void. 
*/


void lib_hibernate_thread();



/** @brief Create a new thread.
	@param fp The thread pointer.
	@param ram_start The start of the ram.
	@param stack_ptr The end of the ram.
	@param priority The priority.
	@param threadname The thread name. 
	@return Void. 
*/
void lib_create_new_thread(void (*fp)(), uint8_t* ram_start, uint8_t *stack_ptr, uint8_t priority, const char *threadname);



/** @brief Set thread termination handler. 
	@param tp The thread handler. 
	@return Void. 
*/


void lib_set_thread_termination_handler(void (*tp) (void));


/** @brief Set thread timer callback function. 
	@param period The period for callback.
	@param type The type of timer.
	@param callback The callback function. 
	@return Void. 
*/


void lib_set_timer_function(uint16_t period, uint8_t type, void (*callback)(void));

/** @brief Post new thread task with logging disabled. 
	@return Void. 
*/


void syscall_post_thread_task_no_logging();

/** @brief Get kernel stack pointer. 
	@return Void pointer. 
*/


void **lib_get_kernel_stack_ptr(); 

/** @brief Get thread table start. 
	@return Thread pointer. 
*/

lib_thread *lib_get_thread_table_start();

/** @}
*/


#endif 
