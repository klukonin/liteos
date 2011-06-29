/** @file libeeprom.c
       @brief The functional implementation of the eeprom API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/


#include "libsystem.h"
#include "liteoscommon.h"
#include "libstring.h"
#include "libthread.h"
#include "libeeprom.h"
#include "../types/types.h"

//Assume that the file system take 0-3199.

//The remaining 3200-4000 , 25 32-byte locations are provided to store hash data

//And the EEPROM library also allows user to access, read, and write to individual EEPROM locations






genericByteStorageHandle *getCurrentEEPROMInfo()
{
   genericByteStorageHandle *currenteeprominfoaddr;

   void (*getaddrfp)(void) = (void (*)(void))GET_EEPROM_STRUCTURE_HANDLE;

   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (currenteeprominfoaddr)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return currenteeprominfoaddr;
}




//Turn off the interrupt, access the location, and use system call to implement poll based imlementation provided by avr libc

void lib_read_from_eeprom(uint16_t addr, uint16_t nBytes, uint8_t *buffer)
{


	_atomic_t currentatomic;

	currentatomic = _atomic_start();


    genericByteStorageHandle *eeprominfoaddr;
    eeprominfoaddr = getCurrentEEPROMInfo();

	void (*getaddrfp)(void) = (void (*)(void))READ_EEPROM_TASK;

    eeprominfoaddr-> addr = addr;
   	eeprominfoaddr-> nBytes = nBytes;
   	eeprominfoaddr-> buffer = buffer;


    getaddrfp();

	_atomic_end(currentatomic);
}




//Turn off the interrupt, access the location, and use system call to implement poll based implementation provided by avr libc

void lib_writeToEEPROM(uint16_t addr, uint16_t nBytes, uint8_t *buffer)
{

	_atomic_t currentatomic;

	currentatomic = _atomic_start();

    genericByteStorageHandle *eeprominfoaddr;
    eeprominfoaddr = getCurrentEEPROMInfo();

	void (*getaddrfp)(void) = (void (*)(void))WRITE_EEPROM_TASK;

    eeprominfoaddr-> addr = addr;
	  eeprominfoaddr-> nBytes = nBytes;
	  eeprominfoaddr-> buffer = buffer;


    getaddrfp();

	_atomic_end(currentatomic);


}



