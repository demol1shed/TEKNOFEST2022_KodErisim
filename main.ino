#include <BTS7960B.h>
#include <MZ80.h>

MZ80 mz;
BTS7960B bts;

void setup(){
    Serial.begin(9600);
}

void loop(){
    Serial.println("Hello world.");
    delay(500);
}