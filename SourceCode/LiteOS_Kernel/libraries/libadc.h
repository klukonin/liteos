/** @file libadc.h
       @brief The functional prototypes for the sensor API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/


#ifndef LIBADCH
#define LIBADCH

/** @defgroup api System APIs in LiteOS

       This module implements system APIs. 
*/


/** @{
*/

/** @brief Return light sensor reading. 
       @return The light sensor reading.
*/
int lib_get_light();


/**  @brief Return temperature sensor reading.
        @return The temperature sensor reading.
*/
int lib_get_temp();

/**  @brief Return magnetic sensor reading in X axis. 
        @return The magnetic sensor reading in X axis
*/
int lib_get_magx();

/**  @brief  Return magnetic sensor reading in Y axis. 
	 @return The magnetic sensor reading in Y axis
*/
int lib_get_magy();

/**  @brief  Return accelerator sensor reading in X axis. 
        @return The magnetic sensor reading in X axis
*/
int lib_get_accx();

/** @brief  Return accelerator sensor reading in Y axis. 
       @return The magnetic sensor reading in Y axis
*/
int lib_get_accy(); 

/** @} */

#endif 
