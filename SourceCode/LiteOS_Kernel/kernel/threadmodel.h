#ifndef THREADMODELH
#define THREADMODELH

int thread_get_next();

#ifdef ENERGY_SHARE_SCHEDULING
  
   void energy_manager_increase_round();
   
#endif 

#endif


