/** @file fsconfig.c
	 @brief This file provides the implementation of functions in the configuration settings for the file system. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/

#include "fsconfig.h"
#include "../bytestorage/bytestorage.h"

//-------------------------------------------------------------------------
uint8_t fsread8uint(int inode, int offset)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    return read8uint(addr);
}

//-------------------------------------------------------------------------
int8_t fsread8int(int inode, int offset)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    return read8int(addr);
}

//-------------------------------------------------------------------------
uint16_t fsread16uint(int inode, int offset)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    return read16uint(addr);
}

//-------------------------------------------------------------------------
int16_t fsread16int(int inode, int offset)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    return read16int(addr);
}

//-------------------------------------------------------------------------
uint32_t fsread32uint(int inode, int offset)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    return read32uint(addr);
}

//-------------------------------------------------------------------------
int32_t fsread32int(int inode, int offset)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    return read32int(addr);
}

//-------------------------------------------------------------------------
void fswrite8uint(int inode, int offset, uint8_t value)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    write8uint(addr, value);
}

//-------------------------------------------------------------------------
void fswrite8int(int inode, int offset, int8_t value)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    write8int(addr, value);
}

//-------------------------------------------------------------------------
void fswrite16uint(int inode, int offset, uint16_t value)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    write16uint(addr, value);
}

//-------------------------------------------------------------------------
void fswrite16int(int inode, int offset, int16_t value)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    write16int(addr, value);
}

//-------------------------------------------------------------------------
void fswrite32uint(int inode, int offset, uint32_t value)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    write32uint(addr, value);
}

//-------------------------------------------------------------------------
void fswrite32int(int inode, int offset, int32_t value)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    write32uint(addr, value);
}

//-------------------------------------------------------------------------
void fsreadBytes(int inode, int offset, int nBytes, void *buffer)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    genericreadBytes(addr, nBytes, buffer);
}

//-------------------------------------------------------------------------
void fswriteBytes(int inode, int offset, int nBytes, void *buffer)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    genericwriteBytes(addr, nBytes, buffer);
}

//-------------------------------------------------------------------------
void fsinitBytes(int inode, int offset, int nBytes, uint8_t value)
{
    uint16_t addr;

    addr = inode * INODESIZE + offset;
    initBytes(addr, nBytes, value);
}
