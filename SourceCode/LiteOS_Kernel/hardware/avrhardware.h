/** @file avrhardware.h
       @brief The functional prototypes of the avr hardware definitions shared by MicaZ and IRIS. 

       @author Qing Charles Cao (cao@utk.edu)
*/

#ifndef AVRHARDWAREH
#define AVRHARDWAREH

#include "../types/types.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#if defined(PLATFORM_AVR_IRIS)
#include <avr/iom1281.h>
#elif defined(PLATFORM_AVR_MICAZ)
#include <avr/iom128.h>

#endif


/** \defgroup avrhardware Hardware related definitions.

These functions and definitions collectively form the avr hardware definitions. */

/** @{ */


#ifndef sbi
#define sbi(port, bit) ((port) |= _BV(bit))
#define cbi(port, bit) ((port) &= ~_BV(bit))
#define inp(port) (port)
#define inb(port) (port)
#define outp(value, port) ((port) = (value))
#define outb(port, value) ((port) = (value))
#define inw(port) (*(volatile uint16_t *)&(port))
#define outw(port, value) ((*(volatile uint16_t *)&(port)) = (value))
#define PRG_RDB(addr) pgm_read_byte(addr)
#endif

#if ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3))
#define __outw(val, port) outw(port, val);
#endif
#ifndef __inw
#ifndef __SFR_OFFSET
#define __SFR_OFFSET 0
#endif /* !__SFR_OFFSET */
#define __inw(_port) inw(_port)
#define __inw_atomic(__sfrport) ({				\
uint16_t __t;					\
bool bStatus;					\
bStatus = bit_is_set(SREG,7);			\
cli();						\
__t = inw(__sfrport);				\
if (bStatus) sei();				\
__t;						\
})

#endif 

/** @brief Assign PINs in hardware. */
#define LITE_ASSIGN_PIN_H(name, port, bit) \
inline void LITE_SET_##name##_PIN(void); \
inline void LITE_CLR_##name##_PIN(void); \
inline int LITE_READ_##name##_PIN(void); \
inline void LITE_MAKE_##name##_OUTPUT(void); \
inline void LITE_MAKE_##name##_INPUT(void);

/** @brief Assign output PINs in hardware. */

#define LITE_ASSIGN_OUTPUT_ONLY_PIN_H(name, port, bit) \
inline void LITE_SET_##name##_PIN(void); \
inline void LITE_CLR_##name##_PIN(void); \
inline void LITE_MAKE_##name##_OUTPUT(void);

/** @brief Assign alias PINs in hardware. */

#define LITE_ALIAS_OUTPUT_ONLY_PIN_H(alias, connector)\
inline void LITE_SET_##alias##_PIN(void); \
inline void LITE_CLR_##alias##_PIN(void); \
inline void LITE_MAKE_##alias##_OUTPUT(void); \

/** @brief Assign alias PINs in hardware. */

#define LITE_ALIAS_PIN_H(alias, connector) \
inline void LITE_SET_##alias##_PIN(void); \
inline void LITE_CLR_##alias##_PIN(void);  \
inline char LITE_READ_##alias##_PIN(void);  \
inline void LITE_MAKE_##alias##_OUTPUT(void);  \
inline void LITE_MAKE_##alias##_INPUT(void);

/** @brief Assign PIN port and names. */

#define LITE_ASSIGN_PIN(name, port, bit) \
inline void LITE_SET_##name##_PIN() {sbi(PORT##port , bit);} \
inline void LITE_CLR_##name##_PIN() {cbi(PORT##port , bit);} \
inline int LITE_READ_##name##_PIN() \
{return (inp(PIN##port) & (1 << bit)) != 0;} \
inline void LITE_MAKE_##name##_OUTPUT() {sbi(DDR##port , bit);} \
inline void LITE_MAKE_##name##_INPUT() {cbi(DDR##port , bit);}

/** @brief Assign output PIN port and names. */

#define LITE_ASSIGN_OUTPUT_ONLY_PIN(name, port, bit) \
inline void LITE_SET_##name##_PIN() {sbi(PORT##port , bit);} \
inline void LITE_CLR_##name##_PIN() {cbi(PORT##port , bit);} \
inline void LITE_MAKE_##name##_OUTPUT() {;}

/** @brief Assign output PIN port and names. */

#define LITE_ALIAS_OUTPUT_ONLY_PIN(alias, connector)\
inline void LITE_SET_##alias##_PIN() {LITE_SET_##connector##_PIN();} \
inline void LITE_CLR_##alias##_PIN() {LITE_CLR_##connector##_PIN();} \
inline void LITE_MAKE_##alias##_OUTPUT() {} \

/** @brief Assign output PIN port and names. */


#define LITE_ALIAS_PIN(alias, connector) \
inline void LITE_SET_##alias##_PIN() {LITE_SET_##connector##_PIN();} \
inline void LITE_CLR_##alias##_PIN() {LITE_CLR_##connector##_PIN();} \
inline char LITE_READ_##alias##_PIN() {return LITE_READ_##connector##_PIN();} \
inline void LITE_MAKE_##alias##_OUTPUT() {LITE_MAKE_##connector##_OUTPUT();} \
inline void LITE_MAKE_##alias##_INPUT()  {LITE_MAKE_##connector##_INPUT();}

/** @brief Signal event definition. */

#define LITE_SIGNAL(signame)					\
void signame() __attribute__ ((signal, spontaneous, C/*, used,  
             externally_visible)*/))
#define _INTERRUPT(signame)				\
void   signame(void) __attribute__((interrupt/*, used,   externally_visible*/)); \
void signame(void)

/** @brief Watchdog prescaler.
*/
     enum
     {
         LITE_period16 = 0x00,  // 47ms
         LITE_period32 = 0x01,  // 94ms
         LITE_period64 = 0x02,  // 0.19s
         LITE_period128 = 0x03, // 0.38s
         LITE_period256 = 0x04, // 0.75s
         LITE_period512 = 0x05, // 1.5s
         LITE_period1024 = 0x06,        // 3.0s
         LITE_period2048 = 0x07 // 6.0s
     };
     void wait_cycle(void);


/** @brief Avr atomic start.
	@return atomic register.
*/
     inline _atomic_t _atomic_start_avr(void);

/** @brief Avr atomic end. 
	@return Void.
*/
     inline void _atomic_end_avr(_atomic_t oldSreg);

/** @brief Avr sleep. 
       @return Void.
*/

     inline void _atomic_sleep(void);

/** @brief Avr sleep.
	@return Void.
*/	 
     inline void _avr_sleep();

/** @brief Avr enable interrupt.
       @return Void.
*/

     inline void _avr_enable_interrupt(void);

/** @brief Avr disable interrupt.
       @return Void.
*/

	 inline void _avr_disable_interrupt(void);

/** @brief Avr reset node.
	@return Void.
*/

     void avr_resetNode();


/** @brief Avr push registers.	
*/

#define PUSH_GPR()        \
__asm__("push r0");     \
__asm__("push r1");     \
__asm__("push r2");     \
__asm__("push r3");     \
__asm__("push r4");     \
__asm__("push r5");     \
__asm__("push r6");     \
__asm__("push r7");     \
__asm__("push r8");     \
__asm__("push r9");     \
__asm__("push r10");    \
__asm__("push r11");    \
__asm__("push r12");    \
__asm__("push r13");    \
__asm__("push r14");    \
__asm__("push r15");    \
__asm__("push r16");    \
__asm__("push r17");    \
__asm__("push r18");    \
__asm__("push r19");    \
__asm__("push r20");    \
__asm__("push r21");    \
__asm__("push r22");    \
__asm__("push r23");    \
__asm__("push r24");    \
__asm__("push r25");    \
__asm__("push r26");    \
__asm__("push r27");    \
__asm__("push r28");    \
__asm__("push r29");    \
__asm__("push r30");    \
__asm__("push r31")


/** @brief Avr push register status.	
*/


#define PUSH_REG_STATUS()         \
__asm__("push r31");        \
__asm__("in r31,__SREG__"); \
__asm__("push r31")


/** @brief Avr pop register status.
*/


#define POP_REG_STATUS()          \
__asm__("pop r31");         \
__asm__("out __SREG__,r31");\
__asm__("pop r31")


/** @brief Avr pop registers.
*/


#define POP_GPR()         \
__asm__("pop r31");     \
__asm__("pop r30");     \
__asm__("pop r29");     \
__asm__("pop r28");     \
__asm__("pop r27");     \
__asm__("pop r26");     \
__asm__("pop r25");     \
__asm__("pop r24");     \
__asm__("pop r23");     \
__asm__("pop r22");     \
__asm__("pop r21");     \
__asm__("pop r20");     \
__asm__("pop r19");     \
__asm__("pop r18");     \
__asm__("pop r17");     \
__asm__("pop r16");     \
__asm__("pop r15");     \
__asm__("pop r14");     \
__asm__("pop r13");     \
__asm__("pop r12");     \
__asm__("pop r11");     \
__asm__("pop r10");     \
__asm__("pop r9");      \
__asm__("pop r8");      \
__asm__("pop r7");      \
__asm__("pop r6");      \
__asm__("pop r5");      \
__asm__("pop r4");      \
__asm__("pop r3");      \
__asm__("pop r2");      \
__asm__("pop r1");      \
__asm__("pop r0")


/** @brief Avr swap stack ptr to the kernel ptr.
*/


#define SWAP_STACK_PTR(OLD, NEW) \
__asm__("in %A0, __SP_L__\n\t in %B0, __SP_H__":"=r"(OLD):);\
__asm__("out __SP_H__,%B0\n\t out __SP_L__,%A0"::"r"(NEW))


/** @brief Avr prepare stack. 
*/

#define PREPARE_REG_FOR_STACK()                               \
SWAP_STACK_PTR(old_stack_ptr, current_thread->sp);  \
__asm__("push %A0\n push %B0"::"r"(thread_func_dispatcher));\
for(i=0;i<AVR_STACK_PREPARE_LENGTH;i++)                                   \
__asm__("push __zero_reg__");                     \
SWAP_STACK_PTR(current_thread->sp, old_stack_ptr)


/** @brief Avr life cycle time. 
*/

#define LITE_CYCLE_TIME_NS 136

void inline LITE_wait_250ns(void);

void inline LITE_uwait(int u_sec);

LITE_ASSIGN_PIN_H(RED_LED, A, 2);
LITE_ASSIGN_PIN_H(GREEN_LED, A, 1);
LITE_ASSIGN_PIN_H(YELLOW_LED, A, 0);
LITE_ASSIGN_PIN_H(SERIAL_ID, A, 4);
LITE_ASSIGN_PIN_H(BAT_MON, A, 5);
LITE_ASSIGN_PIN_H(THERM_PWR, A, 7);
 
// Flash assignments
LITE_ASSIGN_PIN_H(FLASH_SELECT, A, 3);
LITE_ASSIGN_PIN_H(FLASH_CLK, D, 5);
LITE_ASSIGN_PIN_H(FLASH_OUT, D, 3);
LITE_ASSIGN_PIN_H(FLASH_IN, D, 2);
// interrupt assignments
LITE_ASSIGN_PIN_H(INT0, E, 4);
LITE_ASSIGN_PIN_H(INT1, E, 5);
LITE_ASSIGN_PIN_H(INT2, E, 6);
LITE_ASSIGN_PIN_H(INT3, E, 7);
// spibus assignments 
LITE_ASSIGN_PIN_H(MOSI, B, 2);
LITE_ASSIGN_PIN_H(MISO, B, 3);
//LITE_ASSIGN_PIN_H(SPI_OC1C, B, 7);
LITE_ASSIGN_PIN_H(SPI_SCK, B, 1);
// power control assignments
LITE_ASSIGN_PIN_H(PW0, C, 0);
LITE_ASSIGN_PIN_H(PW1, C, 1);
LITE_ASSIGN_PIN_H(PW2, C, 2);
LITE_ASSIGN_PIN_H(PW3, C, 3);
LITE_ASSIGN_PIN_H(PW4, C, 4);
LITE_ASSIGN_PIN_H(PW5, C, 5);
LITE_ASSIGN_PIN_H(PW6, C, 6);
LITE_ASSIGN_PIN_H(PW7, C, 7);
// i2c bus assignments
LITE_ASSIGN_PIN_H(I2C_BUS1_SCL, D, 0);
LITE_ASSIGN_PIN_H(I2C_BUS1_SDA, D, 1);
// uart assignments
LITE_ASSIGN_PIN_H(UART_RXD0, E, 0);
LITE_ASSIGN_PIN_H(UART_TXD0, E, 1);
LITE_ASSIGN_PIN_H(UART_XCK0, E, 2);
LITE_ASSIGN_PIN_H(AC_NEG, E, 3);        // RFID Reader Red LED
LITE_ASSIGN_PIN_H(UART_RXD1, D, 2);
LITE_ASSIGN_PIN_H(UART_TXD1, D, 3);
LITE_ASSIGN_PIN_H(UART_XCK1, D, 5);

void LITE_SET_PIN_DIRECTIONS(void);
/**@}*/
#endif //LITE_AVRHARDWARE_H
