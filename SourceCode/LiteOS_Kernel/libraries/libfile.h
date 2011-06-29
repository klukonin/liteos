/** @file libfile.h
       @brief The functional prototypes of the file system API. 

       @author Qing Charles Cao (cao@utk.edu)
       
*/


#ifndef LIBFILEH
#define LIBFILEH

#include "liteoscommon.h"


/** @addtogroup api
*/


/** @name This module implements reading/writing files for the motes.  Testing code:

       @code
      
	#include "leds.h"
	#include "libthread.h"
	#include "adc.h"
	#include "radio.h"
	#include "Serial.h"
	#include "File.h"


	uint8_t receivedata[20]; 
	uint8_t receiveserial[7]; 
	uint8_t temp;
	uint16_t i; 
	uint16_t j; 

	MYFILE *fp; 
	int main()
	{
	 __asm__ __volatile__("sei" ::); 

	 for (i=0;i<20;i++)
 		 receivedata[i] = i*3; 

	  serialSend_string("Now testing file system\n\r");
	  serialSend_string("Open file abc\n\r");
	  fp =   mfopen("/abc", "w");
	  
	  for (i=0; i<10; i++)
	 {
	  mfwrite(fp , receivedata, 20); 
	  mfseek(fp, 20, 1);
	 }

	 mfclose(fp); 


 	 serialSend_string("File created\n\r");

	 serialReceive(3, 6, receiveserial); 

	 //serialSend_string("Command received\n\r");

	  fp =   mfopen("/abc", "r");
	  
	  for (i=0; i<10; i++)
	 {
	  mfread(fp , receivedata, 20);

	  
	  serialSend_string("\nNow send a 20 read\n\r");

	//	  serialSend(20, receivedata); 

	
	  mfseek(fp, 20, 1);
	
	 }
	
	 mfclose(fp); 

 
	return 0; 
	}

	@endcode
*/

/** @{
*/




/** @brief 	Open a file based on its parameters and for later operations. 
  	@param pathname The path name of the file to be opened. 
  	@param mode The mode of the file to be opened.
       @return MYFILE The pointer to the file to be opened.
*/

LIB_MYFILE *lib_mfopen(const char *pathname, const char *mode);

/** @brief   Close a file based on the file pointer.
       @param fp The file pointer to the previously opened file. 
       @return Void.
*/
void lib_mfclose(LIB_MYFILE *fp); 

/**@brief   Read data from a file.
      @param fp The pointer of the previously opened file.
      @param buffer The buffer for the read data.
      @param nBytes The number of bytes to be read.
      @return Void.
*/
void lib_mfread(LIB_MYFILE *fp, void *buffer, int nBytes);

/** @brief  Write to a file. 
       @param fp The pointer of the previously opened file. 
       @param buffer The buffer for the data to be written. 
       @param nBytes The number of bytes to be written. 
       @return Void. 
*/
void lib_mfwrite(LIB_MYFILE *fp, void *buffer, int nBytes);

/** @brief   Seek the file for repositioning the file pointer.
       @param fp The pointer of the previously opened file. 
       @param offset The offset of the seek operation.
       @param position The position of the seek operation. 
       @return Void.
*/

void lib_mfseek(LIB_MYFILE *fp, int offset, int position);

/** @brief Write to a file without specifying the length.
       @param fp The pointer of the previously opened file. 
       @param buffer The start position for the write buffer. 
       @return Void.
*/

void lib_mfwrite_withoutlength(LIB_MYFILE *fp, void *buffer);

/** @}*/

#endif 
