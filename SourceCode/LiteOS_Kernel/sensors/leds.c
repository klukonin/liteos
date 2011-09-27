/** @file Leds.c
	@brief The detailed implementation of the Leds.

	@author Qing Charles Cao (cao@utk.edu)
*/

#include "../hardware/avrhardware.h"
#include "leds.h"
#include "../utilities/eventlogger.h"

static uint8_t Leds_ledsOn;

/** @brief The definitions of the LED bits.
*/
enum
{
    Leds_RED_BIT = 1, Leds_GREEN_BIT = 2, Leds_YELLOW_BIT = 4
};
void Leds_Leds()
{
    {
        _atomic_t _atomic = _atomic_start();

        Leds_ledsOn = 0;
        LITE_MAKE_RED_LED_OUTPUT();
        LITE_MAKE_YELLOW_LED_OUTPUT();
        LITE_MAKE_GREEN_LED_OUTPUT();
        LITE_SET_RED_LED_PIN();
        LITE_SET_YELLOW_LED_PIN();
        LITE_SET_GREEN_LED_PIN();
        _atomic_end(_atomic);
    }
}

//-------------------------------------------------------------------------
void Leds_redOn()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_CLR_RED_LED_PIN();
        Leds_ledsOn |= Leds_RED_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_redOff()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_SET_RED_LED_PIN();
        Leds_ledsOn &= ~Leds_RED_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_redToggle()
{
    {
        _atomic_t _atomic = _atomic_start();

        if (Leds_ledsOn & Leds_RED_BIT)
        {
            Leds_redOff();
        }
        else
        {
            Leds_redOn();
        }
        _atomic_end(_atomic);
    }
#ifdef TRACE_ENABLE
    addTrace(TRACE_LEDEVENT_REDTOGGLE, 100);
#endif
    return;
}

//-------------------------------------------------------------------------
void Leds_greenOn()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_CLR_GREEN_LED_PIN();
        Leds_ledsOn |= Leds_GREEN_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_greenOff()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_SET_GREEN_LED_PIN();
        Leds_ledsOn &= ~Leds_GREEN_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_greenToggle()
{
    {
        _atomic_t _atomic = _atomic_start();

        if (Leds_ledsOn & Leds_GREEN_BIT)
        {
            Leds_greenOff();
        }
        else
        {
            Leds_greenOn();
        }
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_yellowOn()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_CLR_YELLOW_LED_PIN();
        Leds_ledsOn |= Leds_YELLOW_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_yellowOff()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_SET_YELLOW_LED_PIN();
        Leds_ledsOn &= ~Leds_YELLOW_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_yellowToggle()
{
    {
        _atomic_t _atomic = _atomic_start();

        if (Leds_ledsOn & Leds_YELLOW_BIT)
        {
            Leds_yellowOff();
        }
        else
        {
            Leds_yellowOn();
        }
        _atomic_end(_atomic);
    }
    return;
}
