/** @file hal_avr.h
 *
 * @brief This file implements the HAL API for the AT86RF230 radio 
 *        transceiver.
 *
 * @defgroup hal_avr_api RF230 Hardware Abstraction Layer API Functions
 *      This set of functions (Some defined as macros) is the API for the
 *      Hardware Abstraction Layer. These functions gives complete access to
 *      all of the low level functionality of the radio transceiver 
 *      (IO, SPI and ISR).
 *
 * @defgroup hal_avr_board RF230 Hardware Abstraction Layer Board Specific Configuration
 *      This set of macros are provided so that the hal is easy to port to any
 *      AVR device and board configuration. The porting is simply done by changing
 *      the macros accordingly to the schematic and selected device. The clock
 *      settings ensure that any of the supported clock frequencies are supported
 *      seamlessly during compile time.
 *      Default pin configuration (RCBs):
 *      -#  Chip Select: PB0
 *      -#  SPI Clock Signal: PB1
 *      -#  MOSI: PB2
 *      -#  MISO: PB3
 *      -#  SLP_TR: PB4 (Should: PB4-> PB7-Dong)
 *      -#  RST: PB5  (Should: PB5-> PA6-Dong)
 *      -#  CLKM: Not Used (Should: -> PD6-Dong)
 *      -#  IRQ: Timer1 Input Capture pin. Could also use pin change or external
 *          interrupt.(Should: -> PD4-Dong)
 *
 * @par Application note:
 *      AVR2001: AT86RF230 Software Programmer's Guide
 *
 * @par Documentation
 *      For comprehensive code documentation, supported compilers, compiler 
 *      settings and supported devices see readme.html
 *
 * @author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 * 

  @par
  
  Modification log:--Match the Schematics-Dong
  March 08: Added and modify pin configuration for IRIS
  # Chip Select: PB0
  # SPI Clock Signal: PB1
  # MOSI: PB2
  # MISO: PB3
  # SLP_TR: PB7
  # RST: PA6
  # CLKM: PD6
  # IRQ: PD4 Timer1 Input Capture pin. Could also use pin change or external
    interrupt.
*/


#ifndef HAL_AVR_H
#define HAL_AVR_H
/*============================ INCLUDE =======================================*/
//#include <stdint.h>
//#include <stdbool.h>
/*============================ MACROS ========================================*/

/** @brief Pin number that corresponds to the SLP_TR pin.
 * 
 *  @ingroup hal_avr_board   SLP_TR-> PB7-Dong
 */
#define SLP_TR              ( 0x07 )

/** @brief Data Direction Register that corresponds to the port where SLP_TR is 
 *         connected.
 *  @ingroup hal_avr_board
 */
#define DDR_SLP_TR          ( DDRB )

/** @brief Port (Write Access) where SLP_TR is connected.
 *  @ingroup hal_avr_board
 */
#define PORT_SLP_TR         ( PORTB )

/** @brief Pin (Read Access) where SLP_TR is connected.
 *  @ingroup hal_avr_board
 */
#define PIN_SLP_TR          ( PINB )        //Q: What is PINB?-Dong

/** @brief This macro pulls the SLP_TR pin high.
 *
 *  @ingroup hal_avr_api
 */
#define hal_set_slptr_high( )         ( PORT_SLP_TR |= ( 1 << SLP_TR ) )

/** @brief This macro pulls the SLP_TR pin low.
 *
 *  @ingroup hal_avr_api
 */
#define hal_set_slptr_low( )          ( PORT_SLP_TR &= ~( 1 << SLP_TR ) )

/** @brief  Read current state of the SLP_TR pin (High/Low).
 *
 *  @return 0 if the pin is low, 1 is the pin is high.
 *
 *  @ingroup hal_avr_api
 */
#define hal_get_slptr( ) ( ( PIN_SLP_TR & ( 1 << SLP_TR ) ) >> SLP_TR )

/** @brief Pin number that corresponds to the RST pin.
 *  @ingroup hal_avr_board   RST-> PA6-Dong
 */
#define RST                 ( 0x06 )

/** @brief Data Direction Register that corresponds to the port where RST is 
 *         connected.
 *  @ingroup hal_avr_board
 */
#define DDR_RST             ( DDRA )

/** @brief Port (Write Access) where RST is connected.
 *  @ingroup hal_avr_board
 */
#define PORT_RST            ( PORTA )

/** @brief Pin (Read Access) where RST is connected.
 *  @ingroup hal_avr_board
 */
#define PIN_RST             ( PINA )   //Q: What is PINA?-Dong
/** @brief This macro pulls the RST pin high.
 *
 *  @ingroup hal_avr_api
 */
#define hal_set_rst_high( )           ( PORT_RST |= ( 1 << RST ) )

/** @brief This macro pulls the RST pin low.
 *
 *  @ingroup hal_avr_api
 */
#define hal_set_rst_low( )            ( PORT_RST &= ~( 1 << RST ) )

/** @brief  Read current state of the RST pin (High/Low).
 *
 *  @return 0 if the pin is low, 1 if the pin is high.
 *
 *  @ingroup hal_avr_api
 */
#define hal_get_rst( ) ( ( PIN_RST & ( 1 << RST )  ) >> RST )

/** @brief The slave select pin is PB0.
 *  @ingroup hal_avr_board  Chip Select (CS)-> PB0-Dong
 */
#define HAL_SS_PIN              ( 0x00 )

/** @brief The SPI module is located on PORTB.
 *  @ingroup hal_avr_board
 */
#define HAL_PORT_SPI            ( PORTB )

/** @brief Data Direction Register for PORTB.
 *  @ingroup hal_avr_board
 */
#define HAL_DDR_SPI             ( DDRB )

/** @brief Data Direction bit for SS.
 *  @ingroup hal_avr_board
 */
#define HAL_DD_SS               ( 0x00 )  //Q: Why not Port B sepcified here-Dong

/** @brief Data Direction bit for SCK.
 *  @ingroup hal_avr_board
 */
#define HAL_DD_SCK              ( 0x01 )

/** @brief Data Direction bit for MOSI.
 *  @ingroup hal_avr_board
 */
#define HAL_DD_MOSI             ( 0x02 )

/** @brief Data Direction bit for MISO.
 *  @ingroup hal_avr_board
 */
#define HAL_DD_MISO             ( 0x03 )

#define HAL_SS_HIGH( ) (HAL_PORT_SPI |= ( 1 << HAL_SS_PIN )) //!< MACRO for pulling SS high.
#define HAL_SS_LOW( )  (HAL_PORT_SPI &= ~( 1 << HAL_SS_PIN )) //!< MACRO for pulling SS low.

/** @brief Macros defined for HAL_TIMER1.
 *
 *  These macros are used to define the correct setupt of the AVR's Timer1, and
 *  to ensure that the hal_get_system_time function returns the system time in 
 *  symbols (16 us ticks).
 *
 *  @ingroup hal_avr_board
 */

#if ( F_CPU == 16000000UL )
    #define HAL_TCCR1B_CONFIG ( ( 1 << ICES1 ) | ( 1 << CS12 ) )
    #define HAL_US_PER_SYMBOL ( 1 )
    #define HAL_SYMBOL_MASK   ( 0xFFFFffff )
#elif ( F_CPU == 8000000UL )
    #define HAL_TCCR1B_CONFIG ( ( 1 << ICES1 ) | ( 1 << CS11 ) | ( 1 << CS10 ) )
    #define HAL_US_PER_SYMBOL ( 2 )
    #define HAL_SYMBOL_MASK   ( 0x7FFFffff )
#elif ( F_CPU == 4000000UL )
    #define HAL_TCCR1B_CONFIG ( ( 1 << ICES1 ) | ( 1 << CS11 ) | ( 1 << CS10 ) )
    #define HAL_US_PER_SYMBOL ( 1 )
    #define HAL_SYMBOL_MASK   ( 0xFFFFffff )
#elif ( F_CPU == 1000000UL )
    #define HAL_TCCR1B_CONFIG ( ( 1 << ICES1 ) | ( 1 << CS11 ) )
    #define HAL_US_PER_SYMBOL ( 2 )
    #define HAL_SYMBOL_MASK   ( 0x7FFFffff )
#else
    #error "Clock speed not supported."
#endif

#define HAL_ENABLE_INPUT_CAPTURE_INTERRUPT( ) ( TIMSK1 |= ( 1 << ICIE1 ) )
#define HAL_DISABLE_INPUT_CAPTURE_INTERRUPT( ) ( TIMSK1 &= ~( 1 << ICIE1 ) )

#define HAL_ENABLE_OVERFLOW_INTERRUPT( ) ( TIMSK1 |= ( 1 << TOIE1 ) )
#define HAL_DISABLE_OVERFLOW_INTERRUPT( ) ( TIMSK1 &= ~( 1 << TOIE1 ) )

/** @brief  Enable the interrupt from the radio transceiver.
 *
 *  @ingroup hal_avr_api
 */
#define hal_enable_trx_interrupt( ) ( HAL_ENABLE_INPUT_CAPTURE_INTERRUPT( ) )

/** @brief  Disable the interrupt from the radio transceiver.
 *
 *  @return 0 if the pin is low, 1 if the pin is high.
 *
 *  @ingroup hal_avr_api
 */
#define hal_disable_trx_interrupt( ) ( HAL_DISABLE_INPUT_CAPTURE_INTERRUPT( ) )
/*============================ TYPDEFS =======================================*/
/*============================ PROTOTYPES ====================================*/

#endif
/*EOF*/
