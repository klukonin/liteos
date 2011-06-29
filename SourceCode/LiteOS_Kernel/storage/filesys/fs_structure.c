/** @file fs_structure.c
	 @brief This file implements several functions for file system structure. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/



#include "fs_structure.h"
#include "inode.h"
#include "fsconfig.h"
#include "fsapi.h"

fid fidtable[ MAX_FILE_TABLE_SIZE ];

//-------------------------------------------------------------------------
int checkFsValid()
{
   if ( checkNodeValid( FSROOTNODE ) == DIRNODE )
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

//-------------------------------------------------------------------------
void initFidTable()
{
   int i;
   for ( i = 0; i < MAX_FILE_TABLE_SIZE; i ++ )
   {
      fidtable[ i ].valid = 0;
   }
}

//-------------------------------------------------------------------------
int getFreeFid()
{
   int i;
   for ( i = 0; i < MAX_FILE_TABLE_SIZE; i ++ )
   {
      if ( fidtable[ i ].valid == 0 )
      {
         break;
      }
   }
   fidtable[ i ].valid = 1;
   fidtable[ i ].index = i;
   fidtable[ i ].addr = fidtable[ i ].mode = 0;
   fidtable[ i ].fpos =  - 1;
   return i;
}

//-------------------------------------------------------------------------
void releaseFid( int fid )
{
   fidtable[ fid ].valid = 0;
}

//-------------------------------------------------------------------------
void releaseAllFid()
{
  int i;
  for ( i = 0; i < MAX_FILE_TABLE_SIZE; i ++ )
   fidtable[ i ].valid = 0;
}
