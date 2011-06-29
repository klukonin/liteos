/**  @file storageconstants.h
 	 @brief 	 This file serves as the header definitions for the storage constants. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/


#ifndef STORAGECONSTANTSH
#define STORAGECONSTANTSH

/** @addtogroup filesystem */

/** @{ */

/** Network name offset. */
#define NETWORKNAMEOFFSET 3104

/** Node file name offset. */
#define NODEFILENAMEOFFSET    3125

/** Node id offset. */
#define NODEIDOFFSET 3150

/** Node status offset. */
#define NODESTATUSOFFSET 3154

/** Node channel offset. */
#define NODECHANNELOFFSET  3156

/** EEPROM vector contains 12 bytes. */
#define EEPROMVECTORSTART 3138

/** flash vector contains 32 bytes. */
#define FLASHVECTORSTART  3150

/** @} */

#endif 
