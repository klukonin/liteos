/** @file inode.c
	 @brief This file implements the file system node operations. 
	 @author Qing Charles Cao (cao@utk.edu)
*/

#include "inode.h"
#include "../../types/types.h"
#include "fsconfig.h"
#include "../../types/string.h"
#include "vectornode.h"
#include "vectorflash.h"
#include "../flash/pagestorage.h"





int checkNodeValid(int nodeaddr)
{
    uint8_t validness;
    uint8_t nodetype;

    validness = fsread8uint(nodeaddr, VALIDOFFSET);
    nodetype = fsread8uint(nodeaddr, TYPEOFFSET);
    if (validness == 0)
    {
        return 0;
    }
    else
    {
        return (int)nodetype;
    }
}

//-------------------------------------------------------------------------
void buildNewNode(int addr, char *name, uint8_t uid, uint8_t parent, uint8_t
                  type)
{
    uint8_t namelength;

    namelength = mystrlen(name);
    if (namelength < 12)
    {
        namelength++;
    }
    //consider several types of type
    if (type == DIRNODE)
    {
        fswriteBytes(addr, FILENAMEOFFSET, namelength, name);
        fswrite8uint(addr, TYPEOFFSET, type);
        fswrite8uint(addr, VALIDOFFSET, 1);
        fsinitBytes(addr, DIR_ADDRSUBOFFSET, 14, 0);
        fswrite8uint(addr, DIR_UIDOFFSET, uid);
        fswrite16uint(addr, DIR_SIZEOFFSET, 0);
        fswrite8uint(addr, DIR_PARENTOFFSET, parent);
    }
    else if (type == FILENODE)
    {
        fswriteBytes(addr, FILENAMEOFFSET, namelength, name);
        fswrite8uint(addr, TYPEOFFSET, type);
        fswrite8uint(addr, VALIDOFFSET, 1);
        fsinitBytes(addr, FILE_ADDRPAGEOFFSET, 14, 0);
        fswrite8uint(addr, FILE_UIDOFFSET, uid);
        fswrite16uint(addr, FILE_SIZEOFFSET, 0);
        fswrite8uint(addr, FILE_PARENTOFFSET, parent);
    }
    else if (type == DEVNODE)
    {
        fswriteBytes(addr, FILENAMEOFFSET, namelength, name);
        fswrite8uint(addr, TYPEOFFSET, type);
        fswrite8uint(addr, VALIDOFFSET, 1);
        fsinitBytes(addr, DEV_PROGRAMENTRYOFFSET, 14, 0);
        fswrite8uint(addr, DEV_UIDOFFSET, uid);
        fswrite16uint(addr, DEV_SIZEOFFSET, 0);
        fswrite8uint(addr, DEV_PARENTOFFSET, parent);
    }
    else if (type == APPNODE)
    {
        fswriteBytes(addr, FILENAMEOFFSET, namelength, name);
        fswrite8uint(addr, TYPEOFFSET, type);
        fswrite8uint(addr, VALIDOFFSET, 1);
        fsinitBytes(addr, APP_ENTRYOFFSET, 14, 0);
        fswrite8uint(addr, APP_UIDOFFSET, uid);
        fswrite16uint(addr, APP_SIZEOFFSET, 0);
        fswrite8uint(addr, APP_PARENTOFFSET, parent);
    }
    if ((addr == 0) && (parent == 0))
    {
    }
    else
    {
        int i;

        for (i = 0; i < 10; i++)
        {
            uint8_t subaddr;

            subaddr = fsread8uint(parent, DIR_ADDRSUBOFFSET + i);
            if (subaddr == 0)
            {
                fswrite8uint(parent, DIR_ADDRSUBOFFSET + i, addr);
                break;
            }
        }
    }
    return;
}

//-------------------------------------------------------------------------
void printNode(int addr)
{
    char filename[13];
    uint8_t uid, parent, type, valid;
    uint8_t subdirectory[10];
    int i;
    uint8_t flashdirectory[8];

    filename[12] = '\0';
    fsreadBytes(addr, FILENAMEOFFSET, 12, filename);
    type = fsread8uint(addr, TYPEOFFSET);
    valid = fsread8uint(addr, VALIDOFFSET);
    if ((valid > 0) && (type == DIRNODE))
    {
        for (i = 0; i < 10; i++)
        {
            subdirectory[i] = fsread8uint(addr, DIR_ADDRSUBOFFSET + i);
        }
    }
    if ((valid > 0) && (type == FILENODE))
    {
        for (i = 0; i < 8; i++)
        {
            flashdirectory[i] = fsread8uint(addr, FILE_ADDRPAGEOFFSET + i);
        }
    }
    uid = fsread8uint(addr, 28);
    parent = fsread8uint(addr, 31);
    //vs patch 
    //        printf("****Node %d*************\n", addr);
    //        printf("filename: %s\n", filename);
    //        printf("type (dir, file, dev, app) %d\n", type);
    //        printf("valid is %d\n", valid);
    //        if (type == DIRNODE)
    //        {for (i=0;i<10;i++)
    //        printf("The %d sub directory is %d\n", i, subdirectory[i]);
    //        }
    if (type == FILENODE)
    {
        //                for (i=0;i<8;i++)
        //                        printf("The %d sub flash is %d\n", i, flashdirectory[i]);
    }
    //        printf("The node parent is %d\n", parent);
    //        printf("*************************\n");
    return;
}

//-------------------------------------------------------------------------
void formatFS()
{
    int i;

    for (i = 0; i <= FILE_SYS_RANGE; i++)
    {
        fsinitBytes(i, 0, 32, 0);
    }
    //  write8uint(i, VALIDOFFSET, 0); 
    initVectorFlash();
    initVectorNode();
}

//-------------------------------------------------------------------------
void formatFSLite()
{
    initVectorFlash();
    initVectorNode();
}

//-------------------------------------------------------------------------
void deleteNode(int addr)
{
    uint8_t type;
    uint8_t parent;

    type = fsread8uint(addr, TYPEOFFSET);
    parent = fsread8uint(addr, 31);
    if (type == DIRNODE)
    {
        uint8_t i;

        for (i = 0; i < 10; i++)
        {
            uint8_t subaddr;

            subaddr = fsread8uint(addr, DIR_ADDRSUBOFFSET + i);
            if (subaddr > 0)
            {
                deleteNode(subaddr);
            }
        }
        fswrite8uint(addr, VALIDOFFSET, 0);
        if ((addr == 0) && (parent == 0))
        {
        }
        else
        {
            uint8_t i;

            for (i = 0; i < 10; i++)
            {
                uint8_t subaddr;

                subaddr = fsread8uint(parent, DIR_ADDRSUBOFFSET + i);
                if (subaddr == addr)
                {
                    fswrite8uint(parent, DIR_ADDRSUBOFFSET + i, 0);
                    break;
                }
            }
        }
    }
    else if (type == FILENODE)
    {
        char i;
        uint8_t readpage;

        fswrite8uint(addr, VALIDOFFSET, 0);
        if ((addr == 0) && (parent == 0))
        {
        }
        else
        {
            for (i = 0; i < 10; i++)
            {
                uint8_t subaddr;

                subaddr = fsread8uint(parent, DIR_ADDRSUBOFFSET + i);
                if (subaddr == addr)
                {
                    fswrite8uint(parent, DIR_ADDRSUBOFFSET + i, 0);
                    break;
                }
            }
        }
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
        {
            uint8_t currentaddr;

            currentaddr = addr;
            while (1)
            {
                currentaddr = fsread8uint(currentaddr, FILE_NEXTOFFSET);
                if (currentaddr > 0)
                {
                    releaseVectorNode(currentaddr);
                    for (i = 0; i < 8; i++)
                    {
                        readpage =
                            fsread8uint(currentaddr, FILE_ADDRPAGEOFFSET + i);
                        if (readpage == 0)
                        {
                            break;
                        }
                        else
                        {
                            releaseFlashPage(readpage);
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    else if (type == DEVNODE)
    {
        fswrite8uint(addr, VALIDOFFSET, 0);
        if ((addr == 0) && (parent == 0))
        {
        }
        else
        {
            int i;

            for (i = 0; i < 10; i++)
            {
                uint8_t subaddr;

                subaddr = fsread8uint(parent, DIR_ADDRSUBOFFSET + i);
                if (subaddr == addr)
                {
                    fswrite8uint(parent, DIR_ADDRSUBOFFSET + i, 0);
                    break;
                }
            }
        }
    }
    else if (type == APPNODE)
    {
        fswrite8uint(addr, VALIDOFFSET, 0);
        if ((addr == 0) && (parent == 0))
        {
        }
        else
        {
            int i;

            for (i = 0; i < 10; i++)
            {
                uint8_t subaddr;

                subaddr = fsread8uint(parent, DIR_ADDRSUBOFFSET + i);
                if (subaddr == addr)
                {
                    fswrite8uint(parent, DIR_ADDRSUBOFFSET + i, 0);
                    break;
                }
            }
        }
    }
    releaseVectorNode(addr);
}

//-------------------------------------------------------------------------
void buildRootNode()
{
    buildNewNode(FSROOTNODE, "root", 0, 0, DIRNODE);
}

//-------------------------------------------------------------------------
void copyVectorNode(int from, int to)
{
    int i;

    for (i = 0; i < 32; i++)
    {
        uint8_t temp;

        temp = fsread8uint(from, i);
        fswrite8uint(to, i, temp);
    }
}

//-------------------------------------------------------------------------
void copyVectorPage(int from, int to)
{
    int i;
    int pagefrom, pageto;

    for (i = 0; i < 8; i++)
    {
        pagefrom = i + (from - 1) * 8;
        pageto = i + (to - 1) * 8;
        copyPage(pagefrom, pageto);
    }
    return;
}

//-------------------------------------------------------------------------
int inodeMatch(uint8_t addr, char *string)
{
    char temp[13];
    uint8_t length;
    uint8_t i;
    uint8_t valid;

    valid = fsread8uint(addr, VALIDOFFSET);
    if (valid == 0)
    {
        return 0;
    }
    fsreadBytes(addr, FILENAMEOFFSET, 12, temp);
    temp[12] = '\0';
    length = mystrlen(temp);
    for (i = 0; i < length; i++)
    {
        if (superstring(temp + i, string) == 0)
        {
            return 1;
        }
    }
    return 0;
}
