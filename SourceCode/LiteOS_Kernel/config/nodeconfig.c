/** @file nodeconfig.c
	@brief The implementations of node configurations. 

	@author Qing Charles Cao (cao@utk.edu)
*/

#include "../types/types.h"
#include "nodeconfig.h"
#include "../storage/filesys/storageconstants.h"
#include "../storage/bytestorage/bytestorage.h"

//Global variable!!! 
//This is the id of the current node. Every time it is modified, has to be written back into the 
//bytestorage for synchronization 
//Assuming that bytestorage is non-volatile, then this id could be kept across system reboots. 
//It is not hardcoded into the program so that it can be modifed using user commands 

volatile uint16_t CURRENT_NODE_ID;

//The network name that this node belongs to 

char networkid[17];

//The name of this particular node 

char filenameid[17];

char *node_readnodestring()
{
  return filenameid; 	
}


void node_writenodeid(uint16_t nodeid)
{
    write16uint(NODEIDOFFSET, nodeid);
}

//-------------------------------------------------------------------------
uint16_t node_readnodeid()
{
    return read16uint(NODEIDOFFSET);
}


//-------------------------------------------------------------------------
void node_setradiochannel(uint8_t channel)
{
    write8uint(NODECHANNELOFFSET, channel);
}

//-------------------------------------------------------------------------
uint8_t node_getradiochannel()
{
    return read8uint(NODECHANNELOFFSET);
}

 
