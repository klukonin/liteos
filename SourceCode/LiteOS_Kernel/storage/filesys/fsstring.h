/** @file fsstring.h
	 @brief This file declares several functions for string manipulations in file system. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/


#ifndef FSSTRINGH
#define FSSTRINGH

/** \addtogroup filesystem */

/**@{*/

/** @brief Extract the string. For example, /c/Temp extracts c first.  
	@param start The string to be extracted.
	@param buffer The buffer to put the string.
 	@return The next string to be extracted in the original string.
*/
char *extractString(char *start, char *buffer);

/** @brief Extract last file name in the path name parameter.
	@param pathname The path name.
	@return Extracted string. 
*/
char *extractLastName(char *pathname);

/** @brief Find the file mode.
	@param s The string.
	@return Integer for the file mode.
*/
int fileMode(char *s);

/**@}*/
#endif
