/** @file socketadc.h
	@brief The functional declaration for socket adc.

	@author Qing Cao (cao@utk.edu)
*/


#ifndef SOCKETADCH
#define SOCKETADCH
#include "../sensors/adcdriver.h"

/** @addtogroup socket */
/** @{ 
*/	
/** @brief This part of definitions is used when the thread control structure applies ADC. 
	@return Void. 
*/
void ADCLight();

/** @brief  This part of definitions is used when the thread control structure applies ADC.  
	@return Void. 
*/
void ADCTemp();

/** @brief  This part of definitions is used when the thread control structure applies ADC.  
    @return Void.
*/
void ADCMagX();

/** @brief  This part of definitions is used when the thread control structure applies ADC.  
	@return Void. 
*/
void ADCMagY();

/** @brief  This part of definitions is used when the thread control structure applies ADC.  
    @return Void. 
*/
void ADCAccX();

/** @brief This part of definitions is used when the thread control structure applies ADC.  
	@return Void. 
*/
void ADCAccY();

/** @} */
#endif
