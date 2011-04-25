#ifndef THREADSYSCALLH
#define THREADSYSCALLH
#include "../kernel/threaddata.h"


thread **getCurrentThreadAddress();

thread *getThreadTableStart();

#endif
