/** @file commandhandle.h
	@brief The declarations of the command handle. 

	@author Qing Charles Cao (cao@utk.edu)
*/


#ifndef COMMANDHANDLEH
#define COMMANDHANDLEH
#include "../types/types.h"
#include "../storage/filesys/fsapi.h"

/**\defgroup commands Interactive shell APIs.

This module implements various command handlers. 
*/

/**@{ */

/** @brief This function defines the command handler for an incoming message. 

       This is the key function that handles the incoming messages. Basically the messages themselves contain the info on how should be delivered and scheduled. 
       
       @param receivebuffer The pointer to the start location of the incoming messages.
	@param total The total number of bytes of the incoming messages.
	@return Void.	
*/
void commandHandle(uint8_t * receivebuffer, uint8_t total);

/** @brief This function inits the shell. 
	@return Void.
*/
void InitShell();

/** @brief The buffer for the shell thread. 
*/
extern uint8_t shellbuffer[256];

/** @brief The shell thread function. 
	@return Void.
*/
void ShellThread();

/** @} */
#endif
