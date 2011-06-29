/**  @file vectornode.h
 	 @brief 	 This file declares the operations for vector node. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/


#ifndef VECTORNODEH
#define VECTORNODEH

/** @addtogroup filesystem */

/**@{ */

/** @brief Init the vector node. 
	@return Void.
*/
void initVectorNode();

/** @brief Get the vector node. 
	@return The vector node id. 
*/
int getVectorNode();

/** @brief Scan the vector nodes.
	@return Void.
*/
void scanVectorNode();

/** @brief Release vector node.
	@param num The number of vector node. 
	@return Void. 
*/
void releaseVectorNode(int num);

/**	@brief Count how many vector nodes there are. 
	@return The number of vector nodes. 
*/
int countVectorNode();

/** @brief Write vector node into external storage. 
	@return Void.
*/
void writeVectorNodeToExternalStorage();

/** @brief Read vector node from external storage. 
	@return Void. 
*/
void readVectorNodeFromExternalStorage();

/** @}*/
#endif
