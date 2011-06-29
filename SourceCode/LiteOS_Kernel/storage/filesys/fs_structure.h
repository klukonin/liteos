/** @file fs_structure.h
	 @brief This file declares several functions for file system structure. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/



#ifndef FSSTRUCTUREH
#define FSSTRUCTUREH

#include "fsapi.h"

/** @brief The fid table for handles. */

extern fid fidtable[MAX_FILE_TABLE_SIZE];

/** \addtogroup filesystem */

/** @{ */
/** @brief Check whether the file system is present in the system.
	@return Whether the file system is valid.
*/
int checkFsValid();

/** @brief Init the fid table used to keep track of the nodes.
	@return Void. 
*/
void initFidTable();

/** @brief Get free fid. 
	@return Fid integer.
*/
int getFreeFid();

/** @brief Release an fid. 
	@param fid The fid number.
	@return Void.
*/
void releaseFid(int fid);

/** @brief Release all fids.
	@return Void.
*/
void releaseAllFid();

/**@}*/
#endif
