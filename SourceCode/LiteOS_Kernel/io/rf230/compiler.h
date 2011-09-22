/** @file compiler.h

 *  @brief This file includes the correct compiler definitions for the different 
 *        architectures.
 *
 *  @par Application note:
 *      AVR2001: AT86RF230 Software Programmer's Guide
 *
 *  @par Documentation
 *      For comprehensive code documentation, supported compilers, compiler 
 *      settings and supported devices see readme.html
 *
 *  @author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 * 
 */

#ifndef COMPILER_H
#define COMPILER_H

    #if defined( AVR )
        #include "compiler_avr.h"
    #elif defined( AVR32 )
        #include "compiler_avr32.h"
    #elif defined( SAM7 )
        #include "compiler_sam7.h"
        //#error Architecture not supported yet.
    #else
        #error This files should only be compiled with the ARCHITECTURE set.
    #endif
#endif
/*EOF*/
