/** @file stdfsa.h
	 @brief 	 This file serves as an middle layer between the file system API provided to the user and and low level operations.
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/

#ifndef STDFSH
#define STDFSH


#include "../../types/types.h"


/** @addtogroup filesystem
*/

/**@{ */

/** @brief Change the directory for a file. 
	@param filename The file name.
	@param directory The directory id. 
	@return The changed directory id.
*/
int changeDirectory(char *filename, int directory);

/** @brief Check if a directory is full block. 
	@param directory The directory id. 
	@return Wehther it is a full block. 
*/
uint8_t fullBlock(int directory);

/** @brief Create a directory. 
	@param filename The file name. 
	@param directory The directory id. 
	@return The created directory id.
*/

uint8_t createDir(char *filename, int directory);

/** @brief Create a file.. 
	@param filename The file name. 
	@param directory The directory id. 
	@return The created file id. 
*/

uint8_t createFileFromDirectory(char *filename, int directory);

/** @brief Get current directory. 
	@return The current directory id.
*/

int getPwd();

/** @brief Set the current directory. 
	@param directory The current directory to be set. 
	@return Void.
*/

void setPwd(int directory);

/** @brief Check if a block exists. 
	@param filename The file name. 
	@param directory The directory id. 
	@return Whether it exists. 
*/

int existBlock(char *filename, int directory);

/** @brief Check if a directory is empty. 
	@param directory The directory id. 
	@return Whether the directory is empty. 
*/

int emptyDirectory(int directory);

/** @brief Locate a file name.
	@param pathname The path name. 
	@param state The current state.
	@return The located file id. 
*/

int locateFileName(char *pathname, int *state);

/** @brief Open a file. 
	@param addr The address. 
	@param fid The file handle. 
	@param mode The opening mode. 
 	@return Void. 
*/

void openFile(int addr, int fid, int mode);

/** @brief Whether an addr is a directory. 
	@param addr The address. 
	@return Whether it is a directory. 
*/

int isDirectory(int addr);

/** @brief Free blocks at an address.
	@param addr The address. 
	@return Void. 
*/

void freeBlocks(int addr);

/** @brief Build a new sector at addr. 
	@param addr The address. 
	@return Void. 
*/

void newSector(int addr);

/** @brief Add child node at an address. 
	@param addr The address
	@param child The child id. 
	@return Void. 
*/

void addChildNode(uint8_t addr, uint8_t child);

/** @brief Remove child node at an address. 
	@param addr The address. 
	@param child The child id. 
	@return Void. 
*/

void removeChildNode(uint8_t addr, uint8_t child);

/** @brief Get name. 
	@param buffer The buffer.
	@param addr The address. 
	@return Void. 
*/

void getName(char *buffer, int addr);

/** @brief Whether a block exists. 
	@param filename The file name. 
	@param directory Directory id. 
	@return Whether it exists. 
*/

int existBlockAddr(char *filename, int directory);

/** @brief Get a real sector. 
	@param addr The address. 
	@param sectornum The sector number. 
	@return Returned sector number. 
*/

uint8_t getRealSector(uint8_t addr, uint8_t sectornum);

/**@}*/
#endif
