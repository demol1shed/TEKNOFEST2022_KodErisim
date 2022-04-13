#include <Arduino.h>
//#include <Q2HX711.h>

//deneme

#define DATA 3
#define CLK 4

//Q2HX711 hx711(DATA, CLK);

void setup() {
  Serial.begin(9600);
}

void loop() {
  //Serial.println(hx711.read());
  Serial.print("Kurulum başarılı.");
  delay(500);
}
