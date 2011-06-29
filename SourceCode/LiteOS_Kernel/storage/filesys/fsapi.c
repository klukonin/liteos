/** @file fsapi.c
	@brief This file implements the functions for the global level file system API. 
	
	@author Qing Charles Cao (cao@utk.edu)
*/


#include "stdfsa.h"
#include "fs_structure.h"
#include "vectornode.h"
#include "vectorflash.h"
#include "fsstring.h"
#include "fsconfig.h"
#include "inode.h"
#include "fsapi.h"
#include "../flash/pagestorage.h"
#include "../bytestorage/bytestorage.h"
#include "../../types/types.h"
#include "../../types/string.h"


extern fid fidtable[MAX_FILE_TABLE_SIZE];

//-------------------------------------------------------------------------
MYFILE *fsopen(char *pathname, char *mode)
{
    int retaddr, state;
    int openmode;

    retaddr = locateFileName(pathname, &state);
    openmode = fileMode(mode);
    //ERROR in open mode 
    if (openmode == 0)
    {
        return NULL;
    }
    //If the file location fails, return -1 
    if (retaddr == -1)
    {
        return NULL;
    }
    //first, file does not exist
    if (state == 0)
    {
        if (openmode == 2)
        {
            int blockaddr;
            int fid;
            char *temp;

            temp = extractLastName(pathname);
            blockaddr = createFileFromDirectory(temp, retaddr);
            fid = getFreeFid();
            openFile(blockaddr, fid, openmode);
            return &fidtable[fid];
        }
        else
        {
            return NULL;
        }
    }
    if (state != FILENODE)
    {
        return NULL;
    }
    {
        int fid;

        fid = getFreeFid();
        openFile(retaddr, fid, openmode);
        return &fidtable[fid];
    }
}

//-------------------------------------------------------------------------
void fclose2(MYFILE * fp)
{
    releaseFid(fp->index);
    fp = NULL;
    return;
}

//-------------------------------------------------------------------------
int fseek2(MYFILE * fp, int32_t offset, int position)
{
    int temp;

    if (position == 0)
    {
        if (offset > fp->size)
        {
            return -1;
        }
        fp->fpos = offset;
        return 0;
    }
    temp = fp->fpos;
    if (position == 1)
    {
        fp->fpos = fp->fpos + offset;
    }
    if ((fp->fpos < 0) || (fp->fpos > fp->size))
    {
        fp->fpos = temp;
        return -1;
    }
    if (position == 2)
    {
        fp->fpos = fp->size + offset;
    }
    if ((fp->fpos < 0) || (fp->fpos > fp->size))
    {
        fp->fpos = temp;
        return -1;
    }
    return 0;
}

//-------------------------------------------------------------------------
int fexist2(char *pathname)
{
    int retaddr, state;

    retaddr = locateFileName(pathname, &state);
    if ((retaddr == -1) || (state == 0))
    {
        return -1;
    }
    else
    {
        return retaddr;
    }
}

//-------------------------------------------------------------------------
int fcreatedir2(char *pathname)
{
    int retaddr, state;

    retaddr = locateFileName(pathname, &state);
    if (retaddr == -1)
    {
        return -1;
    }
    //first, file does not exist
    if (state == 0)
    {
        createDir(extractLastName(pathname), retaddr);
        return 0;
    }
    return -1;
}

//-------------------------------------------------------------------------
int fdelete2(char *pathname)
{
    int ret;

    if ((ret = fexist2(pathname)) == -1)
    {
        return -1;
    }
    else
    {
        deleteNode(ret);
        return 0;
    }
}

//-------------------------------------------------------------------------
int fcheckFSAllocation()
{
    return countVectorNode();
}

//-------------------------------------------------------------------------
int fcheckStorageAllocation()
{
    return countVectorFlash();
}


//-------------------------------------------------------------------------
//vs patch used for debugging only 
void fsprintall()
{
    int i;

    //  printf("*****************************************************************\n");
    for (i = 0; i <= 96; i++)
    {
        if (checkNodeValid(i) > 0)
        {
            printNode(i);
        }
    }
}

//-------------------------------------------------------------------------
int fread2(MYFILE * fp, void *buffer, int nBytes)
{
    int startsector;
    int endsector;
    uint8_t realsector;
    int readbytes;
    int pagenum, blockoffset, pageoffset;

    if (fp->fpos + nBytes > fp->size)
    {
        return -1;
    }
    startsector = fp->fpos / 2048;
    endsector = (fp->fpos + nBytes - 1) / 2048;
    if (startsector == endsector)
    {
        blockoffset = fp->fpos % 2048;
        pageoffset = blockoffset % 256;
        pagenum = blockoffset / 256;
        //realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
        //  realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
        realsector = getRealSector(fp->addr, startsector);
        pagenum = pagenum + (realsector - 1) * 8;
        //now pagenum, offset2 means the actual start location just read it
        readpagestorage(pagenum, pageoffset, buffer, nBytes);
    }
    else
    {
        blockoffset = fp->fpos % 2048;
        pageoffset = blockoffset % 256;
        pagenum = blockoffset / 256;
        //      realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
        //realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
        realsector = getRealSector(fp->addr, startsector);
        pagenum = pagenum + (realsector - 1) * 8;
        readbytes = 256 - pageoffset;
        //now pagenum, offset2 means the actual start location just read it
        readpagestorage(pagenum, pageoffset, buffer, readbytes);
        buffer = (void *)((char *)buffer + readbytes);
        blockoffset = (fp->fpos + nBytes - 1) % 2048;
        pageoffset = blockoffset % 256;
        pagenum = blockoffset / 256;
        //realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+endsector);
        //realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
        realsector = getRealSector(fp->addr, endsector);
        pagenum = pagenum + (realsector - 1) * 8;
        readbytes = nBytes - readbytes;
        //now pagenum, offset2 means the actual start location just read it
        readpagestorage(pagenum, 0, buffer, readbytes);
    }
    return 0;
}

//-------------------------------------------------------------------------
int fwrite2(MYFILE * fp, void *buffer, int nBytes)
{
    int startsector;
    int endsector;
    uint8_t realsector;
    int readbytes;
    int pagenum, blockoffset, pageoffset;

    //first it checks whether there is enough space for the writing to take place, then it does the actual writing in the same way as above 
    if (fp->fpos + nBytes <= fp->size)
    {
        //there is no need to change the size or allocate more space for the write
    }
    else
    {
        uint16_t allocate;

        allocate = (fp->size / 2048) * 2048;
        if (fp->size % 2048 > 0)
        {
            allocate += 2048;
        }
        //     if ((allocate == 32768)&&(fp->fpos + nBytes > 32768))
        //         foobar(); 
        if (fp->fpos + nBytes > allocate)
        {
            newSector(fp->addr);
        }

        if (fp->fpos + nBytes > fp->size)
        {
            fp->size = fp->fpos + nBytes;
            fswrite16uint(fp->addr, FILE_SIZEOFFSET, fp->size);
        }
    }
    
    //avoid flash overflow
    if (fp->size > 64000)
    	return 2; 
    	 
    startsector = fp->fpos / 2048;
    endsector = (fp->fpos + nBytes - 1) / 2048;
    if (startsector == endsector)
    {
        blockoffset = fp->fpos % 2048;
        pageoffset = blockoffset % 256;
        pagenum = blockoffset / 256;
        //      realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
        //realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
        realsector = getRealSector(fp->addr, startsector);
        pagenum = pagenum + (realsector - 1) * 8;
        //now pagenum, offset2 means the actual start location just read it
        writepagestorage(pagenum, pageoffset, buffer, nBytes);
    }
    else
    {
        blockoffset = fp->fpos % 2048;
        pageoffset = blockoffset % 256;
        pagenum = blockoffset / 256;
        //realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
        //realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
        realsector = getRealSector(fp->addr, startsector);
        pagenum = pagenum + (realsector - 1) * 8;
        readbytes = 256 - pageoffset;
        //now pagenum, offset2 means the actual start location just read it
        writepagestorage(pagenum, pageoffset, buffer, readbytes);
        buffer = (void *)((char *)buffer + readbytes);
        blockoffset = (fp->fpos + nBytes - 1) % 2048;
        pageoffset = blockoffset % 256;
        pagenum = blockoffset / 256;
        //realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+endsector);
        //realsector = read8uint(fp->addr, FILE_ADDRPAGEOFFSET+startsector);
        realsector = getRealSector(fp->addr, endsector);
        pagenum = pagenum + (realsector - 1) * 8;
        readbytes = nBytes - readbytes;
        //now pagenum, offset2 means the actual start location just read it
        writepagestorage(pagenum, 0, buffer, readbytes);
    }
    return 0;
}

//-------------------------------------------------------------------------
int fmove(char *source, char *target)
{
    //if ((ret = fexist(pathname))==-1)
    int ret1, ret2;
    int state1, state2;
    uint8_t parent;
    char *p, *q;
    uint8_t namelength;

    p = extractLastName(target);
    q = target + mystrlen(target);
    namelength = q - p;
    ret1 = locateFileName(source, &state1);
    parent = fsread8uint(ret1, FILE_PARENTOFFSET);
    removeChildNode(parent, ret1);
    ret2 = locateFileName(target, &state2);
    fswrite8uint(ret1, FILE_PARENTOFFSET, ret2);
    fswriteBytes(ret1, FILENAMEOFFSET, namelength, p);
    fswrite8uint(ret1, FILENAMEOFFSET + namelength, 0);
    addChildNode(ret2, ret1);
    return 0;
}

//-------------------------------------------------------------------------
int fcopy(char *source, char *target)
{
    //if ((ret = fexist(pathname))==-1)
    int ret1, ret2;
    int state1, state2;
    uint8_t i;
    char *p;
    char *q;
    int NewNode;
    uint8_t namelength;

    p = extractLastName(target);
    q = target + mystrlen(target);
    namelength = q - p;
    ret1 = locateFileName(source, &state1);
    ret2 = locateFileName(target, &state2);
    NewNode = getVectorNode();
    copyVectorNode(ret1, NewNode);
    fswriteBytes(NewNode, FILENAMEOFFSET, namelength, p);
    fswrite8uint(NewNode, FILENAMEOFFSET + namelength, 0);
    fswrite8uint(NewNode, FILE_PARENTOFFSET, ret2);
    addChildNode(ret2, NewNode);
    for (i = 0; i < 8; i++)
    {
        uint8_t temp, temp1;

        temp = fsread8uint(ret1, FILE_ADDRPAGEOFFSET + i);
        if (temp > 0)
        {
            temp1 = getFlashPage();
            fswrite8uint(NewNode, FILE_ADDRPAGEOFFSET + i, temp1);
            copyVectorPage(temp, temp1);
        }
    }
    return 0;
}

//-------------------------------------------------------------------------
void fchangedir(char *path)
{
    int ret, state;

    ret = locateFileName(path, &state);
    setPwd(ret);
    return;
}

//-------------------------------------------------------------------------
void fcurrentdir(char *buffer, int size)
{
    int currentP;
    char *p;
    int len;
    char namebuffer[13];
    int i;
    char *q;

    p = buffer + size - 1;
    *p = '\0';
    p--;
    currentP = getPwd();
    while (1)
    {
        getName(namebuffer, currentP);
        len = mystrlen(namebuffer);
        mystrncpy(p - len, namebuffer, len);
        *p = '/';
        p = p - len - 1;
        currentP = fsread8uint(currentP, 31);
        if (currentP == 0)
        {
            break;
        }
    }
    *p = '/';
    len = buffer + size - 1 - p;
    q = buffer;
    for (i = 0; i < len; i++)
    {
        *q++ = *p++;
    }

    q--;
    *q = '\0';
    return;
}

//-------------------------------------------------------------------------
void finfonode(char *buffer, int addr)
{
    fsreadBytes(addr, FILENAMEOFFSET, 12, buffer);
    buffer[12] = fsread8uint(addr, TYPEOFFSET);
    buffer[13] = fsread8uint(addr, 29);
    buffer[14] = fsread8uint(addr, 30);
}

//-------------------------------------------------------------------------
void fsearch(uint8_t * addrlist, uint8_t * size, char *string)
{
    uint8_t i;
    uint8_t *p;

    p = addrlist;
    *size = 0;
    for (i = 1; i <= INODENUM; i++)
    {
        if (inodeMatch(i, string) == 1)
        {
            p[*size] = i;
            (*size)++;
        }
    }
    return;
}

//-------------------------------------------------------------------------
void formatSystem()
{
    formatFS();
}

//-------------------------------------------------------------------------
void fdirnode(char *buffer, int size, int addr)
{
    int currentP;
    char *p;
    int len;
    char namebuffer[13];
    int i;
    char *q;

    p = buffer + size - 1;
    *p = '\0';
    p--;
    currentP = addr;
    while (1)
    {
        getName(namebuffer, currentP);
        len = mystrlen(namebuffer);
        mystrncpy(p - len, namebuffer, len);
        *p = '/';
        p = p - len - 1;
        currentP = fsread8uint(currentP, 31);
        if (currentP == 0)
        {
            break;
        }
    }
    *p = '/';
    len = buffer + size - 1 - p;
    q = buffer;
    for (i = 0; i < len; i++)
    {
        *q++ = *p++;
    }

    q--;
    *q = '\0';
    return;
}
