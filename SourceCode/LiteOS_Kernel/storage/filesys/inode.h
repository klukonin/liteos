/** @file inode.h
	 @brief This file declarares the file system node operations. 
	 @author Qing Charles Cao (cao@utk.edu)
*/


#ifndef INODEH
#define INODEH
#include "../../types/types.h"

/**@addtogroup filesystem 

@par
Following is the code to test this module 

@code 
#include "ioeeprom.h"
#include "inode.h"
#include "fsconfig.h"
int main()
{
int valid; 
formatFS();
buildNewNode(0, "root", 0, 0, DIRNODE);
buildNewNode(10, "abc.h", 0, 0, FILENODE);
buildNewNode(20, "test", 0, 0, DIRNODE);
buildNewNode(30, "test1", 0, 20, FILENODE);
buildNewNode(40, "test2", 0, 20, DIRNODE);
buildNewNode(50, "test21", 0, 40, FILENODE);
printNode(0);
printNode(10);
printNode(20);
printNode(30);
printNode(40);
printNode(50);
deleteNode(20);
printNode(0);
printNode(20);
deleteNode(10);
printNode(0);   
valid = checkNodeValid(3);
valid = checkNodeValid(6);
//printNode(0);
//printNode(10);
//printNode(20);
return 0;
}
@endcode
*/


/** @ingroup filesystem 
	@brief Check wehther node nodeaddr is a valid node that has been assigned.
	@param addr The address.
	@return Valid or not. 
*/
int checkNodeValid(int addr);


/** @ingroup filesystem 
	@brief Build a new node based on the type, parent, uid, name, and addr information.
	@param addr The address.
	@param name The name of the node.
	@param uid The uid of the node.
	@param parent The parent of the node.
	@param type The type of the node. 
	@return Void.
*/

void buildNewNode(int addr, char *name, uint8_t uid, uint8_t parent, uint8_t
                  type);

/** @ingroup filesystem
	@brief Delete a node.

	To detelet the complete node, even if it is a directory, will delete recursively. 
       The parent of this addr will also be modified 
       Now only handles the file system change. for device and program, the OS must change its scheduler as well.

       @param addr The address.
       @return Void.
*/       
void deleteNode(int addr);

/** @ingroup filesystem
	@brief Print a node.
	@param addr The address.
	@return Void.
*/
void printNode(int addr);

/** @ingroup filesystem
	@brief Format the file system. 
	@return Void.
*/

void formatFS();


/** @ingroup filesystem
	@brief Build the root node. 
	@return Void.
*/

void buildRootNode();

/** @ingroup filesystem
	@brief Whether an inode matches a string. 
	@param addr The address.
	@param string The string to be matched. 
	@return True or false. 
*/

int inodeMatch(uint8_t addr, char *string);

/** @ingroup filesystem
	@brief Copy a vector node. 
	@param from The source address. 
	@param to The dest address. 
	@return Void. 
*/

void copyVectorNode(int from, int to);

/** @ingroup filesystem
	@brief Copy a vector page. 
	@param from The source address. 
	@param to The dest address. 
	@return Void. 
*/

void copyVectorPage(int from, int to);

/** @ingroup filesystem
	@brief Format the file system in the lightweight way. 
	@return Void.
*/


void formatFSLite();

/**@}*/
#endif
