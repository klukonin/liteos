/** @file nodeconfig.h
	@brief The declarations of node configurations. 

	@author Qing Charles Cao (cao@utk.edu)
*/


#ifndef NODECONFIGH
#define NODECONFIGH
#include "../types/types.h"


/** \defgroup  nodeconfig Node configuration module. 

*/

/** @{ */

/** @brief The current node id. 
*/
extern volatile uint16_t CURRENT_NODE_ID;

/** 
@brief The network name that this node belongs to.
*/

extern char networkid[17];

/**  
@brief The file name that this node belongs to.
*/

extern char filenameid[17];


/**
@brief Write the node id into byte storage.
@param nodeid The nodeid to be written into storage. 
@return Void. 
*/
void node_writenodeid(uint16_t nodeid);

/** 
@brief Read the node id from byte storage.
@return The node id read from the storage. 
*/
uint16_t node_readnodeid();

/**
 @brief Read node name. 
 */
char *node_readnodestring();


/** @brief Write the channel of the current node into byte storage.
	@param channel The channel to be written.
	@return Void. 
*/
void node_setradiochannel(uint8_t channel);

/** @brief Get the node channel from byte storage.
	@return The channel number. 
*/
uint8_t node_getradiochannel();

/** @} */
#endif
