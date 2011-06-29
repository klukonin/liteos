/** @file libeeprom.h
       @brief The functional prototypes for the eeprom API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/

#ifndef LIBEEPROMH
#define LIBEEPROMH
#include "liteoscommon.h"

/** @addtogroup api 
*/


/** @name This module implements reading/writing EEPROM of the motes.  Testing code is as follows. 

      @code 
      #include "leds.h"
			#include "libthread.h"
			#include "adc.h"
			#include "radio.h"
			#include "Serial.h"

			uint8_t receivedata[20]; 
			uint8_t temp;
			uint16_t i; 
			uint16_t j; 
			int main()
			{
			 __asm__ __volatile__("sei" ::); 
		  for (i=0;i<20;i++)
  		 receivedata[i] = i*6; 

		  for (i=3200;i<4000;i+=20)
	    lib_write_to_eeprom(i, 20, receivedata);

	  	for (i=3200;i<4000;i+=20)
	  	{
	    for (j=0;j<20;j++)
   		 receivedata[j] = 0; 

			 lib_read_from_eeprom(i, 20, receivedata);

			 serialSend(20, receivedata); 
	    }
  
    	return 0; 
	   }

       @endcode	
*/


/** @{ */



/** @brief Read part of EEPROM into local buffer
       @param addr The start address of the EEPROM
       @param nBytes The number of bytes read from EEPROM
       @param buffer Pointer to the internal buffer to keep EEPROM data
       @return NONE
       @note The first 3200 bytes of EEPROM is occupied by the LiteFS file system
*/
void lib_read_from_eeprom(uint16_t addr, uint16_t nBytes, uint8_t *buffer);


/** @brief Write part of EEPROM using local buffer
       @param addr The start address of the EEPROM
       @param nBytes The number of bytes written to EEPROM
       @param buffer Pointer to the internal buffer to retrive data to be written
       @return NONE
       @note The first 3200 bytes of EEPROM is occupied by the LiteFS file system
*/ 

void lib_write_to_eeprom(uint16_t addr, uint16_t nBytes, uint8_t *buffer);

/** @} */

#endif
