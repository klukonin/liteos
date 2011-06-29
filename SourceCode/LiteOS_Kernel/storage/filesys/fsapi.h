/** @file fsapi.h
	 @brief This file declares the functions for the global level file system API. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/

 
#ifndef FSAPIH
#define FSAPIH
#include "../../types/types.h"


/**@defgroup filesystem File system module. */

/** @{ */

/** @brief File handle. */

typedef struct
{
    uint8_t valid;
    uint8_t index;
    uint8_t addr;
    uint8_t mode;
    uint16_t size;
    int32_t fpos;
} fid, *fidptr, MYFILE;


/** @brief Open a file. 
	This function opens a file according to a pathname and the mode. The file opened must be a file, not program, device driver and directory.
	@param pathname The path of the file. 
	@param mode Opening mode of the file. 
	@return The file handle. 
*/
MYFILE *fsopen(char *pathname, char *mode);

/** @brief Close a file.
	@param fp The file handle.
	@return Void. 
*/
void fclose2(MYFILE * fp);

/** @brief This function changes teh current location of fpos parameter.
	@param fp The file handle. 
	@param offset The offset.
	@param position The position. 
	@return 0 or 1 as integer.
*/

int fseek2(MYFILE * fp, int32_t offset, int position);

/** @brief This function checks if the file specificed by pathname exists or not.
	@param pathname The pathname of the file.
	@return -1 if not exist, or the block id of the file if it does exist.
*/
int fexist2(char *pathname);

/** @brief This function creates a directory as specified by the pathname parameter.
	@param pathname The pathname of the directory.
	@return Success or not. 
*/
int fcreatedir2(char *pathname);

/** @brief Deletes a file.
	Right now this is implemented in a simple way. Bsaically it checks the pathname and see if the file is there. If it is, then it deletes the block of the file.  
	@param pathname The pathname to the directory or the file. 
	@return Success or not. 
*/
int fdelete2(char *pathname);

/** @brief Read a number of bytes into the buffer. 
	@param fp The file handle. 
	@param buffer The buffer.
	@param nBytes The number of bytes.
	@return Success or not. 
*/
int fread2(MYFILE * fp, void *buffer, int nBytes);

/** @brief Write a number of bytes into the buffer. 
	@param fp The file handle. 
	@param buffer The buffer.
	@param nBytes The number of bytes.
	@return Success or not. 
*/

int fwrite2(MYFILE * fp, void *buffer, int nBytes);

/** @brief Move a file.
	@param source The source file.
	@param target The target file.
	@return Success or not. 
*/
int fmove(char *source, char *target);

/** @brief Copy files.
	@param source The source file.
	@param target target The target file.
	@return Success or not. 
*/
int fcopy(char *source, char *target);

/** @brief Print all file system information.
	@return Void. 
*/

void fsprintall();

/**	@brief Change dir based on directory information.
	@param path The path information.
	@return Void. 
*/
void fchangedir(char *path);

/**	@brief Get the current directory information.
	@param buffer The buffer of the target.
	@param size The size of the buffer.
	@return Void. 
*/
void fcurrentdir(char *buffer, int size);

/** @brief Get the addr location file info used for the ls command.
	@param buffer The buffer information.
	@param addr The address.
	@return Void.
*/
void finfonode(char *buffer, int addr);

/** @brief Search for file information.
	@param addrlist The searched information.
	@param size The total size.
	@param string The string information.
	@return Void. 
*/
void fsearch(uint8_t * addrlist, uint8_t * size, char *string);

/** @brief Get node file information.
	@param buffer The buffer.
	@param size The size of the buffer.
	@param addr The address information.
	@return Void. 
*/
void fdirnode(char *buffer, int size, int addr);

/** @brief Format the whole file system.
	@return Void. 
*/
void formatSystem();

//-------------------------------------------------------------------------
/** @brief Return the allocated EEPROM. 
	@return The allocated blocks in EEPROM.
*/
int fcheckFSAllocation();

//-------------------------------------------------------------------------
/** @brief Return the allocated storage.
	@return The allocated blocks in storage. 
*/
int fcheckStorageAllocation();

/** @} */
#endif
