 
#ifndef filesocketH
#define filesocketH
#include "../kernel/threadkernel.h"
#include "threadsyscall.h"
#include "../kernel/threadtools.h"

/**\defgroup filesystem LiteFS API
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
///\ingroup filesystem

/** Open the file task  */
void openFileTask();

///\ingroup filesystem

/** Close the file task  */
void closeFileTask();

///\ingroup filesystem

/** Read the file task  */
void readFileTask();

///\ingroup filesystem

/** Write the file task  */
void writeFileTask();

///\ingroup filesystem

/** Seek the file task  */
void seekFileTask();


///\ingroup filesystem
///Get the file path address
void *getFilePathAddress();     //  __attribute__((naked)) ;

///\ingroup filesystem
///Get the file mode address
void *getFileModeAddress();     //  __attribute__((naked));


#endif
