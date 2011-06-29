/** @file bootloader.h 
	@brief The functional prototypes of the bootloader.

	@author Qing Charles Cao (cao@utk.edu)
*/

#ifndef BOOTLOADERH
#define BOOTLOADERH

#include "../types/types.h"
#include "../types/byteorder.h"

/** \defgroup bootloader Bootloader operations.

\par Overview
Bootloader operations handle the rewriting of flash, and is used to implement dynamic loading, breakpoints, among other program flash related tasks. 
This part describes its basic interface and how it is implemented 

\par Implementation and Usage
The bootloader API of LiteOS is designed for use with the file system. Normally, we expect that the kernel will read strings out of a file, usually with limited
number of bytes, then write such bytes into flash. This is basic mechanism that we consider. 

To use, there are four functions that are related. First, the user should set which page to write by invoking ProgramSetPage(). This function sets the correct page
number for the application. Then, the user repeatedly invokes ProgramString() to write new strings into this flash. The string is expected to be strings like A0B0, etc. 
When the last string is written, the user invokes ProgramLastString() to write the last string into the internal buffer. Or the user may invoke ProgramRemains to write the 
remaining part into the flash as well. 

\note The functions boot_program_page(), reprogram(), reprogramall() are expected to be internal and not to be used. 
*/

/** @{ */

/** @brief Program a page accoridng to the page and buffer information. 

 *    \param page The page number, represented by the byte address of the first byte in this page. 
 *    \param buf The buffer that contains the data to be written into the flash. 
 *    \return Void. 
 */
 
void boot_program_page(uint32_t page, uint8_t * buf)
    __attribute__ ((section(".bootloader")));

/** @brief Reprogram a page accoridng to the page and buffer information.

 *    \param buffer The buffer for the code.
 *    \param codepage Codepage number.
 *    \return Whether the operation is successful.  
 */
 
int reprogram(unsigned char *buffer, unsigned int codepage)
    __attribute__ ((section(".bootloader")));


/** @brief Reprogram using program information from a varialbe \c program, which is for debugging purposes.
 *    \param program The content is the program hex file.
 *    \return Whether the operation is successful.
 */
int reprogramall(unsigned char *program)
    __attribute__ ((section(".bootloader")));

/** @brief Reprogram using the pageno.
 	\param pageno The page number.
	\return Void.
*/

void ProgramSetPage(uint16_t pageno) __attribute__ ((section(".bootloader")));

/** @brief Reprogram a string into the buffer. Only handles 16 bytes of string each time.
       @param buffer The buffer for data. 
       @return Void. 
*/
void ProgramString(char *buffer) __attribute__ ((section(".bootloader")));

/** \brief Reprogram a string into the buffer , which is the last string. Therefore, this function calls reprogramall directly.
 	@param buffer The buffer for data.
 	@param length The length of the data. 
 	@return Void. 
*/
void ProgramLastString(char *buffer, uint8_t length)
    __attribute__ ((section(".bootloader")));

/** \brief Reprogram the remain part of a buffer.
	@return Void. 
*/
void ProgramRemains() __attribute__ ((section(".bootloader")));


/** @} */

/** \defgroup debugging_tracing Debugging and tracing operations.

/** @{ */
/** \brief Insert a breakpoint using instructions received by radio.
 	\param pagenum The page number of the breakpoint.
 	\param pageoffset The page offset of the breakpoint.
 	\param instructions The set of instructions to be inserted.
 	\return Void. 
 
*/
void boot_insertBreakPoint(uint16_t pagenum, uint8_t pageoffset, uint8_t *
                           instructions)
    __attribute__ ((section(".bootloader")));

/** \brief Remove a breakpoint using instructions received by radio.
	\param pagenum The page number of the breakpoint.
 	\param pageoffset The page offset of the breakpoint.
 	\param instructions The set of instructions to be inserted.
 	\return Void.
*/
void boot_removeBreakPoint(uint16_t pagenum, uint8_t pageoffset, uint8_t *
                           instructions)
    __attribute__ ((section(".bootloader")));

/** \brief Insert a tracepoint. 
	\param pagenum The page number.
 	\param pageoffset The page offset.
 	\return Void.
*/

void boot_insertTracePoint(uint16_t pagenum, uint8_t pageoffset)
    __attribute__ ((section(".bootloader")));

/** \brief Insert a long tracepoint. 
	\param pagenum The page number.
 	\param pageoffset The page offset.
 	\return Void.
*/


void boot_insertTracePointLong(uint16_t pagenum, uint8_t pageoffset)
    __attribute__ ((section(".bootloader")));

/** \brief Remove a tracepoint. 
	\param pagenum The page number.
 	\param pageoffset The page offset.
 	\param buffer The buffer. 
 	\return Void.
*/


void boot_removeTracePoint(uint16_t pagenum, uint8_t pageoffset,
                           uint8_t * buffer)
    __attribute__ ((section(".bootloader")));

/** \brief Remove a long tracepoint. 
	\param pagenum The page number.
 	\param pageoffset The page offset.
 	\param buffer The buffer. 
 	\return Void.
*/

void boot_removeTracePointLong(uint16_t pagenum, uint8_t pageoffset,
                               uint8_t * buffer)
    __attribute__ ((section(".bootloader")));

/**@}*/
#endif
