/**  @file threaddata.c
        @brief The implementation for the scheduling module. 

        @author Qing Charles Cao (cao@utk.edu)
        
        
*/

#include "threaddata.h"
void call_fcn_ptr(void (*tp) ())
{
    (*tp) ();
}
