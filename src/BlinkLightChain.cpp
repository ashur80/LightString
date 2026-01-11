#include "BlinkLightChain.hpp"

#include <Arduino.h>

// BlinkLightChain implementation
// - `delaytime` uses sentinel values defined in the header to represent
//   always-on / always-off behavior. Regular positive values are treated
//   as millisecond intervals for toggling the output.
// - Timing uses `millis()` and unsigned arithmetic so the code handles
//   the natural 49-day overflow of `millis()` correctly.

/// @brief Construct a BlinkLightChain controller
/// @param port Digital output pin to toggle
/// @param delaytime Blink interval in milliseconds or sentinel
BlinkLightChain::BlinkLightChain(unsigned char port, unsigned long delaytime) :
    port(port),
    delaytime(delaytime),
    firenow(true)
{}

void BlinkLightChain::init()
{
    // Initialize output according to sentinel. If not forced-off,
    // set the pin HIGH (ON) to start.
    if (delaytime  != BLINK_ALWAYS_OFF)
    {
        digitalWrite(port, HIGH);
    }
    else
    {
        digitalWrite(port, LOW);
    }

    // Record current time as the baseline for future toggles.
    ms = millis();
}

void BlinkLightChain::blink()
{
    // Handle sentinel behaviors first
    if (delaytime == BLINK_ALWAYS_ON)
    {
        firenow = true;
    }
    else if (delaytime == BLINK_ALWAYS_OFF)
    {
        firenow = false;
    }
    else
    {
        // Normal blinking mode: toggle when interval has passed.
        const unsigned long curms = millis();
        // Unsigned subtraction handles millis() rollover safely.
        if (curms - ms > delaytime)
        {
            ms = curms;
            firenow = !firenow;
        }
    }

    // Apply the computed output level to the pin.
    const uint8_t val = firenow ? HIGH : LOW;
    digitalWrite(port, val);
}

void BlinkLightChain::setDelay(unsigned long newdelay)
{
    // Update the delay. We intentionally don't reset `ms` here so callers
    // can choose whether to preserve the current timing phase or reset it
    // by calling `init()` again; resetting `ms` here would cause an
    // immediate delay before the first toggle.
    delaytime = newdelay;
}
