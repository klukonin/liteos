/** @file libsystem.h
       @brief The functional declaration for the system configuration API. 

       @author  Qing Cao (cao@utk.edu) 
       
*/

#ifndef SYSTEMH
#define SYSTEMH

#include "liteoscommon.h"



/** @addtogroup api
*/

/** @{
*/



/** @brief Get a random number.
       @return The generated random number.
*/

int lib_random();


/** @brief  Get node ID
       @return The current node ID
*/

uint16_t lib_get_node_id();

/** @brief  Set node ID.
       @param nodeid The node id. 
       @return Void. 
*/


void lib_set_node_id(uint16_t nodeid);

/** @brief  Get current timestamp. 
       @return The current timestamp. 
*/


uint32_t get_current_timestamp();

/** @}
*/


#ifdef PLATFORM_CPU_MEASURE
uint16_t getKernelUtilization(); 
#endif

#endif
