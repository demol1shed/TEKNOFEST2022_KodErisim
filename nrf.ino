/*#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[5] = {'e','r','i','s','i'};
const unsigned long serialFreq = 9600;

void setup() {
  Start(serialFreq, 0, address);
}
void loop() {
  Main();
}

void Start(unsigned long frequency, uint8_t pipeNum, const byte _address[]){
  Serial.begin(frequency);
  // Alıcıyı başlatır.
  radio.begin();
  // Kanalı tanımlar.
  radio.openReadingPipe(pipeNum, _address);
  // Veri frekansını ayarlar.
  radio.setPALevel(RF24_PA_MAX);
  // Kanalı dinlemeye başlar.
  radio.startListening();
}

void Main(){
  // Eğer kanaldan veri geliyorsa:
  if (radio.available()){
    char text[32]; //print only 32 characters
    // Gelen veriyi oku ve kaydet.
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}*/