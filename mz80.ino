/*include <Arduino.h>
#define MZ80 10 // MZ80 pin
#define led 13  // MZ80 uyacıcı led 
int val;

void mz80Setup(){
    // MZ80 gelen veri
    pinMode(MZ80, INPUT);
    // Led output
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
}

void mz80Loop(){
    // Değer MZ80'den gelen input.
    val = digitalRead(MZ80);
    Serial.println(val);
    // Eğer MZ80 görüyorsa,
    if(val == 0){ 
        digitalWrite(LED_BUILTIN, HIGH); 
        delay(500);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
    } 
}*/
