/** @file fsstring.c
	 @brief This file implements several functions for string manipulations in file system. 
	 
	 @author Qing Charles Cao (cao@utk.edu)
*/



#include "fsstring.h"
#include "../../types/types.h"
#include "../../types/string.h"
char *extractString(char *start, char *buffer)
{
    char *p, *q;

    q = buffer;
    p = start;
    if (*p == '/')
    {
        p++;
    }
    while ((isLetter(*p)) || (*p == '_') || (*p == '.') || (isDigit(*p)))
    {
        *q++ = *p++;
    }

    *q = '\0';
    return p;
}

//-------------------------------------------------------------------------
char *extractLastName(char *pathname)
{
    char *p, *start;
    int length;

    length = mystrlen(pathname);
    p = pathname + (length - 1);
    start = pathname;
    while ((*p != '/') && (p != start))
    {
        p--;
    }

    if ((p == start) && ((*start) != '/'))
    {
        return p;
    }
    else
    {
        return ++p;
    }
}

//-------------------------------------------------------------------------
int fileMode(char *s)
{
    if (mystrlen(s) == 1)
    {
        if (s[0] == 'r')
        {
            return 1;
        }

        if (s[0] == 'w')
        {
            return 2;
        }

        if (s[0] == 'a')
        {
            return 3;
        }

        if (s[0] == 't')
        {
            return 4;
        }
    }
    else if (mystrlen(s) == 2)
    {
        return 5;
    }
    else
    {
        return 0;
    }
    return 0;
}
