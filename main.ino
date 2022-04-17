//RECEIVING END
/*#include <nRF24.h>
#include <SPI.h>
#include <RF24.h>

#define CE 7
#define CSN 8

RF24 radio(CE, CSN);
nRF24 radioModule;

void setup(){
    radio = radioModule.nRF24ReceiverSetup(radio, RF24_PA_MIN, 9600, RF24_1MBPS);
}

void loop(){
    radioModule.nRF24Receive(radio);
}*/