/** @file socketthread.h
	@brief The functional prototypes for socket thread.

	@author Qing Cao (cao@utk.edu)
*/

#ifndef THREADSYSCALLH
#define THREADSYSCALLH
#include "../kernel/threaddata.h"

/** @defgroup socket Socket operations. 
*/

/** @addtogroup socket */
/** @{ */
/** @brief Get current thread address.
	@return Thread pointer.
*/
thread **getCurrentThreadAddress();


/** @brief Get thread table start.
	@return Thread pointer.
*/
thread *getThreadTableStart();
/** @}*/
#endif
