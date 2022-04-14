#include "nRF.h"
#include <Arduino.h>

nRFModule::nRFModule(byte ce, byte csn, uint8_t _address[2][6], bool _radioNum){
    CE = ce;
    CSN = csn;;
    address[2][6] = _address[2][6];
    radioNum = _radioNum;
}
nRFModule::~nRFModule(){}

RF24 nRFModule::nRFSetup(int _role){
    RF24 radio;
    Serial.begin(115200);
    while(!Serial){}

    if(!radio.begin()){
        Serial.println(F("Radio hardware is not responding!!"));
        while (1) {} // hold in infinite loop
    }

    radio.setPALevel(RF24_PA_MIN);
    radio.openWritingPipe(address[radioNum]);
    radio.openReadingPipe(1, address[!radioNum]);
    switch (_role){
    case 0:
        radio.stopListening();  // put radio in TX mode
        break;
    
    case 1:
        radio.startListening(); // put radio in RX mode
        break;
    }
    return radio;
}

void nRFModule::nRFTransmit(RF24 radio, int _payload[2]){
    unsigned long start_timer = micros();                    // start the timer
    bool report;
    int counter;
    
    for(int a = 0; a < 2; a++){
        radio.write(&_payload[a], sizeof(_payload[a]));
        counter++;                                           // transmit & save the report
    }
    
    switch (counter){
    case 1:
        Serial.println("Only the x vector loaded.");
      break;
    case 2:
        Serial.println("Both vectors successfully loaded.");
        break;
    }

    unsigned long end_timer = micros();                      // end the timer
    if (report) {
        Serial.print(F("Transmission successful! "));        // payload was delivered
        Serial.print(F("Time to transmit = "));
        Serial.print(end_timer - start_timer);               // print the timer result
        Serial.print(F(" μs. Sent: "));

    } else {
        Serial.println(F("Transmission failed or timed out")); // payload was not delivered
    }

    // to make this example readable in the serial monitor
    // can be decreased to increase the reaction speed of the joystick controllers.
    delay(250);  // slow transmissions down by 1 second
}

void nRFModule::nRFReceive(RF24 radio, int _payloads[2]){
    uint8_t pipe;
    if (radio.available(&pipe)){                  // is there a payload? get the pipe number that recieved it
        //uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
        //radio.read(&payload, bytes);            // fetch payload from FIFO
        // verilerin toplandığı nokta.
        radio.read(&_payloads[0], sizeof(_payloads[0]));
        radio.read(&_payloads[1], sizeof(_payloads[1]));
        Serial.print(F("Received "));
        Serial.print((int)(sizeof(_payloads[0]) + sizeof(_payloads[1])));
        Serial.print(F(" Bytes on pipe "));
        Serial.print(pipe);                                 // print the pipe number
        Serial.print(F(": "));
        Serial.print(_payloads[0]);
        Serial.print('\t');
        Serial.println(_payloads[1]);
    }
    delay(250);
}

void nRFModule::nRFSetRole(RF24 radio, int _role){
    switch (_role){
    case 0:
        radio.stopListening();  // put radio in TX mode
        break;
    
    case 1:
        radio.startListening(); // put radio in RX mode
        break;
    }
}