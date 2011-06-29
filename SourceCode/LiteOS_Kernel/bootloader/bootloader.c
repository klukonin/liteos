/** @file bootloader.c
	@brief The implementation of the bootloader functionality, including self-programming and debugging/tracing. 

	@author Qing Charles Cao (cao@utk.edu)
*/

#include "bootloader.h"
#include "../types/types.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <avr/io.h>

 

 
static uint16_t pageindex;
static uint8_t pagebuffer[BOOTLOADERSIZE];
static uint16_t currentpageindex;

//only six bytes are used here. 
//translate into ldi, ldi, icall

static uint8_t breakpointstring[8] = {
    0xE8, 0xE9, 0xFA, 0xEE, 0x09, 0x95, 0xFC, 0xCF
};

static uint8_t tracepointstring[16] = {
    0xEF, 0x93, 0xFF, 0x93, 0xE4, 0xEF, 0xFA, 0xEE, 0x09, 0x95, 0xFF, 0x91,
        0xEF,
    0x91, 0xFF, 0xFF
};

void __attribute__ ((section(".bootloader"))) ProgramSetPage(uint16_t pageno)
{
    int i;

    for (i = 0; i < 512; i++)
    {
        pagebuffer[i] = 0;
    }
    pageindex = pageno;
    currentpageindex = 0;
}

//-------------------------------------------------------------------------
void __attribute__ ((section(".bootloader"))) ProgramString(char *buffer)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        pagebuffer[currentpageindex + i] = buffer[i];
    }
    currentpageindex += 16;
    if (currentpageindex == 512)
    {
        reprogramall(pagebuffer);
        currentpageindex = 0;
    }
}

//-------------------------------------------------------------------------
void __attribute__ ((section(".bootloader"))) ProgramRemains()
{
    if (currentpageindex != 0)
    {
        reprogramall(pagebuffer);
    }
}

//-------------------------------------------------------------------------
void __attribute__ ((section(".bootloader"))) ProgramLastString(char *buffer,
                                                                uint8_t length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        pagebuffer[currentpageindex + i] = buffer[i];
    }
    reprogramall(pagebuffer);
    currentpageindex = 0;
}

//-------------------------------------------------------------------------
int __attribute__ ((section(".bootloader"))) reprogramall(unsigned char
                                                          *program)
{
    //unsigned char buffer[256];
    unsigned char a, b;
    int j;
    int high, low, all;

    for (j = 0; j < 256; j++)
    {
        a = program[j * 2];
        b = program[j * 2 + 1];
        if ((a >= '0') && (a <= '9'))
        {
            high = a - '0';
        }
        else
        {
            high = a - 'A' + 10;
        }
        if ((b >= '0') && (b <= '9'))
        {
            low = b - '0';
        }
        else
        {
            low = b - 'A' + 10;
        }

        all = high * 16 + low;
        //This might introduce bugs.
        program[j] = (unsigned char)all;
    }
    reprogram(program, pageindex);
    pageindex++;
    return 0;
}

//-------------------------------------------------------------------------
int __attribute__ ((section(".bootloader"))) reprogram(unsigned char *buffer,
                                                       uint16_t codepage)
{
    boot_program_page((uint32_t) SPM_PAGESIZE * ((uint32_t) codepage), buffer);
    return 0;
}

//insert a breakpoint by removing 8 bytes at the location specified by the pagenum and offset 
void __attribute__ ((section(".bootloader"))) boot_insertBreakPoint(uint16_t
                                                                    pagenum,
                                                                    uint8_t
                                                                    pageoffset,
                                                                    uint8_t *
                                                                    instructions)
{
    //first read the page, then change the string, then write back the page
    int i;
    uint32_t addr;
    uint8_t index;
    uint16_t data;
    addr = (uint32_t) pagenum *(uint32_t) SPM_PAGESIZE;

    for (i = 0; i < 128; i++)
    {
        data = pgm_read_word_far(addr + ((uint32_t) i) * 2);
        if (is_host_lsb())
        {
            pagebuffer[i * 2] = data % 256;
            pagebuffer[i * 2 + 1] = data / 256;
        }
        else
        {
            pagebuffer[i * 2 + 1] = data % 256;
            pagebuffer[i * 2] = data / 256;
        }
    }
    index = pageoffset;
    for (i = 0; i < 6; i++)
    {
        instructions[i] = pagebuffer[index + i];
        pagebuffer[index + i] = breakpointstring[i];
    }
    reprogram(pagebuffer, pagenum);
}

//insert a breakpoint by removing 8 bytes at the location specified by the pagenum and offset 
//insert a breakpoint by removing 8 bytes at the location specified by the pagenum and offset 
void __attribute__ ((section(".bootloader"))) boot_insertTracePoint(uint16_t
                                                                    pagenum,
                                                                    uint8_t
                                                                    pageoffset)
{
    //first read the page, then change the string, then write back the page
    int i;
    uint32_t addr;
    uint8_t index;
    uint16_t data;
    uint8_t programmore = 0;
    uint8_t endindex, endindex2;

    //Read the page content
    addr = (uint32_t) pagenum *(uint32_t) SPM_PAGESIZE;

    //read the page content 
    for (i = 0; i < 128; i++)
    {
        data = pgm_read_word_far(addr + ((uint32_t) i) * 2);
        if (is_host_lsb())
        {
            pagebuffer[i * 2] = data % 256;
            pagebuffer[i * 2 + 1] = data / 256;
        }
        else
        {
            pagebuffer[i * 2 + 1] = data % 256;
            pagebuffer[i * 2] = data / 256;
        }
    }
    //assuming that the index as 230 or smaller, then enough for the 26 bytes othersise not 
    //say 234, then it is 22 bytes 
    //Now this part needs to be changed to 16 bytes
    //index is 242, then 14 bytes. 
    index = pageoffset;
    programmore = 0;
    if (pageoffset > 242)
    {
        programmore = 1;
        endindex = (uint8_t) ((uint16_t) 256 - (uint16_t) pageoffset);
    }
    else
    {
        programmore = 0;
        endindex = 14;
    }
    //for the 234 example, 22 here, then 0, 21 are copied 
    for (i = 0; i < endindex; i++)
    {
        //   instructions[ i ] = pagebuffer[ index + i ];
        pagebuffer[index + i] = tracepointstring[i];
    }
    reprogram(pagebuffer, pagenum);
    if (programmore == 0)
    {
        return;
    }
    //next page
    addr = (uint32_t) (pagenum + 1) * (uint32_t) SPM_PAGESIZE;
    for (i = 0; i < 128; i++)
    {
        data = pgm_read_word_far(addr + ((uint32_t) i) * 2);
        if (is_host_lsb())
        {
            pagebuffer[i * 2] = data % 256;
            pagebuffer[i * 2 + 1] = data / 256;
        }
        else
        {
            pagebuffer[i * 2 + 1] = data % 256;
            pagebuffer[i * 2] = data / 256;
        }
    }
    endindex2 = 14 - endindex;
    programmore = 0;
    for (i = 0; i < endindex2; i++)
    {
        //   instructions[ i ] = pagebuffer[ index + i ];
        pagebuffer[i] = tracepointstring[endindex + i];
    }
    reprogram(pagebuffer, pagenum + 1);
}


//insert a breakpoint by removing 8 bytes at the location specified by the pagenum and offset 
void __attribute__ ((section(".bootloader")))
boot_insertTracePointLong(uint16_t pagenum, uint8_t pageoffset)
{
    //first read the page, then change the string, then write back the page
    int i;
    uint32_t addr;
    uint8_t index;
    uint16_t data;
    uint8_t programmore = 0;
    uint8_t endindex, endindex2;

    //Read the page content
    addr = (uint32_t) pagenum *(uint32_t) SPM_PAGESIZE;

    //read the page content 
    for (i = 0; i < 128; i++)
    {
        data = pgm_read_word_far(addr + ((uint32_t) i) * 2);
        if (is_host_lsb())
        {
            pagebuffer[i * 2] = data % 256;
            pagebuffer[i * 2 + 1] = data / 256;
        }
        else
        {
            pagebuffer[i * 2 + 1] = data % 256;
            pagebuffer[i * 2] = data / 256;
        }
    }
    //assuming that the index as 228 or smaller, then enough for the 28 bytes othersise not 
    //say 234, then it is 22 bytes 
    index = pageoffset;
    programmore = 0;
    if (pageoffset > 240)
    {
        programmore = 1;
        endindex = (uint8_t) ((uint16_t) 256 - (uint16_t) pageoffset);
    }
    else
    {
        programmore = 0;
        endindex = 16;
    }
    //for the 234 example, 22 here, then 0, 21 are copied 
    for (i = 0; i < endindex; i++)
    {
        //   instructions[ i ] = pagebuffer[ index + i ];
        pagebuffer[index + i] = tracepointstring[i];
    }
    reprogram(pagebuffer, pagenum);
    if (programmore == 0)
    {
        return;
    }
    //next page
    addr = (uint32_t) (pagenum + 1) * (uint32_t) SPM_PAGESIZE;
    for (i = 0; i < 128; i++)
    {
        data = pgm_read_word_far(addr + ((uint32_t) i) * 2);
        if (is_host_lsb())
        {
            pagebuffer[i * 2] = data % 256;
            pagebuffer[i * 2 + 1] = data / 256;
        }
        else
        {
            pagebuffer[i * 2 + 1] = data % 256;
            pagebuffer[i * 2] = data / 256;
        }
    }
    //PREVIOUSLY like 22 then the value is 6
    endindex2 = 16 - endindex;
    programmore = 0;
    for (i = 0; i < endindex2; i++)
    {
        //   instructions[ i ] = pagebuffer[ index + i ];
        pagebuffer[i] = tracepointstring[endindex + i];
    }
    reprogram(pagebuffer, pagenum + 1);
}



//-------------------------------------------------------------------------
//remove a breakpoint by patching the 8 bytes at the location specified at the pagenum and offset
void __attribute__ ((section(".bootloader"))) boot_removeBreakPoint(uint16_t
                                                                    pagenum,
                                                                    uint8_t
                                                                    pageoffset,
                                                                    uint8_t *
                                                                    instructions)
{
    //first read the page, then change the string, then write back the page. 
    //first read the page, then change the string, then write back the page
    int i;
    uint32_t addr;
    uint8_t index;
    uint16_t data;
    addr = (uint32_t) pagenum *(uint32_t) SPM_PAGESIZE;

    for (i = 0; i < 128; i++)
    {
        data = pgm_read_word_far(addr + ((uint32_t) i) * 2);
        if (is_host_lsb())
        {
            pagebuffer[i * 2] = data % 256;
            pagebuffer[i * 2 + 1] = data / 256;
        }
        else
        {
            pagebuffer[i * 2 + 1] = data % 256;
            pagebuffer[i * 2] = data / 256;
        }
    }
    index = pageoffset;
    for (i = 0; i < 6; i++)
    {
        pagebuffer[index + i] = instructions[i];
    }
    reprogram(pagebuffer, pagenum);
}

//-------------------------------------------------------------------------
void __attribute__ ((section(".bootloader"))) boot_program_page(uint32_t page,
                                                                uint8_t * buf)
{
    uint16_t i;
    uint8_t sreg;

    // Disable interrupts.
    sreg = SREG;
    cli();
    eeprom_busy_wait();
    boot_page_erase(page);
    boot_spm_busy_wait();       // Wait until the memory is erased.
    for (i = 0; i < SPM_PAGESIZE; i += 2)
    {
        // Set up little-endian word.
        uint16_t w = *buf++;

        w += (*buf++) << 8;
        boot_page_fill(page + i, w);
    }
    boot_spm_busy_wait();
    boot_page_write(page);      // Store buffer in flash page.
    boot_spm_busy_wait();       // Wait until the memory is written.
    // Reenable RWW-section again. We need this if we want to jump back
    // to the application after bootloading.
    boot_rww_enable();
    // Re-enable interrupts (if they were ever enabled).
    SREG = sreg;
}

//-------------------------------------------------------------------------------
//This function allows remove tracepoint by repatching it 
void __attribute__ ((section(".bootloader"))) boot_removeTracePoint(uint16_t
                                                                    pagenum,
                                                                    uint8_t
                                                                    pageoffset,
                                                                    uint8_t *
                                                                    buffer)
{
    //first read the page, then change the string, then write back the page
    int i;
    uint32_t addr;
    uint8_t index;
    uint16_t data;
    uint8_t programmore = 0;
    uint8_t endindex, endindex2;

    //Read the page content
    addr = (uint32_t) pagenum *(uint32_t) SPM_PAGESIZE;

    //read the page content 
    for (i = 0; i < 128; i++)
    {
        data = pgm_read_word_far(addr + ((uint32_t) i) * 2);
        if (is_host_lsb())
        {
            pagebuffer[i * 2] = data % 256;
            pagebuffer[i * 2 + 1] = data / 256;
        }
        else
        {
            pagebuffer[i * 2 + 1] = data % 256;
            pagebuffer[i * 2] = data / 256;
        }
    }
    //assuming that the index as 230 or smaller, then enough for the 26 bytes othersise not 
    //say 234, then it is 22 bytes 
    //Now this part needs to be changed to 16 bytes
    //index is 242, then 14 bytes. 
    index = pageoffset;
    programmore = 0;
    if (pageoffset > 242)
    {
        programmore = 1;
        endindex = (uint8_t) ((uint16_t) 256 - (uint16_t) pageoffset);
    }
    else
    {
        programmore = 0;
        endindex = 14;
    }
    //for the 234 example, 22 here, then 0, 21 are copied 
    for (i = 0; i < endindex; i++)
    {
        //   instructions[ i ] = pagebuffer[ index + i ];
        pagebuffer[index + i] = buffer[i];
    }
    reprogram(pagebuffer, pagenum);
    if (programmore == 0)
    {
        return;
    }
    //next page
    addr = (uint32_t) (pagenum + 1) * (uint32_t) SPM_PAGESIZE;
    for (i = 0; i < 128; i++)
    {
        data = pgm_read_word_far(addr + ((uint32_t) i) * 2);
        if (is_host_lsb())
        {
            pagebuffer[i * 2] = data % 256;
            pagebuffer[i * 2 + 1] = data / 256;
        }
        else
        {
            pagebuffer[i * 2 + 1] = data % 256;
            pagebuffer[i * 2] = data / 256;
        }
    }
    endindex2 = 14 - endindex;
    programmore = 0;
    for (i = 0; i < endindex2; i++)
    {
        //   instructions[ i ] = pagebuffer[ index + i ];
        pagebuffer[i] = buffer[endindex + i];
    }
    reprogram(pagebuffer, pagenum);
}

//insert a breakpoint by removing 8 bytes at the location specified by the pagenum and offset 
void __attribute__ ((section(".bootloader")))
boot_removeTracePointLong(uint16_t pagenum, uint8_t pageoffset,
                          uint8_t * buffer)
{
    //first read the page, then change the string, then write back the page
    int i;
    uint32_t addr;
    uint8_t index;
    uint16_t data;
    uint8_t programmore = 0;
    uint8_t endindex, endindex2;

    //Read the page content
    addr = (uint32_t) pagenum *(uint32_t) SPM_PAGESIZE;

    //read the page content 
    for (i = 0; i < 128; i++)
    {
        data = pgm_read_word_far(addr + ((uint32_t) i) * 2);
        if (is_host_lsb())
        {
            pagebuffer[i * 2] = data % 256;
            pagebuffer[i * 2 + 1] = data / 256;
        }
        else
        {
            pagebuffer[i * 2 + 1] = data % 256;
            pagebuffer[i * 2] = data / 256;
        }
    }
    //assuming that the index as 228 or smaller, then enough for the 28 bytes othersise not 
    //say 234, then it is 22 bytes 
    index = pageoffset;
    programmore = 0;
    if (pageoffset > 240)
    {
        programmore = 1;
        endindex = (uint8_t) ((uint16_t) 256 - (uint16_t) pageoffset);
    }
    else
    {
        programmore = 0;
        endindex = 16;
    }
    //for the 234 example, 22 here, then 0, 21 are copied 
    for (i = 0; i < endindex; i++)
    {
        //   instructions[ i ] = pagebuffer[ index + i ];
        pagebuffer[index + i] = buffer[i];
    }
    reprogram(pagebuffer, pagenum);
    if (programmore == 0)
    {
        return;
    }
    //next page
    addr = (uint32_t) (pagenum + 1) * (uint32_t) SPM_PAGESIZE;
    for (i = 0; i < 128; i++)
    {
        data = pgm_read_word_far(addr + ((uint32_t) i) * 2);
        if (is_host_lsb())
        {
            pagebuffer[i * 2] = data % 256;
            pagebuffer[i * 2 + 1] = data / 256;
        }
        else
        {
            pagebuffer[i * 2 + 1] = data % 256;
            pagebuffer[i * 2] = data / 256;
        }
    }
    //PREVIOUSLY like 22 then the value is 6
    endindex2 = 16 - endindex;
    programmore = 0;
    for (i = 0; i < endindex2; i++)
    {
        //   instructions[ i ] = pagebuffer[ index + i ];
        pagebuffer[i] = buffer[endindex + i];
    }
    reprogram(pagebuffer, pagenum + 1);
}
