/** @file pagestorage.h
       @brief The functional prototypes of the page storage. 

	This file implements the generic layer for page storage. 
	@author Qing Charles Cao (cao@utk.edu)
*/

#ifndef PAGESTORAGEH
#define PAGESTORAGEH

/**@defgroup pagestorage Hardware-independent page storage abstraction.*/

#include "../../types/types.h"

/** @ingroup pagestorage 
	@brief Get the size of a storage page.
	@return The size of the storage page. 
*/
uint16_t getpagesize();

/** @ingroup pagestorage
	@brief Get the total number of pages.
	@return The total number of pages. 
*/
uint16_t getpagenumber();

/** @ingroup pagestorage 
	@brief Init the page storage.
	@return Void. 
*/
void pagestorageinit();

/** @ingroup pagestorage 
	@brief Read from a page.
	@param pagenum The page number.
	@param offset The offset in a page. 
	@param buffer The source buffer.
	@param NumOfBytes The total number of bytes to be read. 
*/
void readpagestorage(int pagenum, uint8_t offset, void *buffer,
                     int NumOfBytes);

/** @ingroup pagestorage 
	@brief Write to a page. 
	@param pagenum The page number.
	@param offset The offset in a page. 
	@param buffer The desintation buffer.
	@param NumOfBytes The total number of bytes to be written. 
*/
void writepagestorage(int pagenum, uint8_t offset, void *buffer, int
                      NumOfBytes);

/** @ingroup pagestorage 
	@brief Copy a page to another. 
	@param sourcepage The source page. 
	@param targetpage The target page. 
	@return Void. 
*/					 
void copyPage(int sourcepage, int targetpage);                     
#endif
