/** @file bytestorage.c
       @brief The functional implementation of the byte storage layer. 

	This file implements the byte storage layer, which is implemented on top of the eeprom. 
	@author Qing Charles Cao (cao@utk.edu)
*/


#include "bytestorage.h"

//-------------------------------------------------------------------------
#ifdef PLATFORM_AVR
#include "../eeprom/ioeeprom.h"
void genericreadBytes(uint16_t addr, int nBytes, void *buffer)
{
    eeprom_readBytes(addr, nBytes, buffer);
}

//-------------------------------------------------------------------------
void genericwriteBytes(uint16_t addr, int nBytes, void *buffer)
{
    eeprom_writeBytes(addr, nBytes, buffer);
}

//-------------------------------------------------------------------------
void initBytes(uint16_t addr, int nBytes, uint8_t value)
{
    eeprom_initBytes(addr, nBytes, value);
}

//-------------------------------------------------------------------------
uint8_t read8uint(uint16_t addr)
{
    return eeprom_read8uint(addr);
}

//-------------------------------------------------------------------------
int8_t read8int(uint16_t addr)
{
    return eeprom_read8int(addr);
}

//-------------------------------------------------------------------------
uint16_t read16uint(uint16_t addr)
{
    return eeprom_read16uint(addr);
}

//-------------------------------------------------------------------------

int16_t read16int(uint16_t addr)
{
    return eeprom_read16int(addr);
}

//-------------------------------------------------------------------------
uint32_t read32uint(uint16_t addr)
{
    return eeprom_read32uint(addr);
}

//-------------------------------------------------------------------------
int32_t read32int(uint16_t addr)
{
    return eeprom_read32int(addr);
}

//-------------------------------------------------------------------------
void write8uint(uint16_t addr, uint8_t value)
{
    eeprom_write8uint(addr, value);
}

//-------------------------------------------------------------------------
void write8int(uint16_t addr, int8_t value)
{
    eeprom_write8int(addr, value);
}

//-------------------------------------------------------------------------
void write16uint(uint16_t addr, uint16_t value)
{
    eeprom_write16uint(addr, value);
}

//-------------------------------------------------------------------------
void write16int(uint16_t addr, int16_t value)
{
    eeprom_write16int(addr, value);
}

//-------------------------------------------------------------------------
void write32uint(uint16_t addr, uint32_t value)
{
    eeprom_write32uint(addr, value);
}

//-------------------------------------------------------------------------
void write32int(uint16_t addr, int32_t value)
{
    eeprom_write32int(addr, value);
}

//-------------------------------------------------------------------------
#endif
