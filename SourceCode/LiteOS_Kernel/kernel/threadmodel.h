/**  @file threadmodel.h
        @brief The header for the thread model module. 

        @author Qing Charles Cao (cao@utk.edu)
*/


#ifndef THREADMODELH
#define THREADMODELH

/** @addtogroup scheduling*/
/** @{ */

/** @brief Get the next thread.
	@return Thread index. 
*/
int thread_get_next();

#ifdef ENERGY_SHARE_SCHEDULING

   /** @brief increase round. 
   	     @return Void. 
     */
   void energy_manager_increase_round();
   
#endif 

/** @} */
#endif


