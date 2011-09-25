/** @file eventlogger.h
	@brief The functional declaration of the event logger headers. 

	@author Qing Cao (cao@utk.edu)
*/




#ifndef EVENTLOGGERH
#define EVENTLOGGERH

#include "../types/types.h"


//Following are deprecated. 

//determines if the context switches in the kernel are logged or not
// #define TRACE_ENABLE_CONTEXTSWITCH

//determines if kernel events on sending and recieving are logged or not. 
// #define TRACE_ENABLE_RADIOEVENT

//determines if adc evevents in the kernel are logged or not 
// #define TRACE_ENABLE_ADCEVENT

//determines led events are logged or not 
// #define TRACE_ENABLE_LEDEVENT

//determines if create thread event is logged or not 
// #define TRACE_ENABLE_THREADCREATE


//determiens if destroy events are logged or not 
// #define TRACE_ENABLE_THREADDESTROY
 
 


/** @defgroup logging Logging and reporting module. 
    @{
*/
/**
    @brief Kernel and syscall events 

*/

#define TRACE_CONTEXTSWITCHTOUSERTHREAD     								1
#define TRACE_CONTEXTSWITCHFROMUSERTHREAD   								2
#define TRACE_RADIOEVENT_RECEIVEPACKET	    								3
#define TRACE_RADIOEVENT_SENDPACKET 										4
#define TRACE_ADCEVENT_READLIGHT            								5
#define TRACE_ADCEVENT_READTEMP 											6

#define TRACE_LEDEVENT_REDTOGGLE            								20
#define TRACE_LEDEVENT_GREENTOGGLE          								21
#define TRACE_LEDEVENT_YELLOWTOGGLE         								22

#define TRACE_THREADCREATE                                                  23
#define TRACE_THREADDESTROY													24


//List of system calls. Each could be separately enabled or disabled.

#define TRACE_SYSCALL_GETNODEID       										101
#define TRACE_SYSCALL_SETNODEID        										102
#define TRACE_SYSCALL_GETRANDOMNUMBER   									103

#define TRACE_SYSCALL_GETCURRENTTHREADADDRESS   							201
#define TRACE_SYSCALL_GETCURRENTTHREADINDEX  	   							202
#define TRACE_SYSCALL_GETTHREADCONTROLBLOCK                                 203
#define TRACE_SYSCALL_YIELDFUNCTION										    204
#define TRACE_SYSCALL_POSTTHREADTASK           						    	205

#define TRACE_SYSCALL_POSTTASKSYSCALL                                       301

#define TRACE_SYSCALL_KERNELSTACK                                           401

#define TRACE_SYSCALL_SETRADIOFREQUENCY 									501
#define TRACE_SYSCALL_SETRADIOCHANNEL   									502
#define TRACE_SYSCALL_SETRADIOPOWER     									503
#define TRACE_SYSCALL_GETRADIOSENDBUFFER  								    504
#define TRACE_SYSCALL_INVOKERADIOSENDFUNCTION                               505
#define TRACE_SYSCALL_GETRADIORECEIVEBUFFER 						        506
#define TRACE_SYSCALL_REGISTERRECEIVERHANDLE    			            	507
#define TRACE_SYSCALL_GETRADIOLOCK											508
#define TRACE_SYSCALL_SETRADIOLOCK											509
#define TRACE_SYSCALL_RELEASERADIOLOCK										510
 


#define TRACE_SYSCALL_REDTOGGLEFUNCTION                                     601
#define TRACE_SYSCALL_YELLOWTOGGLEFUNCTION    							    602
#define TRACE_SYSCALL_GREENTOGGLEFUNCTION   	                            603
#define TRACE_SYSCALL_REDONFUNCTION         			                    604
#define TRACE_SYSCALL_REDOFFFUNCTION        								605
#define TRACE_SYSCALL_YELLOWONFUNCTION        							    606
#define TRACE_SYSCALL_YELLOWOFFFUNCTION     								607
#define TRACE_SYSCALL_GREENONFUNCTION        								608
#define TRACE_SYSCALL_GREENOFFFUNCTION       								609


#define TRACE_SYSCALL_ADCLIGHTSYSCALL      								    701
#define TRACE_SYSCALL_ADCTEMPSYSCALL       								    702
#define TRACE_SYSCALL_ADCMAGXSYSCALL       								    703
#define TRACE_SYSCALL_ADCMAGYSYSCALL       								    704
#define TRACE_SYSCALL_ADCACCXSYSCALL       								    705
#define TRACE_SYSCALL_ADCACCYSYSCALL       								    706
#define TRACE_SYSCALL_SOUNDERONFUNCTION     					            707
#define TRACE_SYSCALL_SOUNDEROFFFUNCTION     					            708

#define TRACE_SYSCALL_GETCURRENTEEPROMHANDLEADDRESS   			            709
#define TRACE_SYSCALL_READFROMEEPROM   										710
#define TRACE_SYSCALL_WRITETOEEPROM    										711

#define TRACE_SYSCALL_GETFILEPATHADDR        								801
#define TRACE_SYSCALL_GETFILEMODEADDR        								802
#define TRACE_SYSCALL_OPENFILESYSCALL        								803
#define TRACE_SYSCALL_CLOSEFILESYSCALL       								804
#define TRACE_SYSCALL_READFILESYSCALL        								805
#define TRACE_SYSCALL_WRITEFILESYSCALL       								806
#define TRACE_SYSCALL_SEEKFILESYSCALL      								    807

#define TRACE_SYSCALL_GETCPUCOUNTSYSCALL                                    901

#ifdef PLATFORM_CPU_MEASURE
#define TRACE_SYSCALL_GETCPUUTILIZATIONSYSCALL								1001
#endif 

#ifdef TRACE_ENABLE
/** @brief Init the event logging with channel. 
	@param basechannel The base channel used. 
	@param reportingchannel The channel used.
	@param reportinterval The interval for reporting. 
	@return Void. 
*/
void initTrace(uint8_t basechannel, uint8_t reportingchannel,  uint16_t reportinterval); 

/** @brief Add to trace.
	@param eventid The event id. 
	@param threadid The thread id. 
	@return Void. 
*/ 
void addTrace(uint16_t eventid, uint8_t threadid);

/** @brief Init done. 
	@return Void.
*/

void GenericInitTimerFired();


/** @brief Report the internal events. 
	@return Void. 
*/
void reportTrace();
/** @} */

#endif
#endif
