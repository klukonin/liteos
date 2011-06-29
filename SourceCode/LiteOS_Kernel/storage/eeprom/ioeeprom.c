/** @file ioeeprom.c
       @brief The functional implementation of the eeprom byte operations. 

	This file implements the eeprom layer for byte wise operations. 
	@author Qing Charles Cao (cao@utk.edu)
*/


#include "ioeeprom.h"
#include "../../hardware/avrhardware.h"
#include <avr/eeprom.h>


//-------------------------------------------------------------------------
void eeprom_readBytes(uint16_t addr, int nBytes, void *buffer)
{
    eeprom_busy_wait();
    eeprom_read_block(buffer, (void *)addr, nBytes);
    return;
}

//-------------------------------------------------------------------------
void eeprom_writeBytes(uint16_t addr, int nBytes, void *buffer)
{
    eeprom_busy_wait();
    eeprom_write_block(buffer, (void *)addr, nBytes);
    return;
}

//-------------------------------------------------------------------------
uint8_t eeprom_read8uint(uint16_t addr)
{
    uint8_t ret;
    _atomic_t _atomic = _atomic_start();

    eeprom_busy_wait();
    ret = eeprom_read_byte((uint8_t *) addr);
    _atomic_end(_atomic);
    return ret;
}

//-------------------------------------------------------------------------
int8_t eeprom_read8int(uint16_t addr)
{
    uint8_t value;
    _atomic_t _atomic = _atomic_start();

    eeprom_busy_wait();
    value = eeprom_read_byte((uint8_t *) addr);
    _atomic_end(_atomic);
    return *(int8_t *) & value;
}

//-------------------------------------------------------------------------
uint16_t eeprom_read16uint(uint16_t addr)
{
    uint16_t value;
    _atomic_t _atomic = _atomic_start();

    eeprom_busy_wait();
    value = eeprom_read_word((uint16_t *) addr);
    _atomic_end(_atomic);
    return value;
}

//-------------------------------------------------------------------------
int16_t eeprom_read16int(uint16_t addr)
{
    uint16_t value;
    _atomic_t _atomic = _atomic_start();

    eeprom_busy_wait();
    value = eeprom_read_word((uint16_t *) addr);
    _atomic_end(_atomic);
    return *(int16_t *) & value;
}

//-------------------------------------------------------------------------
void eeprom_write8uint(uint16_t addr, uint8_t value)
{
    _atomic_t _atomic = _atomic_start();

    eeprom_busy_wait();
    eeprom_write_byte((uint8_t *) addr, value);
    _atomic_end(_atomic);
    return;
}

//-------------------------------------------------------------------------
void eeprom_write8int(uint16_t addr, int8_t value)
{
    uint8_t value2;
    _atomic_t _atomic = _atomic_start();

    eeprom_busy_wait();
    value2 = *(uint8_t *) & value;
    eeprom_write_byte((uint8_t *) addr, value2);
    _atomic_end(_atomic);
    return;
}

//-------------------------------------------------------------------------
void eeprom_write16uint(uint16_t addr, uint16_t value)
{
    _atomic_t _atomic = _atomic_start();

    eeprom_busy_wait();
    eeprom_write_word((uint16_t *) addr, value);
    _atomic_end(_atomic);
    return;
}

//-------------------------------------------------------------------------
void eeprom_write16int(uint16_t addr, int16_t value)
{
    uint16_t value2;
    _atomic_t _atomic = _atomic_start();

    eeprom_busy_wait();
    value2 = *(uint16_t *) & value;
    eeprom_write_word((uint16_t *) addr, value2);
    _atomic_end(_atomic);
    return;
}

//-------------------------------------------------------------------------
uint32_t eeprom_read32uint(uint16_t addr)
{
    uint16_t value1;
    uint16_t value2;
    _atomic_t _atomic = _atomic_start();

    eeprom_busy_wait();
    value1 = eeprom_read_word((uint16_t *) addr);
    eeprom_busy_wait();
    value2 = eeprom_read_word((uint16_t *) (addr + 2));
    _atomic_end(_atomic);
    return (((uint32_t) value2) << 16) + (uint32_t) value1;
}

//-------------------------------------------------------------------------
int32_t eeprom_read32int(uint16_t addr)
{
    uint16_t value1;
    uint16_t value2;
    uint32_t returnvalue;
    _atomic_t _atomic = _atomic_start();

    eeprom_busy_wait();
    value1 = eeprom_read_word((uint16_t *) addr);
    eeprom_busy_wait();
    value2 = eeprom_read_word((uint16_t *) (addr + 2));
    returnvalue = ((((uint32_t) value2) << 16) + (uint32_t) value1);
    _atomic_end(_atomic);
    return *(int32_t *) & returnvalue;
}

//-------------------------------------------------------------------------
void eeprom_write32uint(uint16_t addr, uint32_t value)
{
    uint16_t value1;
    uint16_t value2;
    _atomic_t _atomic = _atomic_start();

    value1 = (uint16_t) (value >> 16);
    value2 = (uint16_t) (value & 0x0000ffff);
    eeprom_busy_wait();
    //value2 = *(uint16_t *)&value;
    eeprom_write_word((uint16_t *) addr, value2);
    addr = addr + 2;
    eeprom_busy_wait();
    //value2 = *(uint16_t *)&value;
    eeprom_write_word((uint16_t *) addr, value1);
    _atomic_end(_atomic);
    return;
}

//-------------------------------------------------------------------------
void eeprom_write32int(uint16_t addr, int32_t value)
{
    uint16_t value1;
    uint32_t value2;
    _atomic_t _atomic = _atomic_start();

    value1 = (uint16_t) ((*(uint32_t *) & value) >> 16);
    value2 = (uint16_t) ((*(uint32_t *) & value) & 0x0000ffff);
    eeprom_busy_wait();
    //value2 = *(uint16_t *)&value;
    eeprom_write_word((uint16_t *) addr, value2);
    addr = addr + 2;
    eeprom_busy_wait();
    //value2 = *(uint16_t *)&value;
    eeprom_write_word((uint16_t *) addr, value1);
    _atomic_end(_atomic);
    return;
}

//-------------------------------------------------------------------------
void eeprom_initBytes(uint16_t addr, int nBytes, uint8_t value)
{
    int i;
    _atomic_t _atomic = _atomic_start();

    for (i = 0; i < nBytes; i++)
    {
        eeprom_write8uint(addr + i, value);
    }
    _atomic_end(_atomic);
}
