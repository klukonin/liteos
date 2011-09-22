/** @file compiler_avr.h
 *********************************************************************
 *
 * @brief This file implements some macros that makes the IAR C-compiler and 
 *        avr-gcc work with the same code base for the AVR architecture.
 *
 * @par Application note:
 *      AVR2001: AT86RF230 Software Programmer's Guide
 *
 * @par Documentation
 *      For comprehensive code documentation, supported compilers, compiler 
 *      settings and supported devices see readme.html
 *
 * @author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 * 
 */

#ifndef COMPILER_AVR_H
#define COMPILER_AVR_H

/** @brief 
    This macro will protect the following code from interrupts.*/
#define AVR_ENTER_CRITICAL_REGION( ) {uint8_t volatile saved_sreg = SREG; cli( );

/** @brief 
	This macro must always be used in conjunction with AVR_ENTER_CRITICAL_REGION
    so that interrupts are enabled again.*/
#define AVR_LEAVE_CRITICAL_REGION( ) SREG = saved_sreg;}

#if defined( __ICCAVR__ )

#include <inavr.h>
#include <ioavr.h>
#include <intrinsics.h>

#include "crc16.h"

#define FLASH_DECLARE(x) __flash x
#define FLASH_DECLARE(x) __flash x
#define FLASH_STRING(x) ((__flash const char *)(x))
#define PGM_READ_BYTE(x) *(x)
/**
   @brief Perform a delay of \c us microseconds.

   The macro F_CPU is supposed to be defined to a constant defining the CPU 
   clock frequency (in Hertz).

   The maximal possible delay is 262.14 ms / F_CPU in MHz.

   @note For the IAR compiler, currently F_CPU must be a
   multiple of 1000000UL (1 MHz).
 */
#define delay_us( us )   ( __delay_cycles( ( F_CPU / 1000000UL ) * ( us ) ) )

/*
 * Some preprocessor magic to allow for a header file abstraction of
 * interrupt service routine declarations for the IAR compiler.  This
 * requires the use of the C99 _Pragma() directive (rather than the
 * old #pragma one that could not be used as a macro replacement), as
 * well as two different levels of preprocessor concetanations in
 * order to do both, assign the correct interrupt vector name, as well
 * as construct a unique function name for the ISR.
 *
 * Do *NOT* try to reorder the macros below, or you'll suddenly find
 * out about all kinds of IAR bugs...
 */
#define PRAGMA(x) _Pragma( #x )
#define ISR(vec) PRAGMA( vector=vec ) __interrupt void handler_##vec(void)
#define sei( ) (__enable_interrupt( ))
#define cli( ) (__disable_interrupt( ))

#define watchdog_reset( ) (__watchdog_reset( ))

#define INLINE PRAGMA( inline=forced ) static

#elif defined( __GNUC__ )

#include <avr/io.h>
#include <avr/interrupt.h>
# include <avr/pgmspace.h>

#include <util/crc16.h>
#include <util/delay.h>

#define delay_us( us )   (_delay_us( us ))

#define INLINE static inline
#define crc_ccitt_update( crc, data ) _crc_ccitt_update( crc, data )

#define __x 
#define __z 

#else
#error Compiler not supported.
#endif
#endif
