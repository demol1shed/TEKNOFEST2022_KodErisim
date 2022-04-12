#include "C:\Users\user\Documents\Arduino\libraries\Custom_Headers\src\BTS7960B.h"
#include "C:\Users\user\Documents\Arduino\libraries\Custom_Headers\src\MZ80.h"
#include <Arduino.h>
//#include <Q2HX711.h>

#define MZ80_CON 10 // MZ80 pin
#define LED_ARD 13  // MZ80 uyacıcı led

#define DATA 3
#define CLK 4

MZ80 optikSensor;
BTS7960B motor;

void setup(){
    Serial.begin(9600);
    motor.BTS_SETUP();
    optikSensor.MZ80_SETUP(MZ80_CON);
}

void loop(){
    bool veri = optikSensor.MZ80_INIT(MZ80_CON);
    if(veri){
        motor.CCLKWTURN(0);
    }else{
        motor.CCLKWTURN(255);
    }
    //delay(50);
}