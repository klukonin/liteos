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
 
#ifndef STDSERIALH
#define STDSERIALH
#include "../../types/types.h"

/**\ingroup serial
Print a string to the serial port. 
*/
void printfstr(char *str);

/**\ingroup serial
Print a string with a fixed  length through the serial port. 
*/
void printfstrlen(char *str, uint8_t len);

/**\ingroup serial
Print a 8-bit value to the serial port. 
*/
void printfval(uint8_t val);
void printfinteger32(int32_t val);
void printfuinteger32(uint32_t val);
void printfstrln();
#endif
