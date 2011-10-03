/** @file commandprocessor.h
	@brief The declarations of the command processor module for serial port operations. 

	@author Qing Charles Cao (cao@utk.edu)
*/


#ifndef COMMANDPROCESSORH
#define COMMANDPROCESSORH

#include "../types/types.h"
#include "../io/radio/amcommon.h"
#include "../io/avrserial/serialprint.h"

#ifdef BASE_MODE
void broadcastCommand(uint8_t *receivebuffer, uint8_t length);
Radio_MsgPtr Broadcast2SerialAlternative(Radio_MsgPtr msg);
#endif 

#endif 