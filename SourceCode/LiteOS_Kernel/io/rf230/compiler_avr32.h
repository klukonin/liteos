/** @file compiler_avr32.h
 *
 * @brief This file implements some macros that makes the IAR C-compiler and 
 *        avr-gcc work with the same code base for the AVR32 architecture.
 *
 * @par Application note:
 *      AVR2001: AT86RF230 Software Programmers Guide
 *
 * @par Documentation
 *      For comprehensive code documentation, supported compilers, compiler 
 *      settings and supported devices see readme.html
 *
 * @author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 */

#ifndef COMPILER_AVR32_H
#define COMPILER_AVR32_H

#define F_CPU ( 8000000UL )

#if defined( __ICCAVR32__ )

#include <avr32/iouc3a0512.h>
#include <avr32/uc3a0512.h>
#include <intrinsics.h>
#include "crc16.h"

/**
   @brief
   Perform a delay of \c us microseconds.

   The macro F_CPU is supposed to be defined to a constant defining the CPU 
   clock frequency (in Hertz).

   The maximal possible delay is 262.14 ms / F_CPU in MHz.

   \note For the IAR compiler, currently F_CPU must be a
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
#define sei( ) __enable_interrupt( )
#define cli( ) __disable_interrupt( )

#define INLINE PRAGMA( inline=forced ) static
//#define CRC_CCITT_UPDATE( crc, data ) crc_ccitt_update( crc, data )
#define ENTER_CRITICAL_REGION( ) __enable_interrupt( )
#define LEAVE_CRITICAL_REGION( ) __disable_interrupt( )

#elif defined( __GNUC__ )

#include <avr32/io.h>
#include <avr32/interrupt.h>

#include <util/crc16.h>
#include <util/delay.c>

#define INLINE static inline
#define crc_ccitt_update( crc, data ) _crc_ccitt_update( crc, data )

#else
#error Compiler not supported.
#endif
#endif
