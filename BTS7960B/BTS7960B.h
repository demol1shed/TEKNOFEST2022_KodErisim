#ifndef BTS7960B_H        
#define BTS7960B_H
#include <Arduino.h>
class BTS7960B{
private:
    byte R_PWM;
    byte L_PWM;

    public:  
    BTS7960B();
    BTS7960B(byte r_pwm, byte l_pwm);
    virtual ~BTS7960B();
    // Motoru saat yönünün tersine doğru döndürür.
    void CCLKWTURN(int rpm);
    // Motoru açıp kapama overloaduna sahip parametresi ile motoru saat yönünün tersine döndürür.
    void CCLKWTURN(int rpm, bool _switch);
    // Motoru saat yönününde döndürür.
    void CLKWTURN(int rpm);
    // Motoru açıp kapama overloaduna sahip parametresi ile motoru saat yönünüde döndürür.
    void CLKWTURN(int rpm, bool _switch);
};
#endif