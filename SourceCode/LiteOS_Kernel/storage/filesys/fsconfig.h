/** @file fsconfig.h
	 @brief This file declares the configuration settings for the file system. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/


#ifndef FSCONFIGH
#define FSCONFIGH
#include "../../types/types.h"

/** \ingroup filesystem 
*/

/** @{ */

/** @brief The root node of the file system. */
#define FSROOTNODE 0

/** @brief The size of the inode. */
#define INODESIZE 32

/** @brief The total number of inodes. */
#define INODENUM 96

/**@}*/

/** \addtogroup filesystem */

/**@{*/

/** @brief The relative position from next to current. */

#define relativeAddr(current,next) (((next)-(current)+INODENUM)%INODENUM)


/** @brief The absolute addr from using current and the offset. */
#define absoluteAddr(current,offset) ((current) + (offset) > INODENUM ? (\
(current) + (offset) - INODENUM):((current) + (offset)))

/** @} */

/** \ingroup filesystem */

/**@{ */
/** @brief Node type structures. */
     enum
     {
         DIRNODE = 1, FILENODE = 2, DEVNODE = 3, APPNODE = 4,
     };

/** @brief The file name offset. */
#define FILENAMEOFFSET 0

/** @brief The offset of the type. */
#define TYPEOFFSET 12

/** @brief The valid bit offset. */
#define VALIDOFFSET 13

/** @brief There are four types of files defined, directory, file, device, and user app. This is the definition of the directory node.  */
struct directorynode
{
    char name[12];
    uint8_t type;
    uint8_t valid;
    uint8_t addr_subdir[10];
    uint8_t next;
    uint8_t protect_rwx[3];
    uint8_t uid;
    uint16_t size;
    uint8_t parent;
};

#define DIR_ADDRSUBOFFSET 14
#define DIR_EXTRAOFFSET 24
#define DIR_NEXTOFFSET 24
#define DIR_PROTECTOFFSET 25
#define DIR_UIDOFFSET 28
#define DIR_SIZEOFFSET 29
#define DIR_PARENTOFFSET 31

/** @brief The definition of the file node.  */

struct filenode
{
    char name[12];
    uint8_t type;
    uint8_t valid;
    uint8_t addr_page[8];
    uint16_t endpageuse;
    uint8_t next;
    uint8_t protect_rwx[3];
    uint8_t uid;
    uint16_t size;
    uint8_t parent;
};

#define FILE_ADDRPAGEOFFSET 14
#define FILE_ENDPAGEUSEOFFSET 22
#define FILE_EXTRAOFFSET 24
#define FILE_NEXTOFFSET 24
#define FILE_PROTECTOFFSET 25
#define FILE_UIDOFFSET 28
#define FILE_SIZEOFFSET 29
#define FILE_PARENTOFFSET 31

/** @brief The definition of the app node.  */
struct appnode
{
    char name[12];
    uint8_t type;
    uint8_t valid;
    uint32_t entrypoint;
    uint8_t padding[7];
    uint8_t protect_rwx[3];
    uint8_t uid;
    uint16_t size;
    uint8_t parent;
};

#define APP_ENTRYOFFSET 14
#define APP_ENDPAGEUSEOFFSET 22
#define APP_EXTRAOFFSET 25
#define APP_PROTECTOFFSET 25
#define APP_UIDOFFSET 28
#define APP_SIZEOFFSET 29
#define APP_PARENTOFFSET 31

/** @brief The definition of the device node.  */

struct devnode
{
    char name[12];
    uint8_t type;
    uint8_t valid;
    uint16_t programentrypoint;
    uint16_t ramparameterentrypoint;
    uint16_t ramreturnentrypoint;

    uint8_t padding[5];
    uint8_t protect_rwx[3];
    uint8_t uid;
    uint16_t size;
    uint8_t parent;
};

#define DEV_PROGRAMENTRYOFFSET 14
#define DEV_RAMENTRYOFFSET 16
#define DEV_PROTECTOFFSET 25
#define DEV_UIDOFFSET 28
#define DEV_SIZEOFFSET 29
#define DEV_PARENTOFFSET 31

/**@}*/


/** @brief Read 8-bit from inode. 
	@param inode The inode number.
	@param offset The offset.
	@return Success or not. 
*/

uint8_t fsread8uint(int inode, int offset);

/** @brief Read 8-bit from inode. 
	@param inode The inode number.
	@param offset The offset.
	@return Success or not. 
*/

int8_t fsread8int(int inode, int offset);

/** @brief Read 16-bit from inode. 
	@param inode The inode number.
	@param offset The offset.
	@return Success or not. 
*/

uint16_t fsread16uint(int inode, int offset);

/** @brief Read 16-bit from inode. 
	@param inode The inode number.
	@param offset The offset.
	@return Success or not. 
*/

int16_t fsread16int(int inode, int offset);

/** @brief Read 32-bit from inode. 
	@param inode The inode number.
	@param offset The offset.
	@return Success or not. 
*/

uint32_t fsread32uint(int inode, int offset);

/** @brief Read 32-bit from inode. 
	@param inode The inode number.
	@param offset The offset.
	@return Success or not. 
*/

int32_t fsread32int(int inode, int offset);

/** @brief Write 8-bit into inode. 
	@param inode The inode number.
	@param offset The offset.
	@param value The value. 
	@return Success or not. 
*/

void fswrite8uint(int inode, int offset, uint8_t value);

/** @brief Write 8-bit into inode. 
	@param inode The inode number.
	@param offset The offset.
	@param value The value. 
	@return Success or not. 
*/

void fswrite8int(int inode, int offset, int8_t value);

/** @brief Write 16-bit into inode. 
	@param inode The inode number.
	@param offset The offset.
	@param value The value. 
	@return Success or not. 
*/

void fswrite16uint(int inode, int offset, uint16_t value);

/** @brief Write 16-bit into inode. 
	@param inode The inode number.
	@param offset The offset.
	@param value The value. 
	@return Success or not. 
*/

void fswrite16int(int inode, int offset, int16_t value);

/** @brief Write 32-bit into inode. 
	@param inode The inode number.
	@param offset The offset.
	@param value The value. 
	@return Success or not. 
*/

void fswrite32uint(int inode, int offset, uint32_t value);

/** @brief Write 32-bit into inode. 
	@param inode The inode number.
	@param offset The offset.
	@param value The value. 
	@return Success or not. 
*/

void fswrite32int(int inode, int offset, int32_t value);

/** @brief Read bytes from inode into buffer. 
	@param inode The inode number.
	@param offset The offset.
	@param nBytes The number of bytes.
	@param buffer The pointer to the buffer. 
	@return Success or not. 
*/


void fsreadBytes(int inode, int offset, int nBytes, void *buffer);

/** @brief Write bytes into inode from buffer. 
	@param inode The inode number.
	@param offset The offset.
	@param nBytes The number of bytes.
	@param buffer The pointer to the buffer. 
	@return Success or not. 
*/


void fswriteBytes(int inode, int offset, int nBytes, void *buffer);

/** @brief Init the inode using value. 
	@param inode The inode number.
	@param offset The offset.
	@param nBytes The number of bytes.
	@param value The value for initilization. 
	@return Success or not. 
*/


void fsinitBytes(int inode, int offset, int nBytes, uint8_t value);
#endif
