#include <Arduino.h>
#include <BTS7960B.h>

BTS7960B::BTS7960B(){}

BTS7960B::BTS7960B(byte r_pwm, byte l_pwm){
    R_PWM = r_pwm;
    L_PWM = l_pwm;


    #pragma region pin ayarlari
    // pinMode(R_IS, INPUT);
    // pinMode(L_IS, INPUT);

    #pragma endregion
    //~~~~~~~~~~~~~~~~~~~~~~~~
    #pragma region voltaj ayarlari
    // digitalWrite(R_IS, LOW);
    // digitalWrite(L_IS, LOW);

    #pragma endregion
}

BTS7960B::~BTS7960B(){}

/**
 * @brief Verlen rpm değerinde, saat yönünün tersine döner. 
 * 
 * @param rpm 
 */
void BTS7960B::CCLKWTURN(int rpm){
    analogWrite(R_PWM, rpm);
    analogWrite(L_PWM, 0);
}
/**
 * @deprecated
 * 
 * @param rpm 
 * @param _switch 
 */
void BTS7960B::CCLKWTURN(int rpm, bool _switch){
    if(_switch){
        analogWrite(R_PWM, rpm);
        analogWrite(L_PWM, 0);
    }
}

/**
 * @brief Verlen rpm değerinde, saat yönünde döner.
 * 
 * @param rpm 
 */
void BTS7960B::CLKWTURN(int rpm){
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, rpm);
}
/**
 * @deprecated 
 * 
 * @param rpm 
 * @param _switch 
 */
void BTS7960B::CLKWTURN(int rpm, bool _switch){
    if(_switch){
        analogWrite(R_PWM, 0);
        analogWrite(L_PWM, rpm);
    }
}