/* The LiteOS Operating System Kernel */
/*
   The following is the license of LiteOS.
   This file is part of LiteOS.
   Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu
   LiteOS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   LiteOS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 
/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief This file implements some macros that makes the IAR C-compiler and 
 *        avr-gcc work with the same code base for the AVR32 architecture.
 *
 * \par Application note:
 *      AVR2001: AT86RF230 Software Programmers Guide
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler 
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 * 
 * $Name$
 * $Revision: 613 $
 * $RCSfile$
 * $Date: 2006-04-07 14:40:07 +0200 (fr, 07 apr 2006) $  \n
 *
 * Copyright (c) 2006, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#ifndef COMPILER_AVR32_H
#define COMPILER_AVR32_H

#define F_CPU ( 8000000UL )

#if defined( __ICCAVR32__ )

#include <avr32/iouc3a0512.h>
#include <avr32/uc3a0512.h>
#include <intrinsics.h>
#include "crc16.h"

/**
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
