/**  @file stdfsa.c
 	 @brief 	 This file implements as an middle layer between the file system API provided to the user and and low level operations.
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/




#include "stdfsa.h"
#include "fsconfig.h"
#include "inode.h"
#include "vectornode.h"
#include "fsstring.h"
#include "vectorflash.h"
#include "fsapi.h"
#include "../../types/string.h"
#include "../bytestorage/bytestorage.h"

static int currentdirectory;
extern fid fidtable[MAX_FILE_TABLE_SIZE];

//check whether the addr block has a file name as pointed to by filename
//filename should be no more than 12 bytes and must end with \0
//the checking goes as follows. It checks the bytes by bytes and make sure that 
//the string mathces. if not match then 1 if \0 and match then 0 otherwise proceeds 
//and the filename must be valid 
int checkName(char *filename, int addr)
{
    char *p;
    int i;

    i = 0;
    if (mystrlen(filename) > 12)
    {
        return 1;
    }
    if (checkNodeValid(addr) == 0)
    {
        return 1;
    }
    for (i = 0; i < 12; i++)
    {
        p = filename;
        p += i;
        if ((uint8_t) (*p) != fsread8uint(addr, FILENAMEOFFSET + i))
        {
            return 1;
        }
        else
        {
            if ((*p) == '\0')
            {
                return 0;
            }
            else
            {
                continue;
            }
        }
    }
    return 0;
}

//if the directory is full return 1 else return 0
uint8_t fullBlock(int directory)
{
    int i;

    for (i = 0; i < 10; i++)
    {
        uint8_t subaddr;

        subaddr = fsread8uint(directory, DIR_ADDRSUBOFFSET + i);
        if (subaddr == 0)
        {
            return 0;
        }
    }
    return 1;
}

//-------------------------------------------------------------------------
void getName(char *buffer, int addr)
{
    int i;
    char *p;

    p = buffer;
    for (i = 0; i < 12; i++)
    {
        *p = fsread8uint(addr, FILENAMEOFFSET + i);
        if ((*p) == '\0')
        {
            break;
        }

        p++;
    }
    *p = '\0';
    return;
}

//in this one, the directory is checked to see if the file is out there if not return -1 
int changeDirectory(char *filename, int directory)
{
    int i;

    for (i = 0; i < 10; i++)
    {
        uint8_t subaddr;

        subaddr = fsread8uint(directory, DIR_ADDRSUBOFFSET + i);
        if (checkName(filename, subaddr) == 0)
        {
            return subaddr;
        }
    }
    return -1;
}

//-------------------------------------------------------------------------
int getParentDirectory(int directory)
{
    uint8_t parentnode;

    parentnode = fsread8uint(directory, DIR_PARENTOFFSET);
    return parentnode;
}

//check wehther a block exists. the filename must be single level 
int existBlock(char *filename, int directory)
{
    int i;
    uint8_t temp;

    for (i = 0; i < 10; i++)
    {
        uint8_t subaddr;

        subaddr = fsread8uint(directory, DIR_ADDRSUBOFFSET + i);
        if (checkName(filename, subaddr) == 0)
        {
            if (((temp = checkNodeValid(subaddr)) > 0) && (subaddr > 0))
            {
                return temp;
            }
            else
            {
                continue;
            }
        }
    }
    return 0;
}

//check wehther a block exists. the filename must be single level 
int existBlockAddr(char *filename, int directory)
{
    int i;
    uint8_t temp;

    for (i = 0; i < 10; i++)
    {
        uint8_t subaddr;

        subaddr = fsread8uint(directory, DIR_ADDRSUBOFFSET + i);
        if (checkName(filename, subaddr) == 0)
        {
            if (((temp = checkNodeValid(subaddr)) > 0) && (subaddr > 0))
            {
                return subaddr;
            }
            else
            {
                continue;
            }
        }
    }
    return 0;
}

//return current directory
int getPwd()
{
    return currentdirectory;
}

//set current directory 
void setPwd(int directory)
{
    currentdirectory = directory;
    return;
}

//check directory empty
//returns 0 if it is empty. Returns 1 if otherwise 
int emptyDirectory(int directory)
{
    int i;

    for (i = 0; i < 10; i++)
    {
        uint8_t subaddr;

        subaddr = fsread8uint(directory, DIR_ADDRSUBOFFSET + i);
        if (subaddr > 0)
        {
            return 1;
        }
    }
    return 0;
}

//create a directory
uint8_t createDir(char *filename, int directory)
{
    //fix: should first try to see if there is a directory with the same name
    uint8_t getaddr;

    if (existBlockAddr(filename, directory) > 0)
    {
        return 0;
    }
    if (fullBlock(directory) == 1)
    {
        return 255;
    }
    //this part first inserts a directory and then creates a node 
    getaddr = (uint8_t) getVectorNode();
    buildNewNode(getaddr, filename, 0, (uint8_t) directory, DIRNODE);
    return getaddr;
}

//void foobar()
//{}
//create a file
uint8_t createFileFromDirectory(char *filename, int directory)
{
    //this part first inserts a directory and then creates a node 
    uint8_t getaddr;

    if (existBlockAddr(filename, directory) > 0)
    {
        return 0;
    }
    if (fullBlock(directory) == 1)
    {
        return 255;
    }
    getaddr = (uint8_t) getVectorNode();
    buildNewNode(getaddr, filename, 0, (uint8_t) directory, FILENODE);
    return getaddr;
}

//check if the addr block is a directory or not 
int isDirectory(int addr)
{
    if (checkNodeValid(addr) == DIRNODE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-1 NO SUCH FILE, and the parent direcotry has some problem 
//returns addr of the file and for state
//FILENODE, DIRECTORYNODE, BLAH, BLAH 1, 2, 3, 4
//0, THERE is no such file, but it returns the parent of the path , which does exist
//this function uses the curren tdicreotry if needed
//the pathname is organized as ../ or ./ or name/ or /something and does not end with / 
//sovled
//and the following are the functions this thing uses
//
int locateFileName(char *pathname, int *state)
{
    char p, q;
    char *relativestart;
    int addrTrack;
    char nextString[13];
    int ret;

    //first classify what type of pathname this is 
    p = pathname[0];
    q = pathname[1];
    relativestart = pathname;
    addrTrack = getPwd();
    if ((p == '.') && (q == '.'))
    {
        addrTrack = getParentDirectory(addrTrack);
        relativestart = pathname + 2;
    }
    else if ((p == '.') && (q == '/'))
    {
        relativestart = pathname + 1;
    }
    else if (p == '/')
    {
        addrTrack = FSROOTNODE;
        relativestart = pathname;
    }
    else if (isLetter(p) == 1)
    {
        //this case is the "mnae" case, where there may or may not be further stuff behind 
        //buggy place 
        relativestart = extractString(relativestart, (char *)nextString);
        if (relativestart == '\0')
        {
            if ((ret = existBlock(nextString, addrTrack)) == 0)
            {
                //ok there is only one string but no further strings after that. And this string does not exist 
                *state = 0;
                return addrTrack;
            }
            else
            {
                *state = ret;
                ret = changeDirectory(nextString, addrTrack);
                return ret;
            }
        }
        else
        {
            if ((ret = existBlock(nextString, addrTrack)) == 0)
            {
                //ok there is only one string but further strings after that. And this string does not exist    
                return -1;
            }
            else
            {
                addrTrack = changeDirectory(nextString, addrTrack);
            }
        }
    }
    while (1)
    {
        //Now all cases have been uniform now. It is like /a/b/c type 
        if (isDirectory(addrTrack) == 0)
        {
            return -1;
        }
        relativestart = extractString(relativestart, nextString);
        if (*relativestart == '\0')
        {
            if ((ret = existBlock(nextString, addrTrack)) == 0)
            {
                //ok there is only one string but no further strings after that. And this string does not exist 
                *state = 0;
                return addrTrack;
            }
            else
            {
                *state = ret;
                ret = changeDirectory(nextString, addrTrack);
                return ret;
            }
        }
        else
        {
            if ((ret = existBlock(nextString, addrTrack)) == 0)
            {
                //ok there is only one string but further strings after that. And this string does not exist    
                return -1;
            }
            else
            {
                addrTrack = changeDirectory(nextString, addrTrack);
            }
        }
    }
}

//-------------------------------------------------------------------------
void freeBlocks(int addr)
{
    int i;
    uint8_t readpage;

    for (i = 0; i < 8; i++)
    {
        readpage = fsread8uint(addr, FILE_ADDRPAGEOFFSET + i);
        if (readpage == 0)
        {
            break;
        }
        else
        {
            releaseFlashPage(readpage);
        }
    }
    return;
}

//open a file according to the mode and populate the fid section 
void openFile(int addr, int fid, int mode)
{
    fidtable[fid].addr = (uint8_t) addr;
    fidtable[fid].mode = (uint8_t) mode;
    fidtable[fid].size = fsread16uint(addr, 29);
    //mode: 1 read 2 write 3 append 4 truncate 5 rw
    if (mode == 1)
    {
        fidtable[fid].fpos = 0;
    }
    if (mode == 2)
    {
        fidtable[fid].fpos = 0;
    }
    if (mode == 3)
    {
        fidtable[fid].fpos = fidtable[fid].size;
    }
    if (mode == 4)
    {
        freeBlocks(addr);
        fidtable[fid].fpos = 0;
    }
    if (mode == 5)
    {
        fidtable[fid].fpos = 0;
    }
}

//-------------------------------------------------------------------------
uint8_t getRealSector(uint8_t addr, uint8_t sectornum)
{
    uint8_t currentaddr;

    currentaddr = addr;
    while (sectornum >= 8)
    {
        currentaddr = fsread8uint(currentaddr, FILE_NEXTOFFSET);
        sectornum -= 8;
    }
    return fsread8uint(currentaddr, FILE_ADDRPAGEOFFSET + sectornum);
}

//-------------------------------------------------------------------------
void newSector(int addr)
{
    uint8_t i, readpage;
    uint8_t next, currentaddr, getnode;

    currentaddr = addr;
    //begin rounds of 
    // 1 check whether the current 8 pages are occupied or not
    // 2 check the next field is ok or not
    // 3 get to the next addr and loop 
    while (1)
    {
        for (i = 0; i < 8; i++)
        {
            readpage = fsread8uint(currentaddr, FILE_ADDRPAGEOFFSET + i);
            if (readpage == 0)
            {
                break;
            }
        }
        if (i < 8)
        {
            readpage = getFlashPage();
            fswrite8uint(currentaddr, FILE_ADDRPAGEOFFSET + i, readpage);
            return;
        }
        next = fsread8uint(currentaddr, FILE_NEXTOFFSET);
        if (next == 0)
        {
            getnode = getVectorNode();
            fswrite8uint(currentaddr, FILE_NEXTOFFSET, getnode);
            currentaddr = getnode;
            readpage = getFlashPage();
            fswrite8uint(currentaddr, FILE_ADDRPAGEOFFSET, readpage);
            return;
        }
        currentaddr = next;
    }
}

//-------------------------------------------------------------------------
void addChildNode(uint8_t addr, uint8_t child)
{
    uint8_t i;

    for (i = 0; i < 10; i++)
    {
        uint8_t subaddr;

        subaddr = fsread8uint(addr, DIR_ADDRSUBOFFSET + i);
        if (subaddr == 0)
        {
            fswrite8uint(addr, DIR_ADDRSUBOFFSET + i, child);
            return;
        }
    }
    return;
}

//-------------------------------------------------------------------------
void removeChildNode(uint8_t addr, uint8_t child)
{
    uint8_t i;

    for (i = 0; i < 10; i++)
    {
        uint8_t subaddr;

        subaddr = fsread8uint(addr, DIR_ADDRSUBOFFSET + i);
        if (subaddr == child)
        {
            fswrite8uint(addr, DIR_ADDRSUBOFFSET + i, 0);
            return;
        }
    }
    return;
}
