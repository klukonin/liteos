/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu

LiteOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LiteOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "libfile.h"
#include "libstring.h"
#include "libthread.h"
#include "liteoscommon.h"
#include "libmutex.h"


char *lib_get_file_path_address()
{
   char *pathaddr;
   void (*getaddrfp)(void) = (void (*)(void))GET_FILE_PATH_ADDRESS;
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (pathaddr)
				 :);
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
    return pathaddr;
}



char *lib_get_file_mode_address()

{
   char *modeaddr;
   void (*getaddrfp)(void) = (void (*)(void))GET_FILE_MODE_ADDRESS;
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (modeaddr)
				 :);
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
    return modeaddr;
}




void lib_open_file_syscall()
{
 void (*filefp)() = (void (*)(void))OPEN_FILE_SYSCALL;
 filefp();
}



void lib_close_file_syscall()
{
 void (*filefp)() = (void (*)(void))CLOSE_FILE_SYSCALL;
 filefp();
}


void lib_read_file_syscall()
{
 void (*filefp)() = (void (*)(void))READ_FILE_SYSCALL;
 filefp();
}



void lib_write_file_syscall()
{
 void (*filefp)() = (void (*)(void))WRITE_FILE_SYSCALL;
 filefp();
}



void lib_seek_file_syscall()
{
 void (*filefp)() = (void (*)(void))SEEK_FILE_SYSCALL;
 filefp();
}




LIB_MYFILE *lib_mfopen(const char *pathname, const char *mode)
{
   char *commonpathnameaddr;

   char *commonmodeaddr;

   uint8_t currentthreadindex;

   lib_thread** current_thread;


   current_thread = lib_get_current_thread();

   currentthreadindex = lib_get_current_thread_index();

   commonpathnameaddr =  lib_get_file_path_address();

   commonmodeaddr = lib_get_file_mode_address();


   lib_mystrcpy(commonpathnameaddr, pathname);
   lib_mystrcpy(commonmodeaddr, mode);

   lib_open_file_syscall();

   lib_file_barrier_block(7, 1);

   return (LIB_MYFILE *)((*current_thread)->filedata.filestate.fileptr);
}



void lib_mfclose(LIB_MYFILE *fp)
{
   uint8_t currentthreadindex;
   lib_thread** current_thread;

   current_thread = lib_get_current_thread();
   currentthreadindex = lib_get_current_thread_index();


   (*current_thread)->filedata.filestate.fileptr = (uint8_t*)fp;
   lib_close_file_syscall();

   lib_file_barrier_block(7, 2);

   return;

}

void lib_mfread(LIB_MYFILE *fp, void *buffer, int nBytes)
{
   uint8_t currentthreadindex;
   lib_thread** current_thread;

   current_thread = lib_get_current_thread();
   currentthreadindex = lib_get_current_thread_index();


   (*current_thread)->filedata.filestate.fileptr = (uint8_t*)fp;
   (*current_thread)->filedata.filestate.bufferptr = (uint8_t*)buffer;
   (*current_thread)->filedata.filestate.bytes = nBytes;

   lib_read_file_syscall();

   lib_file_barrier_block(7, 3);

   return;

}



void lib_mfwrite(LIB_MYFILE *fp, void *buffer, int nBytes)
{
   uint8_t currentthreadindex;
   lib_thread** current_thread;

   current_thread = lib_get_current_thread();
   currentthreadindex = lib_get_current_thread_index();
   
  
     
        
   //(*current_thread)->energycontrolblock.energycost += (uint16_t)FILE_WRITE * (uint16_t)nBytes;         
  


   (*current_thread)->filedata.filestate.fileptr = (uint8_t*)fp;
   (*current_thread)->filedata.filestate.bufferptr = (uint8_t*)buffer;
   (*current_thread)->filedata.filestate.bytes = nBytes;

   lib_write_file_syscall();

   lib_file_barrier_block(7, 4);
   
   
 

   return;
}


void lib_mfwrite_withoutlength(LIB_MYFILE *fp, void *buffer)
{


   uint8_t length;

   length = (uint8_t)lib_string_length((char*)buffer);

   lib_mfwrite(fp, buffer, length);

   return;
}






void lib_mfseek(LIB_MYFILE *fp, int offset, int position)
{
   uint8_t currentthreadindex;
   lib_thread** current_thread;

   current_thread = lib_get_current_thread();
   currentthreadindex = lib_get_current_thread_index();

   (*current_thread)->filedata.fileseekstate.fileptr = (uint8_t*)fp;
   (*current_thread)->filedata.fileseekstate.offset = offset;
   (*current_thread)->filedata.fileseekstate.position = position;

   lib_seek_file_syscall();

   return;



}
