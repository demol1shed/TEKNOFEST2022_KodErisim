/*#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led 2

bool sinyal[1];
int durum = 0;

RF24 alici(9,10);
const uint64_t adres[2] = {112 , 155};




void setup(){
  Serial.begin(9600);
  alici.begin();
  alici.setPALevel(RF24_PA_MIN);
  alici.openReadingPipe(1, adres[0]); // bilgi aldığı adres
  alici.openWritingPipe(adres[1]); // bilgi gönderdiği adres
  pinMode(led, OUTPUT);
}

void loop(){
  
  alici.startListening();
  delay(5);
  if (alici.available()){
    alici.read(&durum, sizeof(durum));      
    if (durum == 50){
      digitalWrite(led, HIGH);
      sinyal[0] = 1;
      alici.stopListening();
      alici.write(&sinyal, sizeof(sinyal));
    }
  
    else {
      alici.stopListening();
      sinyal[0] = 0;
      digitalWrite(led, LOW);
      alici.write(&sinyal, sizeof(sinyal));
    }
  }
  else{
    digitalWrite(led, LOW);
  }


}*/