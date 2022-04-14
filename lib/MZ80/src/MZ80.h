#ifndef MZ80_H        
#define MZ80_H

#include <Arduino.h>

class MZ80{
private:
    int val;
    byte _MZ80;
    byte LED;

public:
    MZ80(byte _mz80){};
    MZ80(byte _mz80, byte led);
    virtual ~MZ80();

    int MZ80_READ(){}
    int MZ80_LED_READ(){}
};
#endif