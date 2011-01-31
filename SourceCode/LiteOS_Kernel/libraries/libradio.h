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


#ifndef LIBRADIOH
#define LIBRADIOH

#include "../types/types.h"
#include "libradio.h"
#include "libsystem.h"
#include "liteoscommon.h"
#include "libstring.h"
#include "libthread.h"


/** \defgroup radio Radio Operations API

      This module implements sending and receiving through radio.
*/

/** \ingroup radio

      Get the internal radio handle address.
      \param NONE
      \param address

 */

radiohandletype *lib_get_current_radio_receive_handle_addr();



/** \ingroup radio

      Send a packet over the radio.
      \param port The port number of the radio sending operation
      \param address The next hop address
      \param length The length of the message to be sent out
      \param msg The start address of the message to be sent out
      \return NONE

 */

void lib_radio_send_msg(uint16_t port, uint16_t address, uint8_t length, uint8_t *msg);


/** \ingroup radio

      Send a packet over the radio by broadcast
      \param msg The message to be sent out.
      \return NONE

 */

void lib_radio_send_string(uint8_t *msg);


/** \ingroup radio

      Send an integer over the radio by broadcast
      \param value The integer to be sent out
      \return NONE
*/
void lib_radio_send_integer_u16(uint16_t value);



/** \ingroup radio

       Receive a message through the radio.
       \param port The port number listening to.
       \param maxlength The maximal number of bytes provided
       \param msg The address to store the incoming message
       \return int The number of bytes received through radio
*/
int lib_radio_receive(uint16_t port, uint8_t maxlength, uint8_t *msg);

/** \ingroup radio

       Receive a message through the radio. Wait only for limited amount of time.
       \param port The port number listening to.
       \param maxlength The maximal number of bytes provided
       \param msg The address to store the incoming message
       \param time Maximum waiting time in milliseconds
       \return int The number of bytes received through radio
*/
int lib_radio_receive_timed(uint16_t port, uint8_t maxlength, uint8_t *msg, uint16_t time);

/** \ingroup radio

      Set the radio frequency directly using a number
      \param freq The frequency of the radio intended to work with
      \return NONE
*/
void lib_radio_set_freq(uint16_t freq);


/** \ingroup radio

      Set the channel of the radio, 11 to 26
      \param channel The channel of the radio that is intended to work in.
      \return NONE
*/

void lib_radio_set_channel(uint8_t channel);


/** \ingroup radio
      Set the power level of the radio.
      \param power The power level of the radio intended to work in.
      \return NONE
*/

void lib_radio_set_power(uint8_t power);



radioinfotype *lib_get_current_radio_send_addr(); 



void lib_radio_send_msg_syscall();


 
#endif
