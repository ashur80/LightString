#include "BlinkLightChain.hpp"

#include <Arduino.h>

/// @brief 
/// @param port 
/// @param delaytime 
BlinkLightChain::BlinkLightChain(unsigned char port, unsigned long delaytime) :
port(port),
delaytime(delaytime),
firenow(true)
{}

void BlinkLightChain::init()
{
    if (delaytime  != UINT16_MAX)
    {
        digitalWrite(port, HIGH);
    }
    else 
    {
        digitalWrite(port, LOW);
    }


    ms = millis();
}

void BlinkLightChain::blink()
{
    if (delaytime == 0)
    {
        firenow = true;
    }
    else if (delaytime == UINT16_MAX)
    {
        firenow = false;
    }
    else
    {
        const unsigned long curms = millis();
        if (curms - ms > delaytime)
        {
            ms = curms;
            firenow = !firenow;
        }
    }

    const uint8_t val = firenow ? HIGH : LOW;
    digitalWrite(port, val);
}

void BlinkLightChain::setDelay(unsigned long newdelay)
{
    delaytime = newdelay;
}
