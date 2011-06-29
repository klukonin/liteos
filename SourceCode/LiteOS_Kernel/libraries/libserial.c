/** @file libserial.c
       @brief The functional implementation for the serial API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/



#include "libsystem.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#define xtal 8000000L
#define baud 57600
#include "libserial.h"
#include "libstring.h"
#include "libthread.h"
#include "../types/types.h"

//These are implementation functions that are defined specifically for this file. 

static char cFlag;
static uint8_t previous;
static uint8_t status;
 
_atomic_t flag; 
 

static uint8_t localbuffer[16];


uint8_t itoa(uint16_t value, char*  str, int radix)
{
	  
    int  rem = 0;
    int  pos = 0;
    char ch  = '!' ;
    int i; 
    do
    {
        rem    = value % radix ;
        value /= radix;
        if ( 16 == radix )
        {
            if( rem >= 10 && rem <= 15 )
            {
                switch( rem )
                {
                    case 10:
                        ch = 'a' ;
                        break;
                    case 11:
                        ch ='b' ;
                        break;
                    case 12:
                        ch = 'c' ;
                        break;
                    case 13:
                        ch ='d' ;
                        break;
                    case 14:
                        ch = 'e' ;
                        break;
                    case 15:
                        ch ='f' ;
                        break;
                }
            }
        }
        if( '!' == ch )
        {
            str[pos++] = (char) ( rem + 0x30 );
        }
        else
        {
            str[pos++] = ch ;
        }
    }while( value != 0 );
   // str[pos] = '\0' ;
    
    i=0; 
    
    while (i<pos/2)
    {
        *(str+pos) = *(str+i);       //uses the null character as the temporary storage.
        *(str+i) = *(str + pos - i -1);
        *(str+pos-i-1) = *(str+pos);
        i++;
    }
   
    *(str+pos) = '\0';
    return pos;
}

//No need to be called from main 
//This function is called when the system is being initilized and prints system start 
void lib_init_usart()
{
    UBRR0H = 0;
    UBRR0L = 15;
    UCSR0A |= _BV(U2X0);        // double speed
    UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);        // 8 data-bits, 1 stop-bit
    UCSR0B |= _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
    // Enable recieve/transmit/interrupts
    cFlag = 1;
    previous = 0;
    status = 0;
}

//-------------------------------------------------------------------------
void lib_print_usart_char(uint8_t c)
{

   flag = _atomic_start();
     

    if (cFlag == 0)
    {
        lib_init_usart();
    }
    while ((UCSR0A & (1 << UDRE0)) == 0)
        ;
    UDR0 = c;
	
    _atomic_end(flag);
}

 



 
 
//-------------------------------------------------------------------------
void lib_print_usart_long(uint32_t l)
{
    uint8_t *pcByte = ((uint8_t *) (&l)) + 3;

   flag = _atomic_start();
    
    lib_print_usart_char(*pcByte);
    pcByte--;
    lib_print_usart_char(*pcByte);
    pcByte--;
    lib_print_usart_char(*pcByte);
    pcByte--;
    lib_print_usart_char(*pcByte);
   _atomic_end(flag);
	
} 

//-------------------------------------------------------------------------
void lib_printf_string_source_disabled(char *p)
{
   
   
   flag = _atomic_start();
      
	lib_print_usart_char(0xFC);
    while ((*p) != '\0')
    {
        lib_print_usart_char(*p);
        p++;
    }
    lib_print_usart_char(0xFC);
  
    _atomic_end(flag);
	
}


//-------------------------------------------------------------------------
void lib_printf_string(char *p)
{
   
   
   flag = _atomic_start();
   
   #ifdef PRINT_SOURCE_ENABLED

      itoa(lib_get_node_id(), (char *)localbuffer, 10);
      lib_printf_string_source_disabled((char *)localbuffer);
	  lib_printf_string_source_disabled(": ");	
	  
   #endif
    
	lib_print_usart_char(0xFC);
    while ((*p) != '\0')
    {
        lib_print_usart_char(*p);
        p++;
    }
    lib_print_usart_char(0xFC);
  
    _atomic_end(flag);
	
}


//-------------------------------------------------------------------------
void lib_printf_integer32(int32_t a)
{
   
   flag = _atomic_start();
  
	#ifdef PRINT_SOURCE_ENABLED   
    itoa(lib_get_node_id(), (char *)localbuffer, 10);
	lib_printf_string_source_disabled((char *)localbuffer);
    lib_printf_string_source_disabled(": ");	
	#endif 
	
    lib_print_usart_char(0xFD);
    lib_print_usart_long(*(uint32_t *) (&a));
    lib_print_usart_char(0xFD);
	
	_atomic_end(flag);
}

//-------------------------------------------------------------------------
void lib_printf_uinteger32(uint32_t l)
{
   flag = _atomic_start();
   
    #ifdef PRINT_SOURCE_ENABLED
    itoa(lib_get_node_id(), (char *)localbuffer, 10);
    lib_printf_string_source_disabled((char *)localbuffer);
    lib_printf_string_source_disabled(": ");		
    #endif 
	
    lib_print_usart_char(0xFE);
    lib_print_usart_long(l);
    lib_print_usart_char(0xFE);
	
	_atomic_end(flag);
}


//-------------------------------------------------------------------------
void lib_printf_ln()
{
#ifdef PLATFORM_AVR
    _atomic_t currentatomic;
    currentatomic = _atomic_start();
    lib_print_usart_char(0xFC);
    lib_print_usart_char(0x6D);
    lib_print_usart_char(0x6D);
    lib_print_usart_char(0x6D);
    lib_print_usart_char(0xFC);
	_atomic_end(currentatomic);
    
#endif
}


