/** @file sounder.c
 	@brief The implementation of the sounder driver.

 	@author Qi Mi (qm8e@virginia.edu)
 	
*/ 	

#include "../hardware/avrhardware.h"
#include "sounder.h"

//--------------------------------------
void sounder_init()
{
    //activate the sounder
    {
        _atomic_t _atomic = _atomic_start();

        LITE_MAKE_PW2_OUTPUT();
        _atomic_end(_atomic);
    }
}

//--------------------------------------
void sounderOn()
{
    {
        _atomic_t _atomic = _atomic_start();

         
		LITE_MAKE_PW2_OUTPUT();
        LITE_SET_PW2_PIN();
        _atomic_end(_atomic);
    }
    return;
}

//--------------------------------------
void sounderOff()
{
    {
        _atomic_t _atomic = _atomic_start();

        
		LITE_MAKE_PW2_OUTPUT();
        LITE_CLR_PW2_PIN();
        _atomic_end(_atomic);
    }
    return;
}
