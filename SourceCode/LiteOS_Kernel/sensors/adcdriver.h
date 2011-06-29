/** \file adcdriver.h
       \brief The functional prototypes of the adc driver. 

       This file implements ADC, and has the following interface to the remaining of the system: 
       the APIs for reading sensors, and the variables adc_returnvalue and adc_parameters for setting up adc tasks.
*/


#ifndef ADCDRIVERH
#define ADCDRIVERH

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../types/types.h"

/** @defgroup adcdevice ADC driver.

The functions in this module perform various tasks for handling the adc sensors.  By calling these functions, the user applications can sample the ADC, including 
light, magnetic, etc, and get readings. 

*/


/** \name Channels 
Defines the channels used by the MicaZ implemenation. 
*/

/** @addtogroup adcdevice */
/** @{*/


enum
{
    LIGHT_CHANNEL = 1, TEMP_CHANNEL = 1, ACCX_CHANNEL = 3, ACCY_CHANNEL = 4,
    MAGX_CHANNEL = 5, MAGY_CHANNEL = 6
};

/** @} */
/** \name ADC Operations 
These functions perform various tasks on ADC. To use them, make sure first call adcdriver_init_adc() to initilize the adc, then, use the correponding 
function APIs. 

\note Those functions ending with task, e.g. get_light_task() return the reading from ADC in the global variable adc_returnvalue. 
*/

/** @{*/

/** @} */

/**  @brief Initilizes the ADC module.
	 @return Void.
*/
void adcdriver_init_adc();

/** @brief Get ADC reading using a particular channel. 
       \param channel The channel number as an integer input.
       \return The reading of the adc.
*/
uint16_t get_adc(int channel);

/** \brief Get light sensor reading.
       \return The reading of the light sensor.
*/
uint16_t get_light();

/** \brief Get temperature sensor reading.
	\return The reading of the temperature sensor.
*/
uint16_t get_temp();

/** \brief Get magnetic sensor reading in X axis.
	\return the reading of the magnetic sensor.
*/
uint16_t get_magx();

/** \brief Get magnetic sensor reading in Y axis.
	\return the reading of the magnetic  sensor.
*/
uint16_t get_magy();

/** \brief Get accelerator sensor reading in X axis.
	\return the reading of the accelerator sensor.
*/
uint16_t get_accx();

/** \brief Get accelerator sensor reading in Y axis.
	\return the reading of the accelerator sensor.
*/
uint16_t get_accy();


/** @} */


#endif


