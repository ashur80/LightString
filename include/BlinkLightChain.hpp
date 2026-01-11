#ifndef __BlinkLightChain_Hpp__
#define __BlinkLightChain_Hpp__

// Special sentinel values for BlinkLightChain delay behavior.
// - `BLINK_ALWAYS_ON` means the output should be held ON permanently.
// - `BLINK_ALWAYS_OFF` means the output should be held OFF permanently.
// These are convenient sentinels used by the application; keep them
// in the header so both implementation and callers share the same
// meaning.
constexpr unsigned long BLINK_ALWAYS_ON = 0UL;
constexpr unsigned long BLINK_ALWAYS_OFF = 0xFFFFUL; // matches previous UINT16_MAX used in project

class BlinkLightChain
{
public:
    /// @brief Construct a blink controller for a digital output pin
    /// @param port Digital pin number that the light-chain is connected to
    /// @param delaytime Initial blink period in milliseconds. Use the
    ///        special sentinels `BLINK_ALWAYS_ON` / `BLINK_ALWAYS_OFF`
    ///        to force constant on/off behavior.
    BlinkLightChain(unsigned char port, unsigned long delaytime);

    /// @brief Initialize internal state; call once during `setup()`
    void init();

    /// @brief Called regularly from `loop()` to update the output state
    void blink();

    /// @brief Change the blink delay. Does not implicitly reset internal
    ///        timers; caller can expect an immediate change of behavior
    ///        on the next `blink()` call.
    /// @param newdelay New delay in milliseconds or sentinel constant.
    void setDelay(unsigned long newdelay);

private:
    unsigned char port;
    unsigned long delaytime;
    bool firenow;
    unsigned long ms;
};

#endif //__BlinkLightChain_Hpp__