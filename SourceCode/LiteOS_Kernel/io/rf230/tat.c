/** @file tat.c
 *
 * @brief This files implements the Transceiver Access Toolbox.
 *
 * @par Application note:
 *      AVR2001:  AT86RF230 Software Programmer's Guide
 *
 * @par Documentation
 *      For comprehensive code documentation, supported compilers, compiler 
 *      settings and supported devices see readme.html
 *
 * @author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 */

 
/*============================ INCLDUE =======================================*/
//#include <stdint.h>
//#include <stdbool.h>
#include "../../types/types.h"
#include "at86rf230_registermap.h"

#include "compiler.h"
#include "tat.h"
#include "hal.h"
#include "../../io/radio/amcommon.h"

//#include "Leds.h"

/*============================ MACROS ========================================*/
#define TAT_CCA_DONE_MASK     ( 1 << 7 ) //!< Mask used to check the CCA_DONE bit.
#define TAT_CCA_IDLE_MASK     ( 1 << 6 ) //!< Mask used to check the CCA_STATUS bit.

#define TAT_START_CCA ( 1 ) //!< Value in the CCA_REQUEST subregister that initiate a cca.

#define TAT_TRANSMISSION_SUCCESS  ( 0 )
#define TAT_BUSY_CHANNEL          ( 3 )
#define TAT_MIN_IEEE_FRAME_LENGTH ( 5 )
/*============================ TYPEDEFS ======================================*/

/** @brief  This enumeration defines the necessary timing information for the 
 *          AT86RF230 radio transceiver. All times are in microseconds.
 *
 *          These constants are extracted from the datasheet.
 *
 *  @ingroup tat
 */
typedef enum{
    
    TIME_TO_ENTER_P_ON               = 510, //!< Transition time from VCC is applied to P_ON.
    TIME_P_ON_TO_TRX_OFF             = 510, //!< Transition time from P_ON to TRX_OFF.
    TIME_SLEEP_TO_TRX_OFF            = 880, //!< Transition time from SLEEP to TRX_OFF.
    TIME_RESET                       = 6,   //!< Time to hold the RST pin low during reset
    TIME_ED_MEASUREMENT              = 140, //!< Time it takes to do a ED measurement.
    TIME_CCA                         = 140, //!< Time it takes to do a CCA.
    TIME_PLL_LOCK                    = 150, //!< Maximum time it should take for the PLL to lock.
    TIME_FTN_TUNING                  = 25,  //!< Maximum time it should take to do the filter tuning.
    TIME_NOCLK_TO_WAKE               = 6,   //!< Transition time from *_NOCLK to being awake.
    TIME_CMD_FORCE_TRX_OFF           = 1,    //!< Time it takes to execute the FORCE_TRX_OFF command.
    TIME_TRX_OFF_TO_PLL_ACTIVE       = 180, //!< Transition time from TRX_OFF to: RX_ON, PLL_ON, TX_ARET_ON and RX_AACK_ON.
    TIME_STATE_TRANSITION_PLL_ACTIVE = 1, //!< Transition time from PLL active state to another.
}tat_trx_timing_t;
/*============================ VARIABLES =====================================*/
/*============================ PROTOTYPES ====================================*/
static bool2 is_sleeping( void );

/** @brief  Initialize the Transceiver Access Toolbox and lower layers.
 *
 *          If the initialization is successful the radio transceiver will be in 
 *          TRX_OFF state.          
 *
 *  @note  This function must be called prior to any of the other functions in 
 *         this file! Can be called from any transceiver state.
 *
 *  @return TAT_SUCCESS     The radio transceiver was successfully initialized 
 *                          and put into the TRX_OFF state.
 *  @return TAT_UNSUPPORTED_DEVICE  The connected device is not an Atmel 
 *                                  AT86RF230 radio transceiver.
 *  @return TAT_TIMED_OUT   The radio transceiver was not able to initialize and 
 *                          enter TRX_OFF state within the specified time.
 *
 *  @ingroup tat
 */
 //Debug-tat_init() work correctly now!
 
tat_status_t tat_init( void ){
  
     tat_status_t init_status = TAT_SUCCESS;
    
    delay_us( TIME_TO_ENTER_P_ON );
        
    //Initialize Hardware Abstraction Layer.
    hal_init( );
    
    tat_reset_trx( ); //Do HW reset of radio transeiver.
    
    
    //Force transition to TRX_OFF.
    hal_subregister_write( SR_TRX_CMD, CMD_FORCE_TRX_OFF );
    delay_us( TIME_P_ON_TO_TRX_OFF ); //Wait for the transition to be complete.
    
    
    if (tat_get_trx_state( ) != TRX_OFF) {
        
        init_status = TAT_TIMED_OUT;    
    } else {
    
        //Read Version Number
        uint8_t version_number = hal_register_read( RG_VERSION_NUM );
        
        if ((version_number != RF230_REVA ) && (version_number != RF230_REVB)) {
            init_status = TAT_UNSUPPORTED_DEVICE;
        } else {
        
            if (hal_register_read( RG_MAN_ID_0 ) != SUPPORTED_MANUFACTURER_ID) {
                init_status = TAT_UNSUPPORTED_DEVICE;
            } else {
                hal_register_write( RG_IRQ_MASK, RF230_SUPPORTED_INTERRUPT_MASK );
                 
            } // end: if (hal_register_read( RG_MAN_ID_0 ) != ...
        } // end: if ((version_number != RF230_REVA ) ...
    } // end: if (tat_get_trx_state( ) ...
    
    return init_status;
}

/** @brief  This function will return the channel used by the radio transceiver.
 *
 *  @return Current channel, 11 to 26.
 *
 *  @ingroup tat
 */
uint8_t tat_get_operating_channel( void ){
    return hal_subregister_read( SR_CHANNEL );
}

/** @brief This function will change the operating channel.
 *
 *  @param  channel New channel to operate on. Must be between 11 and 26.
 *
 *  @return TAT_SUCCESS New channel set.
 *  @return TAT_WRONG_STATE Transceiver is in a state where the channel cannot 
 *                          be changed (SLEEP).
 *  @return TAT_INVALID_ARGUMENT Channel argument is out of bounds.
 *  @return TAT_TIMED_OUT The PLL did not lock within the specified time.
 *
 *  @ingroup tat
 */
tat_status_t tat_set_operating_channel( uint8_t channel ){
    
    /*Do function parameter and state check.*/
    if ((channel < RF230_MIN_CHANNEL) || (channel > RF230_MAX_CHANNEL)) {
        return TAT_INVALID_ARGUMENT;
    }
    
    if (is_sleeping( ) == true) { return TAT_WRONG_STATE; }
    
    if (tat_get_operating_channel( ) == channel) { return TAT_SUCCESS; }
    
    /*Set new operating channel.*/        
    hal_subregister_write( SR_CHANNEL, channel );
                
    //Read current state and wait for the PLL_LOCK interrupt if the
    //radio transceiver is in either RX_ON or PLL_ON.
    uint8_t trx_state = tat_get_trx_state( );
                
    if ((trx_state == RX_ON) || (trx_state == PLL_ON)) {
        delay_us( TIME_PLL_LOCK );
    }
    
    tat_status_t channel_set_status = TAT_TIMED_OUT;
    
    //Check that the channel was set properly.
    if (tat_get_operating_channel( ) == channel) {
        channel_set_status = TAT_SUCCESS;
    }
    
    return channel_set_status;
}

/** @brief This function will read and return the output power level.
 *
 *  @returns 0 to 15 Current output power in "TX power settings" as defined in 
 *          the radio transceiver's datasheet
 *
 *  @ingroup tat
 */
uint8_t tat_get_tx_power_level( void ){
    return hal_subregister_read( SR_TX_PWR );
}

/** @brief This function will change the output power level.
 *
 *  @param  power_level New output power level in the "TX power settings" 
 *                      as defined in the radio transceiver's datasheet.
 *
 *  @return TAT_SUCCESS New output power set successfully.
 *  @return TAT_INVALID_ARGUMENT The supplied function argument is out of bounds.
 *  @return TAT_WRONG_STATE It is not possible to change the TX power when the 
 *                          device is sleeping.
 *
 *  @ingroup tat
 */
tat_status_t tat_set_tx_power_level( uint8_t power_level ){
        
    /*Check function parameter and state.*/
    if (power_level > TX_PWR_17_2DBM) { return TAT_INVALID_ARGUMENT; }
    
    if (is_sleeping( ) == true) { return TAT_WRONG_STATE; }
    
    /*Set new power level*/
    hal_subregister_write( SR_TX_PWR, power_level );
        
    return TAT_SUCCESS;
}

/** @brief This function will measure the amount of energy sensed on the antenna 
 *         pins.
 *
 *  @param ed_level  This is a pointer used to return the measured energy. The 
 *                   value is only written if the function returns with 
 *                   TAT_SUCCESS. The written value is 0 to 84.
 *
 *  @return TAT_SUCCESS The detection was successful.
 *  @return TAT_WRONG_STATE The energy detection can only be done if the radio
 *                          transceiver's state is RX_ON or BUSY_RX.
 *  @ingroup tat
 */
tat_status_t tat_do_ed_scan( uint8_t *ed_level ){

    uint8_t current_state = tat_get_trx_state( );
    
    /*Check state. Only possible to do ed measurement from RX_ON or BUSY_RX.*/
    if ((current_state != RX_ON ) && (current_state != BUSY_RX)) {
        return TAT_WRONG_STATE;
    }
    
    /*Do ED measurement.*/    
    //Initiate the measurement by writing to the PHY_ED_LEVEL register.
    hal_register_write( RG_PHY_ED_LEVEL, 0 );
    delay_us( TIME_ED_MEASUREMENT ); //Wait 128 us + 12 us computation time.
    *ed_level = hal_register_read( RG_PHY_ED_LEVEL ); //Write ED level to pointer.

    return TAT_SUCCESS;
}

/** @brief This function returns the current CCA mode used.
 *
 *  @return CCA mode currently used, 0 to 3.
 *
 *  @ingroup tat
 */
uint8_t tat_get_cca_mode( void ){           
    return hal_subregister_read( SR_CCA_MODE );
}

/** @brief This function returns the current ED threshold used by the CCA algorithm.
 *
 *  @return Current ED threshold, 0 to 15.
 *
 *  @ingroup tat
 */
uint8_t tat_get_ed_threshold( void ){
    return hal_subregister_read( SR_CCA_ED_THRES );
}

/** @brief This function will configure the Clear Channel Assessment algorithm.
 *
 *  @param  mode Three modes are available: Energy above threshold, carrier 
 *               sense only and carrier sense with energy above threshold.
 *  @param  ed_threshold Above this energy threshold the channel is assumed to be 
 *                      busy. The threshold is given in positive dBm values. 
 *                      Ex. -91 dBm gives a csThreshold of 91. Value range for 
 *                      the variable is [61 to 91]. Only valid for the CCA_ED 
 *                      and CCA_CARRIER_SENSE_ED modes.
 *
 *  @return TAT_SUCCESS Mode and its parameters successfully changed.
 *  @return TAT_WRONG_STATE This function cannot be called in the SLEEP state.
 *  @return TAT_INVALID_ARGUMENT If one of the three function arguments are out 
 *                               of bounds.
 *
 *  @ingroup tat
 */
tat_status_t tat_set_cca_mode( uint8_t mode, uint8_t ed_threshold ){
    
    /*Check function parameters and state.*/
    if ((mode != CCA_ED) && (mode != CCA_CARRIER_SENSE) && 
        (mode != CCA_CARRIER_SENSE_WITH_ED)) {
        return TAT_INVALID_ARGUMENT;
    } 
       
    //Ensure that the ED threshold is within bounds.
    if (ed_threshold > RF230_MAX_ED_THRESHOLD) { return TAT_INVALID_ARGUMENT; } 
            
    //Ensure that the radio transceiver is not sleeping.
    if (is_sleeping( ) == true) { return TAT_WRONG_STATE; } 
                
    /*Change cca mode and ed threshold.*/
    hal_subregister_write( SR_CCA_MODE, mode );
    hal_subregister_write( SR_CCA_ED_THRES, ed_threshold );
        
    return TAT_SUCCESS;
}

/** @brief  This function will perform a Clear Channel Assessment.
 *
 *  @note The state after the cca mesasurement will be RX_ON.
 *
 *  @return TAT_CCA_IDLE  Channel is ready for transmission.
 *  @return TAT_CCA_BUSY  Channel is not ready for transmission.
 *  @return TAT_WRONG_STATE CCA can only be done in PLL_ON.
 *  @return TAT_TIMED_OUT CCA algorithm timed out.
 *   
 *  @ingroup tat
 */
tat_status_t tat_do_cca( void ){

    /*Check state. CCA measurement only possible form PLL_ON state.*/
    if (tat_get_trx_state( ) != PLL_ON) { return TAT_WRONG_STATE; }
    
    /*Ensure that it is possible to enter RX_ON*/
    if (tat_set_trx_state( RX_ON ) != TAT_SUCCESS) { return TAT_TIMED_OUT; }
            
    //The CCA is initiated by writing 1 to the CCA_REQUEST subregister.
    hal_subregister_write( SR_CCA_REQUEST, TAT_START_CCA );
            
    //The CCA is measured over 128 us + 12 us computation time.
    delay_us( TIME_CCA );
    
    uint8_t status = hal_register_read( RG_TRX_STATUS ); //Read cca status.
    tat_status_t cca_status = TAT_CCA_BUSY; //Return variable.
    
    //Check if the algorithm finished -> CCA_DONE == 1.
    if ((status & TAT_CCA_DONE_MASK) != TAT_CCA_DONE_MASK) {
        cca_status = TAT_TIMED_OUT;
    } else {
                
        //CCA done, but check if the channel is busy or not.
        if ((status & TAT_CCA_IDLE_MASK) != TAT_CCA_IDLE_MASK) {
            cca_status = TAT_CCA_BUSY;
        } else {
            cca_status = TAT_CCA_IDLE;
        } // end: if ((status & TAT_CCA_IDLE_MASK) ...
    } // end: if ((status & TAT_CCA_DONE_MASK) ...
    
    return cca_status;
}

/** @brief This function returns the Received Signal Strength Indication.
 *
 *  @note This function should only be called from the: RX_ON and BUSY_RX. This
 *        can be ensured by reading the current state of the radio transceiver 
 *        before executing this function!
 *  @param rssi Pointer to memory location where RSSI value should be written.
 *  @return TAT_SUCCESS The RSSI measurement was successful.
 *  @return TAT_WRONG_STATE The radio transceiver is not in RX_ON or BUSY_RX.
 *
 *  @ingroup tat
 */
tat_status_t tat_get_rssi_value( uint8_t *rssi ){
    
    uint8_t current_state = tat_get_trx_state( );
    tat_status_t retval = TAT_WRONG_STATE;
    
    /*The RSSI measurement should only be done in RX_ON or BUSY_RX.*/
    if ((current_state == RX_ON) || (current_state == BUSY_RX)) {
        
        *rssi = hal_subregister_read( SR_RSSI );
        retval = TAT_SUCCESS;
    }
    
    return retval;
}

/** @brief This function returns the current threshold volatge used by the 
 *         battery monitor (BATMON_VTH). 
 *
 *  @note This function can not be called from P_ON or SLEEP. This is ensured
 *        by reading the device state before calling this function.
 *
 *  @return Current threshold voltage, 0 to 15.
 *
 *  @ingroup tat
 */
uint8_t tat_batmon_get_voltage_threshold( void ){
    return hal_subregister_read( SR_BATMON_VTH );
}

/** @brief This function returns if high or low voltage range is used. 
 *
 *  @note This function can not be called from P_ON or SLEEP. This is ensured
 *        by reading the device state before calling this function.
 *
 *  @return 0 Low voltage range selected.
 *  @return 1 High voltage range selected.
 *
 *  @ingroup tat
 */
uint8_t tat_batmon_get_voltage_range( void ){
    return hal_subregister_read( SR_BATMON_HR );
}

/** @brief This function is used to configure the battery monitor module 
 *
 *  @param range True means high voltage range and false low voltage range.
 *  @param voltage_threshold The datasheet defines 16 voltage levels for both
 *                          low and high range.
 *  @return TAT_SUCCESS Battery monitor configured
 *  @return TAT_WRONG_STATE The device is sleeping.
 *  @return TAT_INVALID_ARGUMENT The voltage_threshold parameter is out of 
 *                               bounds (Not within [0 - 15]).
 *  @ingroup tat
 */
tat_status_t tat_batmon_configure( bool2 range, uint8_t voltage_threshold ){
     
    /*Check function parameters and state.*/
    if (voltage_threshold > BATTERY_MONITOR_HIGHEST_VOLTAGE) {
        return TAT_INVALID_ARGUMENT;
    }   
    
    if (is_sleeping( ) == true) { return TAT_WRONG_STATE; }
    
    /*Write new voltage range and voltage level.*/
    if (range == true) {
        hal_subregister_write( SR_BATMON_HR, BATTERY_MONITOR_HIGH_VOLTAGE );
    } else {
        hal_subregister_write( SR_BATMON_HR, BATTERY_MONITOR_LOW_VOLTAGE );
    } // end: if (range == true) ...
    
    hal_subregister_write( SR_BATMON_VTH, voltage_threshold );
       
    return TAT_SUCCESS;
}

/** @brief This function returns the status of the Battery Monitor module.
 *
 *  @note This function can not be called from P_ON or SLEEP. This is ensured
 *        by reading the device state before calling this function.
 *
 *  @return TAT_BAT_LOW Battery voltage is below the programmed threshold.
 *  @return TAT_BAT_OK Battery voltage is above the programmed threshold.
 *
 *  @ingroup tat
 */
tat_status_t tat_batmon_get_status( void ){
    
    tat_status_t batmon_status = TAT_BAT_LOW;
    
    if (hal_subregister_read( SR_BATMON_OK ) != 
        BATTERY_MONITOR_VOLTAGE_UNDER_THRESHOLD) {
        batmon_status = TAT_BAT_OK;
    }
    
    return batmon_status;
}

/** @brief This function returns the current clock setting for the CLKM pin.
 *
 *  @return CLKM_DISABLED CLKM pin is disabled.
 *  @return CLKM_1MHZ CLKM pin is prescaled to 1 MHz.
 *  @return CLKM_2MHZ CLKM pin is prescaled to 2 MHz.
 *  @return CLKM_4MHZ CLKM pin is prescaled to 4 MHz.
 *  @return CLKM_8MHZ CLKM pin is prescaled to 8 MHz.
 *  @return CLKM_16MHZ CLKM pin is not prescaled. Output is 16 MHz.
 *
 *  @ingroup tat
 */
uint8_t tat_get_clock_speed( void ){
    return hal_subregister_read( SR_CLKM_CTRL );
}

/** @brief This function changes the prescaler on the CLKM pin.
 *
 *  @param direct   This boolean variable is used to determine if the frequency 
 *                  of the CLKM pin shall be changed directly or not. If direct
 *                  equals true, the frequency will be changed directly. This is 
 *                  fine if the CLKM signal is used to drive a timer etc. on the
 *                  connected microcontroller. However, the CLKM signal can also
 *                  be used to clock the microcontroller itself. In this situation
 *                  it is possible to change the CLKM frequency indirectly 
 *                  (direct == false). When the direct argument equlas false, the
 *                  CLKM frequency will be changed first after the radio transceiver
 *                  has been taken to SLEEP and awaken again.
 *  @param clock_speed This parameter can be one of the following constants: 
 *                     CLKM_DISABLED, CLKM_1MHZ, CLKM_2MHZ, CLKM_4MHZ, CLKM_8MHZ
 *                     or CLKM_16MHZ.
 *
 *  @return TAT_SUCCESS Clock speed updated. New state is TRX_OFF.
 *  @return TAT_INVALID_ARGUMENT Requested clock speed is out of bounds.
 *  
 * @ingroup tat
 */
tat_status_t tat_set_clock_speed( bool2 direct, uint8_t clock_speed ){
    
    /*Check function parameter and current clock speed.*/
    if (clock_speed > CLKM_16MHZ) { return TAT_INVALID_ARGUMENT; }
        
    if (tat_get_clock_speed( ) == clock_speed) { return TAT_SUCCESS; }
    
    /*Select to change the CLKM frequency directly or after returning from SLEEP.*/
    if (direct == false) {
        hal_subregister_write( SR_CLKM_SHA_SEL, 1 );
    } else {
        hal_subregister_write( SR_CLKM_SHA_SEL, 0 );
    } // end: if (direct == false) ...
            
    hal_subregister_write( SR_CLKM_CTRL, clock_speed );
                
    return TAT_SUCCESS;
}

/** @brief  This function calibrates the Single Side Band Filter.
 *
 *  @return     TAT_SUCCESS    Filter is calibrated.
 *  @return     TAT_TIMED_OUT  The calibration could not be completed within time.
 *  @return     TAT_WRONG_STATE This function can only be called from TRX_OFF or 
 *              PLL_ON.
 *   
 *  @ingroup tat
 */
tat_status_t tat_calibrate_filter( void ){
    
    /*Check current state. Only possible to do filter calibration from TRX_OFF or PLL_ON.*/
    uint8_t trx_state = tat_get_trx_state( );
    
    if ((trx_state != TRX_OFF ) && (trx_state != PLL_ON)) { return TAT_WRONG_STATE; }
        
    //Start the tuning algorithm by writing one to the FTN_START subregister.
    hal_subregister_write( SR_FTN_START, 1 );   
    delay_us( TIME_FTN_TUNING ); //Wait for the calibration to finish.
    
    tat_status_t filter_calibration_status = TAT_TIMED_OUT;    
    
    //Verify the calibration result.
    if (hal_subregister_read( SR_FTN_START ) == FTN_CALIBRATION_DONE) {
        filter_calibration_status = TAT_SUCCESS;
    }
    
    return filter_calibration_status;
}

/** @brief  This function calibrates the PLL.
 *
 *  @return     TAT_SUCCESS    PLL Center Frequency and Delay Cell is calibrated.
 *  @return     TAT_TIMED_OUT  The calibration could not be completed within time.
 *  @return     TAT_WRONG_STATE This function can only be called from PLL_ON.
 *   
 *  @ingroup tat
 */
tat_status_t tat_calibrate_pll( void ){
    
    /*Check current state. Only possible to calibrate PLL from PLL_ON state*/
    if (tat_get_trx_state( ) != PLL_ON) { return TAT_WRONG_STATE; }
    
    //Initiate the DCU and CF calibration loops.
    hal_subregister_write( SR_PLL_DCU_START, 1 );
    hal_subregister_write( SR_PLL_CF_START, 1 );
        
    //Wait maximum 150 us for the PLL to lock.
    hal_clear_pll_lock_flag( );
    delay_us( TIME_PLL_LOCK );
    
    tat_status_t pll_calibration_status = TAT_TIMED_OUT;
    
    if (hal_get_pll_lock_flag( ) > 0) {
    
        if (hal_subregister_read( SR_PLL_DCU_START ) == PLL_DCU_CALIBRATION_DONE) {
        
            if (hal_subregister_read( SR_PLL_CF_START ) == PLL_CF_CALIBRATION_DONE) {
                pll_calibration_status = TAT_SUCCESS;    
            } // end: if (hal_subregister_read( SR_PLL_CF_START ) ...
        } // end: if (hal_subregister_read( SR_PLL_DCU_START ) ...
    } // end: if ((hal_get_pll_lock_flag( ) ...
    
    return pll_calibration_status;
}

/** @brief  This function return the Radio Transceivers current state.
 *
 *  @return     P_ON               When the external supply voltage (VDD) is 
 *                                 first supplied to the transceiver IC, the 
 *                                 system is in the P_ON (Poweron) mode. 
 *  @return     BUSY_RX            The radio transceiver is busy receiving a 
 *                                 frame.
 *  @return     BUSY_TX            The radio transceiver is busy transmitting a 
 *                                 frame.
 *  @return     RX_ON              The RX_ON mode enables the analog and digital 
 *                                 receiver blocks and the PLL frequency 
 *                                 synthesizer.
 *  @return     TRX_OFF            In this mode, the SPI module and crystal 
 *                                 oscillator are active.
 *  @return     PLL_ON             Entering the PLL_ON mode from TRX_OFF will 
 *                                 first enable the analog voltage regulator. The
 *                                 transceiver is ready to transmit a frame.
 *  @return     BUSY_RX_AACK       The radio was in RX_AACK_ON mode and received 
 *                                 the Start of Frame Delimiter (SFD). State 
 *                                 transition to BUSY_RX_AACK is done if the SFD
 *                                 is valid. 
 *  @return     BUSY_TX_ARET       The radio transceiver is busy handling the 
 *                                 auto retry mechanism.
 *  @return     RX_AACK_ON         The auto acknowledge mode of the radio is 
 *                                 enabled and it is waiting for an incomming 
 *                                 frame.
 *  @return     TX_ARET_ON         The auto retry mechanism is enabled and the 
 *                                 radio transceiver is waiting for the user to 
 *                                 send the TX_START command.
 *  @return     RX_ON_NOCLK        The radio transceiver is listening for 
 *                                 incomming frames, but the CLKM is disabled so 
 *                                 that the controller could be sleeping. 
 *                                 However, this is only true if the controller 
 *                                 is run from the clock output of the radio. 
 *  @return     RX_AACK_ON_NOCLK   Same as the RX_ON_NOCLK state, but with the 
 *                                 auto acknowledge module turned on. 
 *  @return     BUSY_RX_AACK_NOCLK Same as BUSY_RX_AACK, but the controller 
 *                                 could be sleeping since the CLKM pin is 
 *                                 disabled.
 *  @return     STATE_TRANSITION   The radio transceiver's state machine is in 
 *                                 transition between two states.
 *   
 *  @ingroup tat
 */
uint8_t tat_get_trx_state( void ){
	//Debug
	    //printfstr ("\n GET STATUS"); 
      //printfintegeru32(hal_subregister_read( SR_TRX_STATUS ));
    return hal_subregister_read( SR_TRX_STATUS );
}

/** @brief  This function checks if the radio transceiver is sleeping.
 *
 *  @return     true    The radio transceiver is in SLEEP or one of the *_NOCLK 
 *                      states.
 *  @return     false   The radio transceiver is not sleeping.
 *   
 *  @ingroup tat
 */
static bool2 is_sleeping( void ){
    
    bool2 sleeping = false;
    
    //The radio transceiver will be at SLEEP or one of the *_NOCLK states only if
    //the SLP_TR pin is high.
    if (hal_get_slptr( ) != 0) {
        sleeping = true;
    }
    
    return sleeping;
}

/** @brief  This function will change the current state of the radio 
 *          transceiver's internal state machine.
 *
 *  @param     new_state        Here is a list of possible states: 
 *             - RX_ON        Requested transition to RX_ON state.
 *             - TRX_OFF      Requested transition to TRX_OFF state.
 *             - PLL_ON       Requested transition to PLL_ON state.
 *             - RX_AACK_ON   Requested transition to RX_AACK_ON state.
 *             - TX_ARET_ON   Requested transition to TX_ARET_ON state.
 *
 *  @return    TAT_SUCCESS          Requested state transition completed 
 *                                  successfully.
 *  @return    TAT_INVALID_ARGUMENT Supplied function parameter out of bounds.  
 *  @return    TAT_WRONG_STATE      Illegal state to do transition from.
 *  @return    TAT_BUSY_STATE       The radio transceiver is busy.
 *  @return    TAT_TIMED_OUT        The state transition could not be completed 
 *                                  within resonable time.
 *
 *  @ingroup tat
 */
tat_status_t tat_set_trx_state( uint8_t new_state ){
    
    /*Check function paramter and current state of the radio transceiver.*/
    if (!((new_state == TRX_OFF ) || (new_state == RX_ON) || (new_state == PLL_ON) || 
        (new_state == RX_AACK_ON ) || (new_state == TX_ARET_ON ))) { 
            
        return TAT_INVALID_ARGUMENT; 
    }
    
    if (is_sleeping( ) == true) { return TAT_WRONG_STATE; }
    
    uint8_t original_state = tat_get_trx_state( );
    
    if ((original_state == BUSY_RX ) || (original_state == BUSY_TX) || 
        (original_state == BUSY_RX_AACK) || (original_state == BUSY_TX_ARET)) { 
        return TAT_BUSY_STATE;
    }
    
    if (new_state == original_state) { return TAT_SUCCESS; }
                        
    //At this point it is clear that the requested new_state is:
    //TRX_OFF, RX_ON, PLL_ON, RX_AACK_ON or TX_ARET_ON.
                
    //The radio transceiver can be in one of the following states:
    //TRX_OFF, RX_ON, PLL_ON, RX_AACK_ON, TX_ARET_ON.
    if( new_state == TRX_OFF ){
        tat_reset_state_machine( ); //Go to TRX_OFF from any state.
    } else {
        
        //It is not allowed to go from RX_AACK_ON or TX_AACK_ON and directly to
        //TX_AACK_ON or RX_AACK_ON respectively. Need to go via RX_ON or PLL_ON.
        if ((new_state == TX_ARET_ON) && (original_state == RX_AACK_ON)) {
            
            //First do intermediate state transition to PLL_ON, then to TX_ARET_ON.
            //The final state transition to TX_ARET_ON is handled after the if-else if.
            hal_subregister_write( SR_TRX_CMD, PLL_ON );
            delay_us( TIME_STATE_TRANSITION_PLL_ACTIVE );
        } else if ((new_state == RX_AACK_ON) && (original_state == TX_ARET_ON)) {
            
            //First do intermediate state transition to RX_ON, then to RX_AACK_ON.
            //The final state transition to RX_AACK_ON is handled after the if-else if.
            hal_subregister_write( SR_TRX_CMD, RX_ON );
            delay_us( TIME_STATE_TRANSITION_PLL_ACTIVE );
        }
            
        //Any other state transition can be done directly.    
        hal_subregister_write( SR_TRX_CMD, new_state );
        
        //When the PLL is active most states can be reached in 1us. However, from
        //TRX_OFF the PLL needs time to activate.
        if (original_state == TRX_OFF) {
            delay_us( TIME_TRX_OFF_TO_PLL_ACTIVE );
        } else {
            delay_us( TIME_STATE_TRANSITION_PLL_ACTIVE );
        } // end: if (original_state == TRX_OFF) ...
    } // end: if( new_state == TRX_OFF ) ...
        
    /*Verify state transition.*/
    tat_status_t set_state_status = TAT_TIMED_OUT;
    
    if( tat_get_trx_state( ) == new_state ){ set_state_status = TAT_SUCCESS; }
    
    return set_state_status;
}

/** @brief  This function will put the radio transceiver to sleep.
 *
 *  @return    TAT_SUCCESS          Sleep mode entered successfully.
 *  @return    TAT_TIMED_OUT        The transition to TRX_OFF took too long.
 *
 *  @ingroup tat
 */
tat_status_t tat_enter_sleep_mode( void ){
    
    if (is_sleeping( ) == true) { return TAT_SUCCESS; }

    tat_reset_state_machine( ); //Force the device into TRX_OFF.
    
    tat_status_t enter_sleep_status = TAT_TIMED_OUT;
    
    if (tat_get_trx_state( ) == TRX_OFF) {
            
        //Enter Sleep.
        hal_set_slptr_high( );    
        enter_sleep_status = TAT_SUCCESS;
    }
    
    return enter_sleep_status;  
}

/** @brief  This function will take the radio transceiver from sleep mode and 
 *          put it into the TRX_OFF state.
 *
 *  @return    TAT_SUCCESS          Left sleep mode and entered TRX_OFF state.
 *  @return    TAT_TIMED_OUT        Transition to TRX_OFF state timed out.
 *
 *  @ingroup tat
 */
tat_status_t tat_leave_sleep_mode( void ){
    
    //Check if the radio transceiver is actually sleeping.
    if (is_sleeping( ) == false) { return TAT_SUCCESS; }
    
    hal_set_slptr_low( );
    delay_us( TIME_SLEEP_TO_TRX_OFF );
    
    tat_status_t leave_sleep_status = TAT_TIMED_OUT;
    
    //Ensure that the radio transceiver is in the TRX_OFF state.
    if (tat_get_trx_state( ) == TRX_OFF) {
        leave_sleep_status = TAT_SUCCESS;
    }
    
    return leave_sleep_status;
}

/** @brief  This function will reset the state machine (to TRX_OFF) from any of 
 *          its states, except for the SLEEP state.
 *
 *  @ingroup tat
 */
void tat_reset_state_machine( void ){
    
    hal_set_slptr_low( );
    delay_us( TIME_NOCLK_TO_WAKE );
    hal_subregister_write( SR_TRX_CMD, CMD_FORCE_TRX_OFF );
    delay_us( TIME_CMD_FORCE_TRX_OFF );
}

/** @brief  This function will reset all the registers and the state machine of 
 *          the radio transceiver.
 *
 *  @ingroup tat
 */
void tat_reset_trx( void ){

    hal_set_rst_low( );
    hal_set_slptr_low( );
    delay_us( TIME_RESET );    
    hal_set_rst_high( );
}

/** @brief  This function will enable or disable automatic CRC during frame 
 *          transmission.
 *
 *  @param  auto_crc_on If this parameter equals true auto CRC will be used for 
 *                      all frames to be transmitted. The framelength must be 
 *                      increased by two bytes (16 bit CRC). If the parameter equals 
 *                      false, the automatic CRC will be disabled.
 *
 *  @ingroup tat
 */
void tat_use_auto_tx_crc( bool2 auto_crc_on ){
    
    if (auto_crc_on == true) {
        hal_subregister_write( SR_TX_AUTO_CRC_ON, 1 );
    } else {
        hal_subregister_write( SR_TX_AUTO_CRC_ON, 0 );
    } // end: if (auto_crc_on == true) ...
}

/** @brief  This function will download a frame to the radio transceiver's 
 *          transmit buffer and send it.
 *
 *  @param  data_length Length of the frame to be transmitted. 1 to 128 bytes are the valid lengths. 
 *  @param  *data   Pointer to the data to transmit
 *
 *  @return TAT_SUCCESS Frame downloaded and sent successfully.
 *  @return TAT_INVALID_ARGUMENT If the dataLength is 0 byte or more than 127 
 *                               bytes the frame will not be sent.
 *  @return TAT_WRONG_STATE It is only possible to use this function in the 
 *                          PLL_ON and TX_ARET_ON state. If any other state is 
 *                          detected this error message will be returned.
 *
 *  @ingroup tat
 */
__x tat_status_t tat_send_data( uint8_t data_length, uint8_t *data ){
    
    /*Check function parameters and current state.*/
    if (data_length > RF230_MAX_TX_FRAME_LENGTH) {
    	 //Leds_redToggle();  //Debug--Redtoggle Upon-Good here
    	 return TAT_INVALID_ARGUMENT;
    	  }
    
    if ((tat_get_trx_state( ) != PLL_ON)) { 
    	/* //Debug-Not go into this condition-Correct
    	 //Leds_redToggle();  //Debug--Redtoggle Upon-Good here
    	  printfstr ("\n Send Wrong Status: "); 
        printfintegeru32(tat_get_trx_state( ));*/
    	return TAT_WRONG_STATE; 
    	}
      
    /*Do frame transmission.*/
    //Toggle the SLP_TR pin to initiate the frame transmission.
    /*//Debug    
    // Leds_redToggle();  //Debug--Redtoggle Upon   
     printfstr ("\n Send Correct Status: "); 
     printfintegeru32((uint32_t)tat_get_trx_state( ));*/
   
    hal_set_slptr_high( );
    hal_set_slptr_low( );
   // Leds_redToggle();  //Debug--Redtoggle Upon        
    hal_frame_write( data, data_length ); //Then write data to the frame buffer.
           
    return TAT_SUCCESS; 
}

/** @brief  This function will read the I_AM_COORD sub register.
 *
 *  @return 0 Not coordinator.
 *  @return 1 Coordinator role enabled.
 *
 *  @ingroup tat
 */
uint8_t tat_get_device_role( void ){
    return hal_subregister_read( SR_I_AM_COORD);
}

/** @brief  This function will set the I_AM_COORD sub register.
 *
 *  @param[in] i_am_coordinator If this parameter is true, the associated 
 *                              coordinator role will be enabled in the radio 
 *                              transceiver's address filter. 
 *                              False disables the same feature.
 *  @ingroup tat
 */
void tat_set_device_role( bool2 i_am_coordinator ){
    
    if (i_am_coordinator == true) {
        hal_subregister_write( SR_I_AM_COORD, 0);
    } else {
        hal_subregister_write( SR_I_AM_COORD, 0);
    } // end: if (i_am_coordinator == true) ...
}

/** @brief  This function will return the PANID used by the address filter.
 *
 *  @return Any value from 0 to 0xFFFF.
 *
 *  @ingroup tat
 */
uint16_t tat_get_pan_id( void ){
    
    uint8_t pan_id_15_8 = hal_register_read( RG_PAN_ID_1 ); // Read pan_id_15_8.
    uint8_t pan_id_7_0 = hal_register_read( RG_PAN_ID_0 ); // Read pan_id_7_0.
    
    uint16_t pan_id = ((uint16_t)(pan_id_15_8 << 8)) | pan_id_7_0;
    
    return pan_id;
}

/** @brief  This function will set the PANID used by the address filter.
 *
 *  @param  new_pan_id Desired PANID. Can be any value from 0x0000 to 0xFFFF 
 *
 *  @ingroup tat
 */
void tat_set_pan_id( uint16_t new_pan_id ){
    
    uint8_t pan_byte = new_pan_id & 0xFF; // Extract new_pan_id_7_0.
    hal_register_write( RG_PAN_ID_0, pan_byte );
    
    pan_byte = (new_pan_id >> 8*1) & 0xFF;  // Extract new_pan_id_15_8.
    hal_register_write( RG_PAN_ID_1, pan_byte );    
}

/** @brief  This function will return the current short address used by the 
 *          address filter.
 *
 *  @return Any value from 0x0000 to 0xFFFF
 *
 *  @ingroup tat
 */
uint16_t tat_get_short_address( void ){

    uint8_t short_address_15_8 = hal_register_read( RG_SHORT_ADDR_1 ); // Read short_address_15_8.
    uint8_t short_address_7_0  = hal_register_read( RG_SHORT_ADDR_1 ); // Read short_address_7_0.
    
    uint16_t short_address = ((uint16_t)(short_address_15_8 << 8)) | short_address_7_0;
    
    return short_address;
}

/** @brief  This function will set the short address used by the address filter.
 *
 *  @param  new_short_address Short address to be used by the address filter. 
 *
 *  @ingroup tat
 */
void tat_set_short_address( uint16_t new_short_address ){
    
    uint8_t short_address_byte = new_short_address & 0xFF; // Extract short_address_7_0.
    hal_register_write( RG_SHORT_ADDR_0, short_address_byte );
    
    short_address_byte = (new_short_address >> 8*1) & 0xFF; // Extract short_address_15_8.
    hal_register_write( RG_SHORT_ADDR_1, short_address_byte );
}

/** @brief  This function will read the extended address used by the address 
 *          filter.
 *
 *  @note In this function a pointer is used to convey the 64-bit result, since
 *        it is very inefficient to use the stack for this.
 *
 *  @return Extended Address, any 64-bit value.
 *
 *  @ingroup tat
 */
__x void tat_get_extended_address( uint8_t *extended_address ){
    
    *extended_address++ = hal_register_read( RG_IEEE_ADDR_7 ); // Read ieee_address_63_56.
    *extended_address++ = hal_register_read( RG_IEEE_ADDR_6 ); // Read ieee_address_55_48.
    *extended_address++ = hal_register_read( RG_IEEE_ADDR_5 ); // Read ieee_address_47_40.
    *extended_address++ = hal_register_read( RG_IEEE_ADDR_4 ); // Read ieee_address_39_32.
    *extended_address++ = hal_register_read( RG_IEEE_ADDR_3 ); // Read ieee_address_31_24.
    *extended_address++ = hal_register_read( RG_IEEE_ADDR_2 ); // Read ieee_address_23_16.
    *extended_address++ = hal_register_read( RG_IEEE_ADDR_1 ); // Read ieee_address_15_8.
    *extended_address   = hal_register_read( RG_IEEE_ADDR_0 ); // Read ieee_address_7_0.
}

/** @brief  This function will set a new extended address to be used by the 
 *          address filter.
 *
 *  @param  new_extended_address Extended address to be used by the address filter. 
 *
 *  @ingroup tat
 */
__x void tat_set_extended_address( uint8_t *extended_address ){
    
    hal_register_write( RG_IEEE_ADDR_7, *extended_address++ );
    hal_register_write( RG_IEEE_ADDR_6, *extended_address++ );
    hal_register_write( RG_IEEE_ADDR_5, *extended_address++ );
    hal_register_write( RG_IEEE_ADDR_4, *extended_address++ );
    hal_register_write( RG_IEEE_ADDR_3, *extended_address++ );
    hal_register_write( RG_IEEE_ADDR_2, *extended_address++ );
    hal_register_write( RG_IEEE_ADDR_1, *extended_address++ );
    hal_register_write( RG_IEEE_ADDR_0, *extended_address++ );
}

/** @brief  This function will configure the CSMA algorithm used by the radio 
 *          transceiver when transmitting data from TX_ARET_ON state. 
 *          
 *  @param  seed0 Lower 8 bits of the seed used for the random number generator 
 *                in the CSMA algorithm. Value range: 0 to 255. 
 *  @param  be_csma_seed1 Is a combined argument of the MIN_BE, MAX_CSMA_RETRIES
 *                        and SEED1 variables:
 *                        -# MIN_BE: Bit[7:6] Minimum back-off exponent in the 
 *                           CSMA/CA algorithm.
 *                        -# MAX_CSMA_RETRIES: Bit[5:3] Number of retries in 
 *                          TX_ARET_ON mode to repeat the CSMA/CA procedures
 *                          before the ARET procedure gives up.
 *                        -# SEED1: Bits[2:0] Higher 3 bits of CSMA_SEED, bits[10:8]
 *                           Seed for the random number generator in the 
 *                           CSMA/CA algorithm. 
 *  @return TAT_SUCCESS The CSMA algorithm was configured successfully.
 *  @return TAT_WRONG_STATE This function should not be called in the 
 *                          SLEEP state.
 *
 *  @ingroup tat
 */
tat_status_t tat_configure_csma( uint8_t seed0, uint8_t be_csma_seed1 ){
    
    /*Check state.*/
    if (is_sleeping( ) == true) { return TAT_WRONG_STATE; }
    
    /*Extract parameters, and configure the CSMA-CA algorithm.*/
    uint8_t back_off_exponent = ( be_csma_seed1 & 0xC0 ) >> 6;
    uint8_t csma_retries      = ( be_csma_seed1 & 0x38 ) >> 3;
    uint8_t seed1             = ( be_csma_seed1 & 0x07 );
            
    hal_subregister_write( SR_MAX_FRAME_RETRIES, 0 ); //AT86RF230 rev A errata.
    hal_subregister_write( SR_MAX_CSMA_RETRIES, csma_retries );
    hal_subregister_write( SR_MIN_BE, back_off_exponent );
    hal_register_write( RG_CSMA_SEED_0, seed0 );
    hal_subregister_write( SR_CSMA_SEED_1, seed1 );
            
    return TAT_SUCCESS;
}

/** @brief  This function uses the .
 *
 *  @note This function can only be executed after tat_configure_csma has been 
 *        called!
 *  @note This function can only send valid IEEE 802.15.4 Frames.
 *
 *  @param frame_length Length of frame to transmit.
 *  @param frame Pointer to the frame to transmit.
 *  @param retries Number of times to retry frame transmission (Zero means that 
 *                 the frame will be sent once.).
 *  @return TAT_SUCCESS if the frame was sent successfully within the defined 
 *                      number of retries.
 *  @return TAT_CHANNEL_ACCESS_FAILURE if the channel was found to be busy on 
 *                      the last retry.
 *  @return TAT_NO_ACK if an IEEE 802.15.4 acknowledge was not received in time.
 *  @return TAT_INVALID_ARGUMENT if the frame_length is too long.
 *  @return TAT_WRONG_STATE if the radio transceiver is not in TX_ARET_ON.
 *
 *  @ingroup tat
 */
__x tat_status_t tat_send_data_with_retry( uint8_t frame_length, uint8_t *frame, 
                                       uint8_t retries ){ 
    
    tat_status_t task_status = TAT_CHANNEL_ACCESS_FAILURE;          
    
    /*Do sanity check on function parameters and current state.*/
    if ((frame_length > RF230_MAX_TX_FRAME_LENGTH) || 
        (frame_length < TAT_MIN_IEEE_FRAME_LENGTH)) { 
        return TAT_INVALID_ARGUMENT; 
    }
    
    if (tat_get_trx_state( ) != TX_ARET_ON) { return TAT_WRONG_STATE; }
    
    hal_clear_trx_end_flag( );
    
    /*Do initial frame transmission.*/
    hal_set_slptr_high( );
    hal_set_slptr_low( );
    hal_frame_write( frame, frame_length ); //Then write data to the frame buffer.
    
    bool2 retry = false; // Variable used to control the retry loop.
    
    /*Do retry if requested.*/
    do{
        
        //Wait for TRX_END interrupt.
        while (hal_get_trx_end_flag( ) == 0) {;}
        
        //Check status.
        uint8_t transaction_status = hal_subregister_read( SR_TRAC_STATUS );
        
        //Check for failure.
        if ((transaction_status != TAT_TRANSMISSION_SUCCESS)) {
            
            if (transaction_status == TAT_BUSY_CHANNEL) {
                task_status = TAT_CHANNEL_ACCESS_FAILURE;
            } else {
                task_status = TAT_NO_ACK;
            }
            
            if ((retries--) > 0) {
                
                retry = true;
                
                //Wait for the TRX to go back to TX_ARET_ON.
                while (tat_get_trx_state() != TX_ARET_ON) {;}
            
                hal_clear_trx_end_flag( );
                hal_set_slptr_high( );
                hal_set_slptr_low( );
            } else {
                retry = false;
            }
        } else{
            
            task_status = TAT_SUCCESS;
            retry = false;            
        } // end: if ((transaction_status != TAT_TRANSMISSION_SUCCESS)) ...
    } while (retry == true);
    
    return task_status;                                       
}



/** @brief  This function will receive a frame from the radio transceiver's 
 *          recieve buffer and read it.
 *
 *  
 *  @param  *data   Pointer to the data to receive
 *
 *  @return TAT_SUCCESS Frame receive and read successfully.
 * 
 *  @return TAT_WRONG_STATE It is only possible to use this function in the 
 *                          RX_ON  state. If any other state is 
 *                          detected this error message will be returned.
 *
 *  @ingroup tat
 */
 
 /*
__x Radio_MsgPtr tat_receive_data( hal_rx_frame_t *rx_frame ){
    
   
    //To make sure if status check is needed or not
    //if ((tat_get_trx_state( ) != RX_ON)) { return TAT_WRONG_STATE; }
      
    //Wait for trx_end interupt to signal the end of receive
      while (hal_get_trx_end_flag( ) == 0) {;}
     
    // Read frame to MCU        
    hal_frame_read( rx_frame ); //Then read data to the MCU.
    result = AMStandard_RadioReceive_receive ( (Radio_MsgPtr *) rx_frame->data);
           
    return result; 
}


*/

/*EOF*/
