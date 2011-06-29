/**  @file vectorflash.h
 	 @brief 	 This file declares the operations for vector flash. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/


#ifndef VECTORFLASHH
#define VECTORFLASHH

/** @addtogroup filesystem */

/**@{ */

/** @brief Initialize the vector flash control data structure. 
	@return Void. 
*/
void initVectorFlash();

/** @brief Scan the vector flash. 
	@return Void. 
*/
void scanVectorFlash();

/** @brief Return a flash page. 
	@return The flash page id. 
*/
int getFlashPage();

/**	@brief Release a flash page. 
	@param num The page number. 
	@return Void. 
*/
void releaseFlashPage(int num);

/** @brief Count how many flash pages there are. 
	@return The counted vector flash page. 
*/
int countVectorFlash();

/** @brief Write vector flash into external storage. 
	@return Void. 
*/
void writeVectorFlashToExternalStorage();

/** @brief Read the vector flash from external storage. 
	@return Void. 
*/
void readVectorFlashFromExternalStorage();

/**@} */
#endif
