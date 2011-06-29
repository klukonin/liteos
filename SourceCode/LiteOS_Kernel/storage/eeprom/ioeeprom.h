/** @file ioeeprom.h
       @brief The functional prototypes of the eeprom byte operations. 

	This file implements the eeprom layer for byte wise operations. 
	@author Qing Charles Cao (cao@utk.edu)
*/

#ifndef IOEEPROMH
#define IOEEPROMH
#include "../../types/types.h"


/** @defgroup eeprom EEPROM Opertions 
*/

/** @{ */


/** @ingroup eeprom
	@brief Read an unsigned 8-bit integer from the eeprom.
	@param addr The address.
	@return The read 8-bit integer. 
*/

uint8_t eeprom_read8uint(uint16_t addr);

/** @ingroup eeprom
	@brief Read a signed 8-bit integer from the eeprom.
	@param addr The address.
	@return The read 8-bit integer. 
*/

int8_t eeprom_read8int(uint16_t addr);

/** @ingroup eeprom
	@brief Read an unsigned 16-bit integer from the eeprom.
	@param addr The address.
	@return The read 16-bit integer. 
*/

uint16_t eeprom_read16uint(uint16_t addr);

/** @ingroup eeprom
	@brief Read a signed 16-bit integer from the eeprom.
	@param addr The address.
	@return The read 16-bit integer. 
*/

int16_t eeprom_read16int(uint16_t addr);

/** @ingroup eeprom
	@brief Read an unsigned 32-bit integer from the eeprom.
	@param addr The address.
	@return The read 32-bit integer. 
*/

uint32_t eeprom_read32uint(uint16_t addr);

/** @ingroup eeprom
	@brief Read a signed 32-bit integer from the eeprom.
	@param addr The address.
	@return The read 32-bit integer. 
*/


int32_t eeprom_read32int(uint16_t addr);


/** @ingroup eeprom
	@brief Write an unsigned 8-bit integer into the eeprom.
	@param addr The address.
	@param value The value of the integer. 
	@return Void. 
	
*/

void eeprom_write8uint(uint16_t addr, uint8_t value);

/** @ingroup eeprom
	@brief Write a signed 8-bit integer into the eeprom.
	@param addr The address.
	@param value The value of the integer. 
	@return Void. 
	
*/

void eeprom_write8int(uint16_t addr, int8_t value);

/** @ingroup eeprom
	@brief Write an unsigned 16-bit integer into the eeprom.
	@param addr The address.
	@param value The value of the integer. 
	@return Void. 
	
*/

void eeprom_write16uint(uint16_t addr, uint16_t value);

/** @ingroup eeprom
	@brief Write a signed 16-bit integer into the eeprom.
	@param addr The address.
	@param value The value of the integer. 
	@return Void. 
*/

void eeprom_write16int(uint16_t addr, int16_t value);

/** @ingroup eeprom
	@brief Write an unsigned 32-bit integer into the eeprom.
	@param addr The address.
	@param value The value of the integer. 
	@return Void. 
	
*/
void eeprom_write32uint(uint16_t addr, uint32_t value);

/** @ingroup eeprom
	@brief Write a signed 32-bit integer into the eeprom.
	@param addr The address.
	@param value The value of the integer. 
	@return Void. 
	
*/

void eeprom_write32int(uint16_t addr, int32_t value);

/** @ingroup eeprom
	@brief Read a block of integers from the eeprom to buffer.
	@param addr The address.
	@param nBytes The number of bytes.
	@param buffer The pointer to the buffer.
	@return Void. 
	
*/

void eeprom_readBytes(uint16_t addr, int nBytes, void *buffer);

/** @ingroup eeprom
	@brief Write a block of integers into the eeprom from buffer.
	@param addr The address.
	@param nBytes The number of bytes.
	@param buffer The pointer to the buffer.
	@return Void. 
	
*/

void eeprom_writeBytes(uint16_t addr, int nBytes, void *buffer);


/** @ingroup eeprom
	@brief Initalize a block of integers in eeprom.
	@param addr The address.
	@param nBytes The number of bytes.
	@param value The init value.
	@return Void. 
	
*/

void eeprom_initBytes(uint16_t addr, int nBytes, uint8_t value);

/**@}*/
#endif
