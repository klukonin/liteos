/** @file avrhardware.c
       @brief The functional definitions of the avr hardware definitions shared by MicaZ and IRIS. 

       @author Qing Charles Cao (cao@utk.edu)
*/


#include "avrhardware.h"
void wait_cycle()
{
    asm volatile ("nop");
    asm volatile ("nop");
}

//-------------------------------------------------------------------------
inline void _atomic_sleep()
{
    /* Atomically enable interrupts and sleep */
    sei();                      // Make sure interrupts are on, so we can wake up!
    asm volatile ("sleep");

    wait_cycle();
}

//-------------------------------------------------------------------------
inline void _avr_sleep()
{
    asm volatile ("sleep");
}

//-------------------------------------------------------------------------
inline void _avr_enable_interrupt()
{
    sei();
}

//-------------------------------------------------------------------------
inline void _avr_disable_interrupt()
{
    cli();
}

//-------------------------------------------------------------------------
_atomic_t _atomic_start_avr(void)
{
    _atomic_t result = inp(SREG);

    cli();
    return result;
}

//-------------------------------------------------------------------------
void _atomic_end_avr(_atomic_t oldSreg)
{
    outp(oldSreg, SREG);
}

//-------------------------------------------------------------------------
void avr_resetNode()
{
    asm volatile ("ldi	r30, 0x00"::);
    asm volatile ("ldi	r31, 0x00"::);
    asm volatile ("icall"::);
  
  //  __asm__ __volatile__("ldi	r30, 0x98" ::); 
  //  __asm__ __volatile__("ldi	r31, 0xEA" ::);
  //  __asm__ __volatile__("icall" ::);
}



void inline LITE_wait_250ns()
{
    asm volatile ("nop"::);
    asm volatile ("nop"::);
}

//-------------------------------------------------------------------------
void inline LITE_uwait(int u_sec)
{
    while (u_sec > 0)
    {
        asm volatile ("nop"::);
        asm volatile ("nop"::);
        asm volatile ("nop"::);
        asm volatile ("nop"::);
        asm volatile ("nop"::);
        asm volatile ("nop"::);
        asm volatile ("nop"::);
        asm volatile ("nop"::);

        u_sec--;
    }
}

// LED assignments
LITE_ASSIGN_PIN(RED_LED, A, 2);
LITE_ASSIGN_PIN(GREEN_LED, A, 1);
LITE_ASSIGN_PIN(YELLOW_LED, A, 0);
LITE_ASSIGN_PIN(SERIAL_ID, A, 4);
LITE_ASSIGN_PIN(BAT_MON, A, 5);
LITE_ASSIGN_PIN(THERM_PWR, A, 7);


// Flash assignments
LITE_ASSIGN_PIN(FLASH_SELECT, A, 3);
LITE_ASSIGN_PIN(FLASH_CLK, D, 5);
LITE_ASSIGN_PIN(FLASH_OUT, D, 3);
LITE_ASSIGN_PIN(FLASH_IN, D, 2);
// interrupt assignments
LITE_ASSIGN_PIN(INT0, E, 4);
LITE_ASSIGN_PIN(INT1, E, 5);
LITE_ASSIGN_PIN(INT2, E, 6);
LITE_ASSIGN_PIN(INT3, E, 7);
// spibus assignments 
LITE_ASSIGN_PIN(MOSI, B, 2);
LITE_ASSIGN_PIN(MISO, B, 3);
//LITE_ASSIGN_PIN(SPI_OC1C, B, 7);
LITE_ASSIGN_PIN(SPI_SCK, B, 1);
// power control assignments
LITE_ASSIGN_PIN(PW0, C, 0);
LITE_ASSIGN_PIN(PW1, C, 1);
LITE_ASSIGN_PIN(PW2, C, 2);
LITE_ASSIGN_PIN(PW3, C, 3);
LITE_ASSIGN_PIN(PW4, C, 4);
LITE_ASSIGN_PIN(PW5, C, 5);
LITE_ASSIGN_PIN(PW6, C, 6);
LITE_ASSIGN_PIN(PW7, C, 7);
// i2c bus assignments
LITE_ASSIGN_PIN(I2C_BUS1_SCL, D, 0);
LITE_ASSIGN_PIN(I2C_BUS1_SDA, D, 1);
// uart assignments
LITE_ASSIGN_PIN(UART_RXD0, E, 0);
LITE_ASSIGN_PIN(UART_TXD0, E, 1);
LITE_ASSIGN_PIN(UART_XCK0, E, 2);
LITE_ASSIGN_PIN(AC_NEG, E, 3);  // RFID Reader Red LED
LITE_ASSIGN_PIN(UART_RXD1, D, 2);
LITE_ASSIGN_PIN(UART_TXD1, D, 3);
LITE_ASSIGN_PIN(UART_XCK1, D, 5);
LITE_ALIAS_PIN(TONE_DECODE_SIGNAL, INT3);
LITE_ALIAS_PIN(PHOTO_CTL, INT1);
LITE_ALIAS_PIN(TEMP_CTL, INT2);
LITE_ALIAS_OUTPUT_ONLY_PIN(MIC_CTL, PW3);
LITE_ALIAS_OUTPUT_ONLY_PIN(SOUNDER_CTL, PW2);
LITE_ALIAS_OUTPUT_ONLY_PIN(ACCEL_CTL, PW4);
LITE_ALIAS_OUTPUT_ONLY_PIN(MAG_CTL, PW5);
LITE_ALIAS_OUTPUT_ONLY_PIN(MIC_MUX_SEL, PW6);
void LITE_SET_PIN_DIRECTIONS(void)
{
    /*  outp(0x00, DDRA);
       outp(0x00, DDRB);
       outp(0x00, DDRD);
       outp(0x02, DDRE);
       outp(0x02, PORTE);
     */
    LITE_MAKE_RED_LED_OUTPUT();
    LITE_MAKE_YELLOW_LED_OUTPUT();
    LITE_MAKE_GREEN_LED_OUTPUT();
    LITE_MAKE_PW7_OUTPUT();
    LITE_MAKE_PW6_OUTPUT();
    LITE_MAKE_PW5_OUTPUT();
    LITE_MAKE_PW4_OUTPUT();
    LITE_MAKE_PW3_OUTPUT();
    LITE_MAKE_PW2_OUTPUT();
    LITE_MAKE_PW1_OUTPUT();
    LITE_MAKE_PW0_OUTPUT();
    LITE_MAKE_SERIAL_ID_INPUT();
    LITE_CLR_SERIAL_ID_PIN();   // Prevent sourcing current
    LITE_MAKE_FLASH_SELECT_OUTPUT();
    LITE_MAKE_FLASH_OUT_OUTPUT();
    LITE_MAKE_FLASH_CLK_OUTPUT();
    LITE_SET_FLASH_SELECT_PIN();
    LITE_SET_RED_LED_PIN();
    LITE_SET_YELLOW_LED_PIN();
    LITE_SET_GREEN_LED_PIN();
}
