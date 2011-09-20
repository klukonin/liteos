/* The LiteOS Operating System Kernel */
/*
   The following is the license of LiteOS.
   This file is part of LiteOS.
   Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu
   LiteOS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   LiteOS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
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
/*! \brief IRIS: This function is used to initialize the TRX.
 *
 *  \retval true if the TRX was successfully configured.
 *  \retval false if the TRX was not configured properly.
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
       // Leds_redToggle();  //Debug--Redtoggle Upon
        tat_use_auto_tx_crc( true ); //Automatic CRC must be enabled.
       // hal_set_trx_end_event_handler( trx_end_handler ); // Event handler for TRX_END events.
       //Debug-Init() return success (true) from here
       // Leds_redToggle();  //Debug--Redtoggle Upon
        status = true;
    } // end: if (tat_init( ) != TAT_SUCCESS) ...

    rf230radiom_currentDSN = 0;
    return status;
}    




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
    
  /*  //Debug  
     printfstr ("\n-------------------------- ");
     printfstr ("\n header length: "); 
     printfintegeru32((uint32_t)MSG_HEADER_SIZE);
     printfstr ("\n payload length: "); 
     printfintegeru32((uint32_t)pMsg->length);
     printfstr ("\n footer length: "); 
     printfintegeru32((uint32_t)MSG_FOOTER_SIZE);*/
     
    pMsg->length=pMsg->length+MSG_HEADER_SIZE; //Set the length like in cc2420radiom.c, check if bug happens
    
    
     
    
     
   // just send out payload data first, add 15.4 header later
   
   /*Ensure that it is possible to enter PLL_ON*/
      // tat_reset_trx();
      
      //tat_reset_state_machine();
     //if(tat_get_trx_state()==TRX_OFF) Leds_yellowToggle();  //Debug--Redtoggle Upon
   if (tat_set_trx_state( PLL_ON ) == TAT_SUCCESS) {
       
     //	Leds_yellowToggle();  //Debug--yellowtoggle Upon
   result= tat_send_data((pMsg->length+2), ((uint8_t*) pMsg)+1); //Note: Include MSG_FOOTER_SIZE
   
     if (result == TAT_SUCCESS)
   	  {  result=SUCCESS;
	  //debugrf230
        // Leds_yellowToggle();  //Debug--Redtoggle Upon tat_send-Blinking here!
 
      //qing revise
   	  	 printfstr ("\n *SEND SUCCUESS* \n!");
   	  	//printfstr ("\n *radio_status after send"); 
        //printfintegeru32((uint32_t) tat_get_trx_state( ));
       if( tat_get_trx_state( )!=PLL_ON) {
       	 
       	 //printfstr ("\n *radio_status after send is not PLL_ON");
         if(tat_set_trx_state( PLL_ON )!=TAT_SUCCESS) 
         	{
         		//qing revise
         		
         	printfstr ("\n *radio_status after send set to PLL_ON Failed, Return!");
         		result= FAIL;
         	//	Leds_yellowToggle();  //Debug--Upon sending out packets failure
         		return result;
         	} 	     
      }
       else {
     //  	Leds_yellowToggle();  //Debug--Upon sending out packets finish
        //qing revise
       	printfstr ("\n *radio_status after send is already PLL_ON!");
       	}
      
        //delay_us(1000);
        
        for (i=0;i++;i<100000)
        {
        asm volatile("nop"::); 
        }
	
        
       // Option-Go to RX_ON after Send-Important!
   	  	if (tat_set_trx_state( RX_ON )==TAT_SUCCESS) printfstr ("\n after send go to RX_ON sucessfully "); 
   	  	else {
   	  	//	printfstr ("\n after send go to RX_ON fail "); 
   	  		result=FAIL;
   	  	}
   	  }
     else {
     // Leds_yellowToggle();  //Debug--Upon sending out packets failure
     	result=FAIL;   
    }
   	}
   else {
   //Leds_redToggle();  //Debug--Redtoggle Upon
   	result=FAIL; 
  }
	
	return result; 
}

