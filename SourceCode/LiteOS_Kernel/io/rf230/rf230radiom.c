/** @file rf230radiom.c
	@brief The implementation for rf230 radio. 
*/

#include "rf230radiom.h"
#include "compiler.h"
#include "tat.h"
#include "hal.h"
#include "hal_avr.h"
#include "at86rf230_registermap.h"

#include "../../io/serial/stdserial.h"
#include "../../types/types.h"
#include "../../types/byteorder.h"
#include "../../sensors/leds.h"


static uint16_t rf230radiom_currentDSN;

//--------------------------------------------------------
/** @brief IRIS: This function is used to initialize the TRX.
 *
 *  @return true if the TRX was successfully configured.
 *  @return false if the TRX was not configured properly.
 */
inline bool2 trx_init( void ){
    
    static bool2 status;
    
    if (tat_init( ) != TAT_SUCCESS) {
        //Leds_yellowToggle();  //Debug--Redtoggle Upon
        status = false;
    } else if (tat_set_operating_channel( 21 ) != TAT_SUCCESS) {
        status = false;
    } else if (tat_set_clock_speed( true, CLKM_DISABLED ) != TAT_SUCCESS) {
        status = false; 
    } else{
        tat_use_auto_tx_crc( true ); //Automatic CRC must be enabled.
        status = true;
    } // end: if (tat_init( ) != TAT_SUCCESS) ...

    rf230radiom_currentDSN = 0;
    return status;
}    


/** @brief Send the message. 
 *	@param pMsg The message to be sent. 
 *  @return true if the send is successful. 
 *  @return false if the send is not successful. 
 */

result_t rf230radio_Send_send(Radio_MsgPtr pMsg)
{
    unsigned char result;
	int i =0; 
	  

  	// format the 802.15.4 packet
	
    pMsg->fcflo=0x08;
    pMsg->fcfhi=0x01;
    pMsg->dsn = rf230radiom_currentDSN ++; //seq ++
    pMsg->destpan=BCAST_ADDRESS;

    //pMsg->addr==BCAST_ADDRESS;

    pMsg->addr = toLSB16 (pMsg->addr);
   // pMsg->port=0x3377;  //specify the port number to 0x0077
    
    if ( pMsg->dsn==255) pMsg->dsn=0;
      pMsg->time=0;
    
     
    pMsg->length=pMsg->length+MSG_HEADER_SIZE; //Set the length like in cc2420radiom.c, check if bug happens
    
    
     
    
     
   // just send out payload data first, add 15.4 header later
   
   /*Ensure that it is possible to enter PLL_ON*/
      // tat_reset_trx();
      
      //tat_reset_state_machine();
   if (tat_set_trx_state( PLL_ON ) == TAT_SUCCESS) {
       
   result= tat_send_data((pMsg->length+2), ((uint8_t*) pMsg)+1); //Note: Include MSG_FOOTER_SIZE
   
     if (result == TAT_SUCCESS)
   	  {  result=SUCCESS;
       if( tat_get_trx_state( )!=PLL_ON) {
       	 
         if(tat_set_trx_state( PLL_ON )!=TAT_SUCCESS) 
         	{
         		result= FAIL;
         		return result;
         	} 	     
      }
       else {
       	}
      
        //delay_us(1000);
        
        for (i=0;i++;i<100000)
        {
        asm volatile("nop"::); 
        }
	
        
       // Option-Go to RX_ON after Send-Important!
   	  	if (tat_set_trx_state( RX_ON )==TAT_SUCCESS) printfstr ("\n after send go to RX_ON sucessfully "); 
   	  	else {
   	  		result=FAIL;
   	  	}
   	  }
     else {
     	result=FAIL;   
    }
   	}
   else {
   	result=FAIL; 
  }
	
	return result; 
}

