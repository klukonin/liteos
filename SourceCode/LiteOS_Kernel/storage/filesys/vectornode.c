/**  @file vectornode.c
 	 @brief 	 This file implements the operations for vector node. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/



#include "vectornode.h"
#include "fsconfig.h"
#include "fsapi.h"
#include "../bytestorage/bytestorage.h"
#include "storageconstants.h"

char vectornode[12];
void writeVectorNodeToExternalStorage()
{
    genericreadBytes(EEPROMVECTORSTART, 12, vectornode);
}

//-------------------------------------------------------------------------
void readVectorNodeFromExternalStorage()
{
    genericwriteBytes(EEPROMVECTORSTART, 12, vectornode);
}

//-------------------------------------------------------------------------
int getbit(char *set, int number)
{
    set += number / 8;
    return (*set & (1 << (number % 8))) != 0;   /* 0 or 1       */
}

//-------------------------------------------------------------------------
void setbit(char *set, int number, int value)
{
    set += number / 8;
    if (value)
    {
        *set |= 1 << (number % 8);
    }                           /* set bit      */
    else
    {
        *set &= ~(1 << (number % 8));
    }                           /* clear bit    */
}

//-------------------------------------------------------------------------
void initVectorNode()
{
    int i;

    for (i = 0; i < 12; i++)
    {
        vectornode[i] = 0;
    }
}

//-------------------------------------------------------------------------
int getVectorNode()
{
    int num;

    for (num = 0; num < 96; num++)
    {
        if (getbit(vectornode, num) == 0)
        {
            setbit(vectornode, num, 1);
            return num + 1;
        }
    }
    return -1;
}

//-------------------------------------------------------------------------
void scanVectorNode()
{
    int num, addr;
    uint8_t valid;

    for (num = 0; num < 96; num++)
    {
        addr = num + 1;
        valid = fsread8uint(addr, VALIDOFFSET);
        if (valid == 0)
        {
            setbit(vectornode, num, 0);
        }
        else
        {
            setbit(vectornode, num, 1);
        }
    }
}

//-------------------------------------------------------------------------
void releaseVectorNode(int num)
{
    setbit(vectornode, num - 1, 0);
}

//-------------------------------------------------------------------------
int countVectorNode()
{
    int num, count;

    count = 0;
    for (num = 0; num < 96; num++)
    {
        if (getbit(vectornode, num) == 1)
        {
            count++;
        }
    }
    return 96 - count;
}

