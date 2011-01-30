#ifndef THREADDATAH
#define THREADDATAH

 
#include "../types/types.h"

#ifdef ENERGY_INSTRUMENTATION
  #include "../system/energycontrol.h"
#endif


typedef struct thread
{
    volatile uint16_t *sp;
    volatile uint8_t state;
    uint8_t priority;
    volatile uint8_t remaincredits;
    uint8_t threadName[12];
    uint16_t *ramstart;
    uint16_t *ramend;
    uint16_t sizeofBss;
    uint16_t romstart;
    uint16_t romsize;
    //this combines thread table with clear function table. 
    void(*thread_clear_function)();
    
    volatile union
    {
        void (*tp) ();
        //mutex implementation simplified
        struct
        {
            uint16_t sleeptime;
            uint8_t sleeptype;
        } sleepstate;
        /*  struct {
           uint8_t id; 
           uint16_t address;
           uint8_t length;
           uint8_t *msg;
           }radiostate; 
         */
       
       //iostate is only used by barrier.  
        struct
        {
            uint8_t type, id;
            uint8_t *msg;
        } iostate;
        
        //adc reading is always blocking. therefore, adcreading only used by external system calls. 
        struct
        {
            int adcreading;
        } adcstate;
    }

    data;
    volatile union
    {
        struct
        {
            uint8_t *fileptr;
            uint8_t *bufferptr;
            uint16_t bytes;
        } filestate;
        struct
        {
            uint8_t *fileptr;
            int offset;
            int position;
        } fileseekstate;
    }

    filedata;
    
    volatile struct {
    	
    	//used to track the total energy cost of a thread
    	int32_t energycost;
  
         	
    	//used to track the energ remain of the current thread 
    	//int32_t energyremain;   
  
    }
    
    energycontrolblock; 
    
}

//-------------------------------------------------------------------------

thread;

//This must be a power of 2
 
      
/**\ingroup thread*/
#define LITE_MAX_THREADS_MASK (LITE_MAX_THREADS - 1)

/**\ingroup thread*/
enum
{
    STATE_NULL = 0,             //There is no thread here
    STATE_READY = 1,            //This thread has been created but never executed before
    STATE_ACTIVE = 2,           //There is a thread here, not blocked or sleeping
    STATE_BLOCKED = 3,          //This thread is blocked on a mutex
    STATE_PRESLEEP = 4,         //This thread is about to sleep
    STATE_SLEEP = 5, STATE_IO = 6,      //This thread is blocked until I/O Completion
    STATE_FILE = 7, STATE_BREAK = 8, STATE_MEM_ERROR = 9,
    
    #ifdef ENERGY_INSTRUMENTATION
    STATE_SUSPEND = 10,
    #endif
    
    STATE_WAITROLLBACK = 11
};



/**\ingroup thread*/
void call_fcn_ptr(void (*tp) ());


#endif
