//TRANSMITTING END
/*#include <nRF24.h>
#include <joystick.h>
#include <SPI.h>
#include <RF24.h>

#define CE 7
#define CSN 8
#define JOYX A0
#define JOYY A1

const int joystickReadDelay = 5;

// NOTE: re-instancing radio in radiomodulesetup could cause issue.
RF24 radio(CE, CSN);
nRF24 radioModule;
Joystick joystick(JOYX, JOYY);

void setup(){
    radio = radioModule.nRF24TransmitterSetup(radio, RF24_PA_MIN, 9600, RF24_1MBPS);
}

void loop(){
    joystick.JoystickRead(joystickReadDelay);
    radioModule.nRF24Transmit(radio, joystick.inputVectors);
}*/