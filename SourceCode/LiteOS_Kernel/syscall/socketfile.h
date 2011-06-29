/** @file socketfile.h
	@brief The functional prototypes for socket file.

	@author Qing Cao (cao@utk.edu)
*/


#ifndef SOCKETFILEH
#define SOCKETFILEH
#include "../kernel/threadkernel.h"
#include "socketthread.h"
#include "../kernel/threadtools.h"

/**
This module implements various commands and functions for the LiteFS file system. It is a layered architecture that touches down to the flash space.
\par Overview
The file system API contains two parts, just like the radio and the serial port API. The first part is for user space applications to access the file system 
functionality. The second part is for kernel space user applications to access the file system functionality. We now describe these two parts separately. 
\par Kernel API 
The kernel API may be directly called by supplying correct parameters, and the API functions will do what is told. 
\par User Space API
The user space API may be directly called by user applications through sytem calls. However, these API functions could not be supplied directly with 
parameters. Instead, the user application must first write data into a memory address in the kernel, and then call the functions ending with TASK. 
To use the user space API, note that the user application first use getFilePathAddress(), and getFileModeAddress() to get the
file system memory data structures, populate them, then call the corresponding file task. Note that right now, this user side is limited to read and write. More 
complicated support such as copy will be added in the future versions if found needed. 
*/ 


/** @addtogroup socket */
/** @{ */

/** @brief Open the file task.
	@return Void. 
*/
void openFileTask();

/** @brief Close the file task.
	@return Void. 
*/
void closeFileTask();

/** @brief Read the file task.
	@return Void. 
*/
void readFileTask();

/** @brief Write the file task.
	@return Void.
*/
void writeFileTask();

/** @brief Seek the file task.
	@return Void.
*/
void seekFileTask();


/** @brief Get the file path address.
	@return Void pointer. 
*/
void *getFilePathAddress();     //  __attribute__((naked)) ;


/** @brief Get the file mode address.
	@return Void pointer.
*/
void *getFileModeAddress();     //  __attribute__((naked));

/** @} */

#endif
