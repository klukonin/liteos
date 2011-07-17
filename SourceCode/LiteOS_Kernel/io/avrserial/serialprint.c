/**  @file serialprint.c
        @brief The implementation of the avr serial printing module. 

        This contains the implementations of any functions in the serial printing module. 

        @author Qing Charles Cao (cao@utk.edu)
        
        
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#define xtal 8000000L
#define baud 57600
#include "serialprint.h"
#include "../../kernel/threadkernel.h"
#include "../../hardware/avrhardware.h"
#include "../../entry/realmain.h"
#include "../../sensors/leds.h"

extern volatile uint16_t *stackinterrupt_ptr;  
extern volatile uint16_t *old_stack_ptr;  
static char serial_print_cFlag;
static uint8_t receivebuffer[33];
 
 
static uint8_t status;
 

//No need to be called from main 
//This function is called when the system is being initilized and prints system start 
void initUSART()
{
    UBRR0H = 0;
    UBRR0L = 15;
    UCSR0A |= _BV(U2X0);        // double speed
    UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);        // 8 data-bits, 1 stop-bit
    UCSR0B |= _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
    // Enable recieve/transmit/interrupts
    serial_print_cFlag = 1;
 
    status = 0;
}

//-------------------------------------------------------------------------
void usartPrint(uint8_t c)
{
    if (serial_print_cFlag == 0)
    {
        initUSART();
    }
    while ((UCSR0A & (1 << UDRE0)) == 0)
        ;
    UDR0 = c;
}

void usartPrint_base(uint8_t c)
{
    if (serial_print_cFlag == 0)
    {
        initUSART();
    }
    while ((UCSR0A & (1 << UDRE0)) == 0)
        ;
    UDR0 = c;
}



//-------------------------------------------------------------------------
void printString(char *p)
{
    usartPrint(0xFC);
    while ((*p) != '\0')
    {
        usartPrint(*p);
        p++;
    }
    usartPrint(0xFC);
}

//-------------------------------------------------------------------------
void printStringN(char *p, uint8_t n)
{
    uint8_t i;

    usartPrint(0xFC);
    for (i = 0; i < n; i++)
    {
        usartPrint(*p);
        p++;
    }
    usartPrint(0xFC);
}


//-------------------------------------------------------------------------
void printStringN_base(char *p, uint8_t n)
{
    uint8_t i;

    //usartPrint(0xFC);
    for (i = 0; i < n; i++)
    {
        usartPrint_base(*p);
        p++;
    }
    //usartPrint(0xFC);
}




//-------------------------------------------------------------------------
void usartPutChipHex(uint8_t cChip)
{
    if (cChip > 9)
    {
        usartPrint('a' + cChip - 10);
    }
    else
    {
        usartPrint('0' + cChip);
    }
}

//-------------------------------------------------------------------------
void usartPutLong2(uint32_t l)
{
    uint8_t *pcByte = ((uint8_t *) (&l)) + 3;

    usartPrint(*pcByte);
    pcByte--;
    usartPrint(*pcByte);
    pcByte--;
    usartPrint(*pcByte);
    pcByte--;
    usartPrint(*pcByte);
}

//-------------------------------------------------------------------------
void usartPutHex(uint8_t c)
{
    usartPutChipHex(c >> 4);
    usartPutChipHex(c & 0xf);
}

//-------------------------------------------------------------------------
void printInteger32(int32_t a)
{
    usartPrint(0xFD);
    usartPutLong2(*(uint32_t *) (&a));
    usartPrint(0xFD);
}

//-------------------------------------------------------------------------
void printIntegerU32(uint32_t l)
{
    usartPrint(0xFE);
    usartPutLong2(l);
    usartPrint(0xFE);
}


void printStringLn()
{
	  usartPrint(0xFC);
    usartPrint(0x6D);
    usartPrint(0x6D);
    usartPrint(0x6D);
    usartPrint(0xFC);
    
}
//-------------------------------------------------------------------------
void usartPutLong(uint32_t l)
{
    uint8_t *pcByte = ((uint8_t *) (&l)) + 3;

    usartPrint('0');
    usartPrint('x');
    usartPutHex(*pcByte);
    pcByte--;
    usartPutHex(*pcByte);
    pcByte--;
    usartPutHex(*pcByte);
    pcByte--;
    usartPutHex(*pcByte);
}

//-------------------------------------------------------------------------
void printInteger(int32_t a, int32_t b, int32_t c, int32_t d)
{
    usartPrint(1);
    usartPutLong(*(uint32_t *) (&a));
    usartPutLong(*(uint32_t *) (&b));
    usartPutLong(*(uint32_t *) (&c));
    usartPutLong(*(uint32_t *) (&d));
}


SIGNAL(USART0_RX_vect)
{
    uint8_t dummy = UDR0;
    uint8_t currentindex = 0;
    uint8_t portnum, length;
    uint8_t i;
    uint8_t isthreadtrue;
    _atomic_t _atomic;

    isthreadtrue = 0;
    _atomic = _atomic_start();
	
	Leds_greenOn();
	Leds_yellowOn();
    if (is_thread())
    {
        isthreadtrue = 1;
        SWAP_STACK_PTR(stackinterrupt_ptr, old_stack_ptr);
    }
    _atomic_end(_atomic);
    if (dummy == 'a')
    {
        _atomic_t _atomic = _atomic_start();

        while (!(UCSR0A & (1 << RXC0)))
            ;
        portnum = UDR0;
        portnum = portnum - 48;
        while (!(UCSR0A & (1 << RXC0)))
            ;
        length = UDR0;
        length = length - 48;
        for (i = 0; i < length; i++)
        {
            while (!(UCSR0A & (1 << RXC0)))
                ;
            receivebuffer[currentindex++] = UDR0;
        }
        _atomic_end(_atomic);
        //commandHandle(receivebuffer, currentindex); 
        //initCommandReceived(receivebuffer); 
        //serialDeliverPacket(((uint16_t) portnum), ((uint8_t) length),
                            //&receivebuffer[0]);
        currentindex = 0;
    }
    else if (dummy == 'z')
    {
        _atomic_t _atomic = _atomic_start();

        for (i = 0; i < 33; i++)
        {
            while (!(UCSR0A & (1 << RXC0)))
                ;
            receivebuffer[currentindex++] = UDR0;
        }
        _atomic_end(_atomic);
        //initCommandReceived(receivebuffer);
        currentindex = 0;
    }
    _atomic = _atomic_start();
    if (isthreadtrue == 1)
    {
        isthreadtrue = 0;
        SWAP_STACK_PTR(old_stack_ptr, stackinterrupt_ptr);
        //thread_yield();   
    }
    _atomic_end(_atomic);
    // if (is_thread())
    //{SWAP_STACK_PTR(stackinterrupt_ptr, old_stack_ptr);  }
    // if (is_thread())
    // {
    //   SWAP_STACK_PTR( old_stack_ptr, stackinterrupt_ptr);
    //  thread_yield();   
    // }
}
