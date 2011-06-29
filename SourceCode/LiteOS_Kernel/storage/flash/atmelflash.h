/** \file atmelflash.h
       \brief The functional prototypes of the flash driver. 

	This file implements the flash driver, which is the foundation for the file system. 
*/


#ifndef _ATMEL_FLASH_H_
#define _ATMEL_FLASH_H_

/** \defgroup flash Flash related hardware drivers.

     These definitons collectively form flash related definitons 
 */
/** @{ */

/** @brief Initialize the flash.
	@return Void.
*/
void atmel_flash_init();


/** @brief Read from flash.
	@param pagenum The page number.
	@param offset The offset. 
	@param buffer The buffer pointer.
	@param NumOfBytes The number of bytes. 
	@return Void. 
*/
void readFlash(int pagenum, uint8_t offset, void *buffer, int NumOfBytes);

/** @brief Write into flash.
	@param pagenum The page number.
	@param offset The offset. 
	@param buffer The buffer pointer.
	@param NumOfBytes The number of bytes. 
	@return Void.
*/
void writeFlash(int pagenum, uint8_t offset, void *buffer, int NumOfBytes);

/** @brief The size of pages in flash.
	@return The size of pages.
*/
inline uint16_t atmel_flash_pagesize();

/** @brief The page number of the flash.
	@return The number of pages. 
*/

inline uint16_t atmel_flash_pagenumber();


/** @brief Copy one page to another.
	@param sourcepage The source of the page.
	@param targetpage The target of the page.
	@return Void.
*/
void copyFlash(int sourcepage, int targetpage);

/** @brief Compare the contents of the page. 
	@param p The comparison pointer.
	@param count The number of bytes.
	@return 0 or 1.
*/
uint8_t atmel_flash_compare(uint8_t * p, uint16_t count);

/** @brief Compute the crc from the current flash address, for count bytes.
	@param count The count number.
	@return The CRC calculated. 
*/
uint16_t atmel_flash_crc(uint32_t count);

/** @} */
#endif
