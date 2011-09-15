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
 * \brief This files defines the API for the Transceiver Access Toolbox.
 *
 *      This file contains the function prototypes for the Transceiver Access 
 *      Toolbox, hence it is an API. The Transceiver Access Toolbox is an
 *      abstraction layer that hides the details of the radio transceiver from 
 *      the end-user. The goal for the Transceiver Access Toolbox is to wrap the 
 *      services that the radio transceiver can perform into easy to use functions.
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
/*! \mainpage Transceiver Access Toolbox
 *
 * \section secIntroduction Introduction
 * The Transceiver Access Toolbox is implemented as an easy-to-use library that 
 * covers most of the functionality provided by the AT86RF230 radio transceiver. 
 * The library is completely written in the C programming language.
 *
 * \subsection secMisra MISRA Compliance
 * The Transceiver Access Toolbox is MISRA Compliant to the required rule set,
 * with the following exceptions:
 *    -# Rule 1: No compiler extensions allowed. Comment: Not possible to meet, 
 *               since we are on an embedded target where interrupts are needed.
 *    -# Rule 37: Bitwise operations is not allowed on signed integer types. 
 *                Comment: This is an error in the MISRA parser, since no singned 
 *                variable is used. Problem seams to be in stdint.h. 
 *    -# Rule 54: Null statement can only be on a line by itself. Comment: Triggered 
 *                by the AVR_ENTER_CRITICAL_REGION macro. This macro can be 
 *                written out so that the error goes away. The macro is only 
 *                used for convenience.
 *    -# Rule 71: No prototype seen. Comment: This error orginates from how 
 *                interrupt handlers are implemented in IAR.
 *    -# Rule 96: Function like macro shall be enclosed in parenthesis. Comment: 
 *                Triggered by the AVR_ENTER_CRITICAL_REGION macro. 
 *                This macro can be written out so that the error goes away. 
 *                The macro is only used for convenience and readability of the 
 *                code.
 *
 * \subsection secCompileOptions Compile Options
 *
 *    -# F_CPU: The clock speed of the CPU must be defined. The current 
 *              implementation supports 1, 4, 8 and 16 MHz operation. Example:
 *              # define F_CPU=8000000UL
 *    -# ARCHITECTURE: AVR is the only currently supported architecture. 
 *                     Example: # define AVR
 *
 *    See the examples directory for further information on how to set up the 
 *    IAR or avr-gcc toolchains to define these symbols.
 *
 * \defgroup tat Transceiver Access Toolbox API
 *      The Transceiver Access Toolbox API is a set of functions that provides the
 *      end user with total access to all functionality in the radio transceiver, 
 *      without dealing with its registers and internal dynamics.
 *
 */

#ifndef TAT_H
#define TAT_H
/*============================ INCLUDE =======================================*/
//#include <stdint.h>
//#include <stdbool.h>
#include "hal.h"
#include "compiler.h"
#include "../../io/radio/amcommon.h"
#include "../../types/types.h"

/*============================ MACROS ========================================*/
#define SUPPORTED_PART_NUMBER                   ( 2 )
#define RF230_REVA                              ( 1 )
#define RF230_REVB                              ( 2 )
#define SUPPORTED_MANUFACTURER_ID               ( 31 )
#define RF230_SUPPORTED_INTERRUPT_MASK          ( 0xCF )

#define RF230_MIN_CHANNEL                       ( 11 )
#define RF230_MAX_CHANNEL                       ( 26 )
#define RF230_MIN_ED_THRESHOLD                  ( 0 )
#define RF230_MAX_ED_THRESHOLD                  ( 15 )
#define RF230_MAX_TX_FRAME_LENGTH               ( 127 ) //!< 127 Byte PSDU.

#define TX_PWR_3DBM                             ( 0 )
#define TX_PWR_17_2DBM                          ( 15 )

#define BATTERY_MONITOR_HIGHEST_VOLTAGE         ( 15 )
#define BATTERY_MONITOR_VOLTAGE_UNDER_THRESHOLD ( 0 )
#define BATTERY_MONITOR_HIGH_VOLTAGE            ( 1 )
#define BATTERY_MONITOR_LOW_VOLTAGE             ( 0 )

#define FTN_CALIBRATION_DONE                    ( 0 )
#define PLL_DCU_CALIBRATION_DONE                ( 0 )
#define PLL_CF_CALIBRATION_DONE                 ( 0 )
/*============================ TYPEDEFS ======================================*/

/*! \brief  This macro defines the start value for the TAT_* status constants.
 *          
 *          It was chosen to have this macro so that the user can define where
 *          the status returned from the TAT starts. This can be useful in a
 *          system where numerous drivers are used, and some range of status codes
 *          are occupied.
 * 
 *  \see tat_status_t
 *  \ingroup tat
 */
#define TAT_STATUS_START_VALUE                  ( 0x40 )

/*! \brief  This enumeration defines the possible return values for the TAT API 
 *          functions.
 *          
 *          These values are defined so that they should not collide with the
 *          return/status codes defined in the IEEE 802.15.4 standard.
 *
 *  \ingroup tat
 */
typedef enum{
    //!< The requested service was performed successfully.
    TAT_SUCCESS = TAT_STATUS_START_VALUE,
    //!< The connected device is not an Atmel AT86RF230.
    TAT_UNSUPPORTED_DEVICE,
    //!< One or more of the supplied function arguments are invalid.
    TAT_INVALID_ARGUMENT,
    //!< The requested service timed out.
    TAT_TIMED_OUT,
    //!< The end-user tried to do an invalid state transition.
    TAT_WRONG_STATE,
    //!< The radio transceiver is busy receiving or transmitting.
    TAT_BUSY_STATE,
    //!< The requested state transition could not be completed.
    TAT_STATE_TRANSITION_FAILED,
    //!< Channel in idle. Ready to transmit a new frame.
    TAT_CCA_IDLE,
    //!< Channel busy.
    TAT_CCA_BUSY,
    //!< Transceiver is busy receiving or transmitting data.
    TAT_TRX_BUSY,
    //!< Measured battery voltage is lower than voltage threshold.
    TAT_BAT_LOW,
    //!< Measured battery voltage is above the voltage threshold.
    TAT_BAT_OK,
    //!< The CRC failed for the actual frame.
    TAT_CRC_FAILED,
    //!< The channel access failed during the auto mode.
    TAT_CHANNEL_ACCESS_FAILURE,
    //!< No acknowledge frame was received.
    TAT_NO_ACK,
}tat_status_t;

/*! \brief  This enumeration defines the possible modes available for the 
 *          Clear Channel Assessment algorithm.
 *
 *          These constants are extracted from the datasheet.
 *
 *  \ingroup tat
 */
typedef enum{
    //!< Use energy detection above threshold mode.
    CCA_ED                    = 0,
    //!< Use carrier sense mode.
    CCA_CARRIER_SENSE         = 1,
    //!< Use a combination of both energy detection and carrier sense.
    CCA_CARRIER_SENSE_WITH_ED = 2
}tat_cca_mode_t;

/*! \brief  This enumeration defines the possible CLKM speeds.
 *
 *          These constants are extracted from the datasheet.
 *
 *  \ingroup tat
 */
typedef enum{

    CLKM_DISABLED      = 0,
    CLKM_1MHZ          = 1,
    CLKM_2MHZ          = 2,
    CLKM_4MHZ          = 3,
    CLKM_8MHZ          = 4,
    CLKM_16MHZ         = 5    
}tat_clkm_speed_t;
/*============================ PROTOTYPES ====================================*/
tat_status_t tat_init( void );
uint8_t tat_get_operating_channel( void );
tat_status_t tat_set_operating_channel( uint8_t channel );
uint8_t tat_get_tx_power_level( void );
tat_status_t tat_set_tx_power_level( uint8_t power_level );

tat_status_t tat_do_ed_scan( uint8_t *ed_level );
uint8_t tat_get_cca_mode( void );
uint8_t tat_get_ed_threshold( void );
tat_status_t tat_set_cca_mode( uint8_t mode, uint8_t ed_threshold );
tat_status_t tat_do_cca( void );
tat_status_t  tat_get_rssi_value( uint8_t *rssi );

uint8_t tat_batmon_get_voltage_threshold( void );
uint8_t tat_batmon_get_voltage_range( void );
tat_status_t tat_batmon_configure( bool2 range, uint8_t voltage_threshold );
tat_status_t tat_batmon_get_status( void );

uint8_t tat_get_clock_speed( void );
tat_status_t tat_set_clock_speed( bool2 direct, uint8_t clock_speed );
tat_status_t tat_calibrate_filter( void );
tat_status_t tat_calibrate_pll( void );

uint8_t tat_get_trx_state( void );
tat_status_t tat_set_trx_state( uint8_t new_state );
tat_status_t tat_enter_sleep_mode( void );
tat_status_t tat_leave_sleep_mode( void );
void tat_reset_state_machine( void );
void tat_reset_trx( void );

void tat_use_auto_tx_crc( bool2 auto_crc_on );
__x tat_status_t tat_send_data( uint8_t data_length, uint8_t *data );
//__x Radio_MsgPtr tat_receive_data( hal_rx_frame_t *rx_frame ); //Added Receive Function

uint8_t tat_get_device_role( void );
void tat_set_device_role( bool2 i_am_coordinator );
uint16_t tat_get_pan_id( void );
void tat_set_pan_id( uint16_t new_pan_id );
uint16_t tat_get_short_address( void );
void tat_set_short_address( uint16_t new_short_address );
__x void tat_get_extended_address( uint8_t *extended_address );
__x void tat_set_extended_address( uint8_t *extended_address );
tat_status_t tat_configure_csma( uint8_t seed0, uint8_t be_csma_seed1 );
__x tat_status_t tat_send_data_with_retry( uint8_t frame_length, uint8_t *frame, 
                                       uint8_t retries );
#endif
/*EOF*/
