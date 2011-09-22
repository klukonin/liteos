/** @file crc16.h
 *
 * @brief
 *      This file implements the API for the implemented CCITT 16-bit CRC used 
 *      by IEEE 802.15.4. This is function is included since IAR lacks this 
 *      implementation. For avr-gcc it is available in avr-libc: <util/crc16.h>
 *
 * @par Application note:
 *      AVR2001: AT86RF230 Software Programmer's Guide.
 *
 * @par Documentation
 *      For comprehensive code documentation, supported compilers, compiler 
 *      settings and supported devices see readme.html
 *
 * @author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 */

#ifndef CRC_16_H
#define CRC_16_H
/*============================ INCLUDE =======================================*/
#include "../../types/types.h"

/*============================ MACROS ========================================*/
/*============================ TYPDEFS =======================================*/
/*============================ PROTOTYPES ====================================*/
#if defined( __ICCAVR__ )
uint16_t crc_ccitt_update( uint16_t crc, uint8_t data );
#endif /* __ICCAVR__ */
#endif
/*EOF*/
