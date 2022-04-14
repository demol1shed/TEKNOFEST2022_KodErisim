#ifndef NRF_H
#define NRF_H
#include <Arduino.h>
#include "RF24.h"
#include "printf.h"

class nRFModule{
private:
    byte CE;
    byte CSN;
    uint8_t address[2][6];
    bool radioNum;

public:
    nRFModule(byte ce, byte csn, uint8_t _address[2][6], bool _radioNum){}
    ~nRFModule(){}
    RF24 nRFSetup(int _role){}
    void nRFTransmit(RF24 radio, int _payloads[2]){}
    void nRFReceive(RF24 radio, int _payloads[2]){}
    void nRFSetRole(RF24 radio, int _role){}
};
#endif