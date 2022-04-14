#include <Arduino.h>
#include "MZ80.h"

MZ80::MZ80(byte _mz80){
    _MZ80 = _mz80;
    pinMode(_MZ80, INPUT);
}
MZ80::MZ80(byte _mz80, byte led){
    _MZ80 = _mz80;
    LED = led;
    pinMode(_MZ80, INPUT);
    pinMode(LED, OUTPUT);
}
MZ80::~MZ80(){}

int MZ80::MZ80_READ(){
    return digitalRead(_MZ80);
}

int MZ80::MZ80_LED_READ(){
    val = digitalRead(_MZ80);
        if(val == 0 && LED != 0){
            digitalWrite(LED, HIGH); 
            delay(500);
            return val;
        }else{
            if(LED != 0){
                digitalWrite(LED, LOW);
                delay(500);
                return val;
            }else{
                digitalWrite(LED_BUILTIN, LOW);
                delay(500);
                return val;
            }
        }
}