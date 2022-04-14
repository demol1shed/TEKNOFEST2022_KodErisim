#include <Arduino.h>
#include "BTS7960B.h"

BTS7960B::BTS7960B(byte r_pwm, byte l_pwm, byte r_is, byte l_is, byte l_en, byte r_en){
    R_PWM = r_pwm;
    L_PWM = l_pwm;
    R_IS = r_is;
    L_IS = l_is;
    R_EN = r_en;
    L_EN = l_en;

    #pragma region pin ayarlari
    pinMode(R_IS, INPUT);
    pinMode(L_IS, INPUT);
    pinMode(R_PWM, OUTPUT);
    pinMode(L_PWM, OUTPUT);
    pinMode(R_EN, OUTPUT);
    pinMode(L_EN, OUTPUT);
    #pragma endregion
    //~~~~~~~~~~~~~~~~~~~~~~~~
    #pragma region voltaj ayarlari
    digitalWrite(R_IS, LOW);
    digitalWrite(L_IS, LOW);
    digitalWrite(R_EN, HIGH);
    digitalWrite(L_EN, HIGH);
    #pragma endregion
}

BTS7960B::~BTS7960B(){}

void BTS7960B::CCLKWTURN(int rpm){
    analogWrite(R_PWM, rpm);
    analogWrite(L_PWM, 0);
}
void BTS7960B::CCLKWTURN(int rpm, bool _switch){
    if(_switch){
        analogWrite(R_PWM, rpm);
        analogWrite(L_PWM, 0);
    }
}

void BTS7960B::CLKWTURN(int rpm){
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, rpm);
}
void BTS7960B::CLKWTURN(int rpm, bool _switch){
    if(_switch){
        analogWrite(R_PWM, 0);
        analogWrite(L_PWM, rpm);
    }
}