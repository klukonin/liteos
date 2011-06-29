/** @file liteoscommon.h
       @brief The functional portals provided by the kernel. 
       
       @author  Qing Cao (cao@utk.edu) 
       
*/



#ifndef LITEOSCOMMONH
#define LITEOSCOMMONH




#include "../types/types.h"



//Definition group 1

#define GET_NODE_ID_FUNCTION 										0xEA00

#define SET_NODE_ID_FUNCTION 										0xEA04

#define GET_RANDOM_NUMBER_FUNCTION 							0xEA08


//Definition group 2 

#define GET_CURRENT_THREAD_ADDRESS_FUNCTION 		0xEA80


//get the index of the current thread 
#define GET_CURRENT_THREAD_INDEX_FUNCTION			  0xEA84

#define GET_CURRENT_THREAD_TABLE_START          0xEA88


#define YIELD_FUNCTION  												0xEA8C

//Post a new thread task to the kernel to continue thread scheduling 
#define POST_NEW_THREAD_TASK 										0xEA90



#define POST_NEW_THREAD_TASK_NOLOG 										0xEA94

//Defintion group 3 

#define POST_TASK_SYSCALL 											0xEB00

//Definition group 4 


//#define MALLOC_MEMORY_FUNCTION 									0xEB80

//#define FREE_MEMORY_FUNCTION 										0xEB84


//getStackPtr()
#define GET_KERNEL_STACK_PTR_ADDRESS						0xEB80

//Definition 5



#define SET_RADIO_FREQ_FUNCTION 								0xEC00

#define SET_RADIO_CHANNEL_FUNCTION 							0xEC04

#define SET_RADIO_POWER_FUNCTION 								0xEC08


//send by radio 
//getRadioSendFunction, then calls SocketRadioSend()
//Assumed to be putting the data into radio data structure then send

#define GET_CURRENT_RADIO_SENDER_BUFFER 						0xEC0C


#define SOCKET_RADIO_SEND_FUNCTION 							0xEC10

//Handle is for receiving packets 

#define GET_CURRENT_RADIO_RECEIVER_BUFFER				0xEC14


//Register the radio receive event 
//calls setCurrentRadioHandle
#define REGISTER_RADIO_RECEIVE_EVENT 						0xEC18





//Definition group 6

//red toggle function 
#define RED_LED_TOGGLE_FUNCTION  								0xEC80

#define YELLOW_TOGGLE_FUNCTION 									0xEC84

//green toggle function 
#define GREEN_LED_TOGGLE_FUNCTION 							0xEC88


 
#define RED_ON_FUNCTION 												0xEC8C

#define RED_OFF_FUNCTION 												0xEC90

#define YELLOW_ON_FUNCTION 											0xEC94

#define YELLOW_OFF_FUNCTION 										0xEC98

#define GREEN_ON_FUNCTION 											0xEC9C

#define GREEN_OFF_FUNCTION 											0xECA0


//Definition group 7

//ADC interfaces 
#define ADC_READ_LIGHT 													0xED00

#define ADC_READ_TEMP 													0xED04

#define ADC_READ_MAGX 													0xED08

#define ADC_READ_MAGY 													0xED0C

#define ADC_READ_ACCX 													0xED10

#define ADC_READ_ACCY 													0xED14

	
//sounderonfunction()
#define SOUNDER_ON_FUNCTION									0xED18

//sounderofffunction()
#define SOUNDER_OFF_FUNCTION									0xED1C


//Definition group 8


#define GET_EEPROM_STRUCTURE_HANDLE 						0xED80

#define READ_EEPROM_TASK 												0xED84

#define WRITE_EEPROM_TASK 											0xED88


//Definition group 9


//get the file path parameter address
//calls getFilePathAddr()
#define GET_FILE_PATH_ADDRESS										0xEE00

//get the file mode parameter address
//calls getFileModeAddr
#define GET_FILE_MODE_ADDRESS 									0xEE04

//open a file
#define OPEN_FILE_SYSCALL 											0xEE08

//close a file 
#define CLOSE_FILE_SYSCALL 											0xEE0C

//read from a file 
#define READ_FILE_SYSCALL 											0xEE10

//write to a file 
#define WRITE_FILE_SYSCALL 											0xEE14

//seek between the file location 
#define SEEK_FILE_SYSCALL 											0xEE18


//Definition group 10 
 
//get the cpu counter	

#define GET_CPU_COUNT_FUNCTION									0xEE80
//
// 
//	

//Definition group 11

////getInternalTracingBlockAddressSyscall() function is called

#ifdef TRACE_ENABLE_EVENT

#define GET_LOGGER_BLOCK_ADDRESS  							    0xEF00
//
////enableTracingSyscall
#define ENABLE_TRACING_SYSCALL									0xEF04
//
////disableTracingSyscall
#define DISABLE_TRACING_SYSCALL									0xEF08
// 

#endif 



typedef struct {
 uint16_t socket_port;
 uint16_t socket_addr;
 uint8_t socket_msg_len;
 uint8_t *socket_msg; 
} radioinfotype; 

 

typedef struct {
  uint16_t addr;
  uint16_t nBytes;
  uint8_t *buffer; 
} genericByteStorageHandle; 


typedef struct {
  uint16_t port;
  uint8_t maxLength; 
  uint8_t *dataReady; 
  uint8_t *data; 
  uint8_t *packetinfo; 
  uint8_t handlevalid; 
  void (*handlefunc) (void);
  
}radiohandletype;


 

typedef struct ecb_block {
 
   uint8_t identifier;
   uint8_t share;  
   uint8_t available; 
   int32_t totalenergy; 
   int32_t remainenergy; 
   

} ecb_block; 


typedef struct lib_thread
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

 //   volatile struct {
    	
    	//used to track the total energy cost of a thread
    //	int32_t energycost;
  //
         	
    	//used to track the energ remain of the current thread 
    	//int32_t energyremain;   
  
  //  }
    
    //energycontrolblock;     
}

//-------------------------------------------------------------------------

lib_thread;



 


typedef struct 
{
  uint8_t valid;
  uint8_t index;
  uint8_t addr;
  uint8_t mode;
  uint16_t size; 
  int32_t fpos;
} LIB_MYFILE;


enum {
  STATE_NULL = 0,     //There is no thread here
  STATE_READY = 1,    //This thread has been created but never executed before
  STATE_ACTIVE = 2,   //There is a thread here, not blocked or sleeping
  STATE_BLOCKED = 3,  //This thread is blocked on a mutex
  STATE_PRESLEEP = 4,    //This thread is about to sleep
  STATE_SLEEP = 5,
  STATE_IO = 6,        //This thread is blocked until I/O Completion
  STATE_FILE = 7,
  STATE_BREAK = 8,
  STATE_MEM_ERROR = 9, 
  STATE_SUSPEND = 10,
  STATE_WAITROLLBACK = 11
};


typedef struct thread_create_block
{
  void (*fp)();
  uint8_t* ram_start;
  uint8_t *stack_ptr;
  uint8_t priority;
  const char *threadname; 
} thread_create_block_type;



 
  #define CPU_PER_THOUSAND 326
  #define RADIO_SEND 167
  #define RADIO_RECEIVE 180
  #define FILE_READ 0
  #define FILE_WRITE 1181
  #define SERIAL_SEND 531 

#endif 
