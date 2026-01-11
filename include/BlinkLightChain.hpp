#ifndef __BlinkLightChain_Hpp__
#define __BlinkLightChain_Hpp__

class BlinkLightChain
{
public:
    BlinkLightChain(unsigned char port, unsigned long delaytime);
    void init();
    void blink();
    void setDelay(unsigned long newdelay);

private:
    unsigned char port;
    unsigned long delaytime;
    bool firenow;
    unsigned long ms;
};

#endif //__BlinkLightChain_Hpp__