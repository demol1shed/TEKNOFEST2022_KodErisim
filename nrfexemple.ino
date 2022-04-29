/*/*
 * See documentation at https://nRF24.github.io/RF24
 * See License information at root directory of this library
 * Author: Brendan Doherty (2bndy5)
 */

/**
 * A simple example of sending data from 1 nRF24L01 transceiver to another.
 *
 * This example was written to be used on 2 devices acting as "nodes".
 * Use the Serial Monitor to change each node's behavior.
 
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define JOY_X_A10 10
#define JOY_Y_A11 11

// instantiate an object for the nRF24L01 transceiver
RF24 radio(9, 10); // using pin 7 for the CE pin, and pin 8 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = {"1Node", "2Node"};
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
bool role = false;  // true = TX role, false = RX role

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
// float payload = 0.0;
// vectoralPayloads[0] = xVector, vectoralPayloads[1] = yVector
int vectoralPayloads[2] = {0,0};

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  // print example's introductory prompt
  Serial.println(F("RF24/examples/GettingStarted"));

  // To set the radioNumber via the Serial monitor on startup
  Serial.println(F("Which radio is this? Enter '0' or '1'. Defaults to '0'"));
  while (!Serial.available()) {
    // wait for user input
  }
  char input = Serial.parseInt();
  radioNumber = input == 1;
  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);

  // role variable is hardcoded to RX behavior, inform the user of this
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  // radio.setPayloadSize(sizeof(payload)); // float datatype occupies 4 bytes

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0

  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1

  // additional setup specific to the node's role
  if (role) {
    radio.stopListening();  // put radio in TX mode
  } else {
    radio.startListening(); // put radio in RX mode
  }

  // For debugging info
  // printf_begin();             // needed only once for printing details
  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data

} // setup

void loop() {
  joystick();                                              // record joystick input
  if (role) {
    // This device is a TX node

    unsigned long start_timer = micros();                    // start the timer
    bool report;
    int counter;
    for(int a = 0; a < 2; a++){
      radio.write(&vectoralPayloads[a], sizeof(vectoralPayloads[a]));
      counter++;                                             // transmit & save the report
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
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: "));
      //Serial.println(payload);                               // print payload sent
      //payload += 0.01;                                       // increment float payload

    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
    }

    // to make this example readable in the serial monitor
    // can be decreased to increase the reaction speed of the joystick controllers.
    delay(250);  // slow transmissions down by 1 second

  } else {
    // This device is a RX node

    uint8_t pipe;
    if (radio.available(&pipe)){             // is there a payload? get the pipe number that recieved it
      //uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      //radio.read(&payload, bytes);            // fetch payload from FIFO
      // verilerin toplandığı nokta.
      /*for(int i = 0; i < 2; i++)
        radio.read(&vectoralPayloads[i], sizeof(vectoralPayloads[i]));*/
      /*Serial.print(F("Received "));
      //Serial.print((int)(sizeof(vectoralPayloads[0]) + sizeof(vectoralPayloads[1])));                    // print the size of the payload
      Serial.print(F(" Bytes on pipe "));
      Serial.print(pipe);                                 // print the pipe number
      Serial.print(F(": "));
      /*for(int j = 0; j < 2; j++)
        Serial.print(vectoralPayloads[j]);                // print the payload's value
        Serial.print('\t');*/
   // }
  //} // role

  /*if (Serial.available()) {
    // change the role via the serial monitor

    char c = toupper(Serial.read());
    if (c == 'T' && !role) {
      // Become the TX node

      role = true;
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      radio.stopListening();

    } else if (c == 'R' && role) {
      // Become the RX node

      role = false;
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      radio.startListening();
    }
  }
}

void joystick(){
  vectoralPayloads[0] = analogRead(JOY_X_A10);
  vectoralPayloads[1] = analogRead(JOY_Y_A11);
  Serial.print(vectoralPayloads[0]);
  Serial.print('\t');
  Serial.println(vectoralPayloads[1]);
  delay(250);
}
*/