/*#include  <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>

#define led 6

RF24 verici(9,10);     
const uint64_t adres[2] = {112 ,155};
 

int buton = 7;     
int sinyal[1];
bool durum = 0;


void setup(void)
{
  Serial.begin(9600);
  verici.begin();      
  verici.setPALevel(RF24_PA_MIN); 
  verici.openWritingPipe(adres[0]); // bilgi gönderildiği adres
  verici.openReadingPipe(1, adres[1]); // bilgi alındığı adres
  pinMode(led, OUTPUT);
}


void loop(void){
  delay(5);
  verici.stopListening();
  if(digitalRead(buton) == 1){
    sinyal[0] = 50;
  }
  else{
    sinyal[0] = 0;
  }
  
  verici.write(&sinyal, sizeof(sinyal)); 



  verici.startListening();
  delay(5);
  if(verici.available()){
    verici.read(&durum, sizeof(durum));
    if(durum == 1){
      digitalWrite(led,HIGH);
    }
    else{
      digitalWrite(led,LOW);
    }
  }

  else{
    digitalWrite(led,LOW);
  }




  
    
}*/
