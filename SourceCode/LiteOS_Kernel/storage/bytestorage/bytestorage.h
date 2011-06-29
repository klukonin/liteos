/** @file bytestorage.h
       @brief The functional prototypes of the byte storage layer. 

	This file implements the byte storage layer, which is implemented on top of the eeprom. 
	@author Qing Charles Cao (cao@utk.edu)
*/



#ifndef BYTESTORAGEH
#define BYTESTORAGEH

/**@defgroup bytestorage Byte storage hardware independent abstraction.
*/
#include "../../types/types.h"

/** @ingroup bytestorage 
	@brief Read bytes from the byte storage. 
	@param addr The address.
	@param nBytes The number of bytes.
	@param  buffer The target buffer.
	@return Void. 
*/
void genericreadBytes(uint16_t addr, int nBytes, void *buffer);


/** @ingroup bytestorage 
	@brief Write bytes to the byte storage. 
	@param addr The address.
	@param nBytes The number of bytes.
	@param  buffer The target buffer.
	@return Void. 
*/
void genericwriteBytes(uint16_t addr, int nBytes, void *buffer);



/** @ingroup bytestorage 
	@brief Init the byte storage section. 
	@param addr The address.
	@param nBytes The number of bytes.
	@param  value The initialization value. 
	@return Void. 
*/

void initBytes(uint16_t addr, int nBytes, uint8_t value);

/** @ingroup bytestorage 
	@brief Read an unsigned 8-bit integer from the byte storage.
	@param addr The address.
	@return The read 8-bit integer. 
*/

uint8_t read8uint(uint16_t addr);

/** @ingroup bytestorage 
	@brief Read an signed 8-bit integer from the byte storage.
	@param addr The address.
	@return The read 8-bit integer. 
*/

int8_t read8int(uint16_t addr);

/** @ingroup bytestorage 
	@brief Read an unsigned 16-bit integer from the byte storage.
	@param addr The address.
	@return The read 16-bit integer. 
*/

uint16_t read16uint(uint16_t addr);

/** @ingroup bytestorage 
	@brief Read an signed 16-bit integer from the byte storage.
	@param addr The address.
	@return The read 16-bit integer. 
*/

int16_t read16int(uint16_t addr);

/** @ingroup bytestorage 
	@brief Read an unsigned 32-bit integer from the byte storage.
	@param addr The address.
	@return The read 32-bit integer. 
*/

uint32_t read32uint(uint16_t addr);

/** @ingroup bytestorage 
	@brief Read a signed 32-bit integer from the byte storage.
	@param addr The address.
	@return The read 32-bit integer. 
*/

int32_t read32int(uint16_t addr);

/** @ingroup bytestorage 
	@brief Write an unsigned 8-bit integer from the byte storage.
	@param addr The address.
	@param value The value. 
	@return Void. 
*/

void write8uint(uint16_t addr, uint8_t value);

/** @ingroup bytestorage 
	@brief Write an signed 8-bit integer from the byte storage.
	@param addr The address.
	@param value The value. 
	@return Void. 
*/

void write8int(uint16_t addr, int8_t value);

/** @ingroup bytestorage 
	@brief Write an unsigned 16-bit integer from the byte storage.
	@param addr The address.
	@param value The value. 
	@return Void. 
*/

void write16uint(uint16_t addr, uint16_t value);

/** @ingroup bytestorage 
	@brief Write an signed 16-bit integer from the byte storage.
	@param addr The address.
	@param value The value. 
	@return Void. 
*/

void write16int(uint16_t addr, int16_t value);

/** @ingroup bytestorage 
	@brief Write an unsigned 32-bit integer from the byte storage.
	@param addr The address.
	@param value The value. 
	@return Void. 
*/

void write32uint(uint16_t addr, uint32_t value);

/** @ingroup bytestorage 
	@brief Write an signed 32-bit integer from the byte storage.
	@param addr The address.
	@param value The value. 
	@return Void. 
*/

void write32int(uint16_t addr, int32_t value);


#endif
