/** @file memorylogger.h
	@brief The functional declaration of the memory reporting service. 

	@author Qing Cao (cao@utk.edu)
*/




#ifndef MEMORYLOGGERH
#define MEMORYLOGGERH

#include "../types/types.h"

#ifdef TRACE_MEMORY_CONTENTS
 
/** @defgroup logging Logging and reporting module. 
    @{
*/

/** @brief Init the memory logging module with interval. 
	@param basechannel The base channel used. 
	@param reportingchannel The channel used.
	@param reportinterval The interval for reporting. 
	@return Void. 
*/
void initMemoryReporting(uint16_t reportinterval); 

/** @brief Start a new cycle for sending out multiple packets for memory contents. 
	@return Void.
*/

void memoryInitTimerFired();


/** @brief Report the internal memory contents based on the current context. 
	@return Void. 
*/
void memoryReportTimerFired();

/** @} */

#endif 
#endif
