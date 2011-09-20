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
 * \brief This file includes the correct HAL given the selected architecture.
 *
 * \par Application note:
 *      AVR2001: AT86RF230 Software Programmer's Guide
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
#ifndef HAL_H
#define HAL_H
/*============================ INCLUDE =======================================*/
#if defined( AVR )
        #include "hal_avr.h"
    #elif defined( AVR32 )
        #error Architecture not supported yet.
    #elif defined( SAM7 )
        #include "hal_sam7.h"
        //#error Architecture not supported yet.
    #else
        #error This files should only be compiled with the ARCHITECTURE set.
    #endif

//#include <stdint.h>
//#include <stdbool.h>

#include "compiler.h"

//#include "../scheduling.h"
#include "../../io/radio/amcommon.h"
#include "../../io/radio/amradio.h"
#include "../../types/types.h"

/*============================ MACROS ========================================*/
#define HAL_BAT_LOW_MASK       ( 0x80 ) //!< Mask for the BAT_LOW interrupt.
#define HAL_TRX_UR_MASK        ( 0x40 ) //!< Mask for the TRX_UR interrupt.
#define HAL_TRX_END_MASK       ( 0x08 ) //!< Mask for the TRX_END interrupt.
#define HAL_RX_START_MASK      ( 0x04 ) //!< Mask for the RX_START interrupt.
#define HAL_PLL_UNLOCK_MASK    ( 0x02 ) //!< Mask for the PLL_UNLOCK interrupt.
#define HAL_PLL_LOCK_MASK      ( 0x01 ) //!< Mask for the PLL_LOCK interrupt.

#define HAL_MIN_FRAME_LENGTH   ( 0x03 ) //!< A frame should be at least 3 bytes.
#define HAL_MAX_FRAME_LENGTH   ( 0x64 ) // A frame is extactly 100 bytes to be consistent on both sides of the radio. 
/*============================ TYPDEFS =======================================*/
/*! \brief  This struct defines the rx data container.
 *
 *  \see hal_frame_read
 *
 *  \ingroup hal
 */
typedef struct{
    uint8_t length;
    uint8_t data[ HAL_MAX_FRAME_LENGTH ];
    uint8_t lqi;
    bool2 crc;
} hal_rx_frame_t;

//! RX_START event handler callback type. Is called with timestamp in IEEE 802.15.4 symbols and frame length.
typedef void (*hal_rx_start_isr_event_handler_t)(uint32_t const isr_timestamp, uint8_t const frame_length);

//! RRX_END event handler callback type. Is called with timestamp in IEEE 802.15.4 symbols and frame length.
typedef void (*hal_trx_end_isr_event_handler_t)(uint32_t const isr_timestamp);
/*============================ PROTOTYPES ====================================*/
void hal_init( void );

void hal_reset_flags( void );
uint8_t hal_get_bat_low_flag( void );
void hal_clear_bat_low_flag( void );

uint8_t hal_get_trx_ur_flag( void );
void hal_clear_trx_ur_flag( void );

uint8_t hal_get_trx_end_flag( void );
void hal_clear_trx_end_flag( void );
hal_trx_end_isr_event_handler_t hal_get_trx_end_event_handler( void );
void hal_set_trx_end_event_handler( hal_trx_end_isr_event_handler_t trx_end_callback_handle );
void hal_clear_trx_end_event_handler( void );

uint8_t hal_get_rx_start_flag( void );
void hal_clear_rx_start_flag( void );
hal_rx_start_isr_event_handler_t hal_get_rx_start_event_handler( void );
void hal_set_rx_start_event_handler( hal_rx_start_isr_event_handler_t rx_start_callback_handle );
void hal_clear_rx_start_event_handler( void );

uint8_t hal_get_unknown_isr_flag( void );   
void hal_clear_unknown_isr_flag( void );

uint8_t hal_get_pll_unlock_flag( void );
void hal_clear_pll_unlock_flag( void );

uint8_t hal_get_pll_lock_flag( void );
void hal_clear_pll_lock_flag( void );

uint8_t hal_register_read( uint8_t address );
void hal_register_write( uint8_t address, uint8_t value );
uint8_t hal_subregister_read( uint8_t address, uint8_t mask, uint8_t position );
void hal_subregister_write( uint8_t address, uint8_t mask, uint8_t position, 
                            uint8_t value );
__z void hal_frame_read( hal_rx_frame_t *rx_frame );
__z void hal_frame_write( uint8_t *write_buffer, uint8_t length );
__z void hal_sram_read( uint8_t address, uint8_t length, uint8_t *data );
__z void hal_sram_write( uint8_t address, uint8_t length, uint8_t *data );
uint32_t hal_get_system_time( void );
void rf230radio_receive(void);
#endif
/*EOF*/
