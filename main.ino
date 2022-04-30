#pragma region Kütüphaneler
// Hazır kütüphaneler
#include <SPI.h>
#include <RF24.h>
// Özel kütüphaneler
#include <nRF24.h>
#include <BTS7960B.h>
#include <MZ80.h>
#include <joystick.h>
#pragma endregion

#pragma region nRF Pinleri
#define CE 48
#define CSN 43
#pragma endregion
#pragma region Motor Pinleri
#define RPWM 13
#define LPWM 12
#define RPWM2 11
#define LPWM2 10
#pragma endregion
#pragma region MZ80 Pinleri
#define MZPIN 32
#define MZPIN1 34
#define MZPIN2 36
#define MZPIN3 38
#define MZPIN4 40
#pragma endregion

// Motorların sabit durma değerleri
const int sabitDeger = 125;
const int sabitDeger2 = 129;
// Optik sensör sayısı.
const int onOptikSayisi = 3;
const int yanOptikSayisi = 2;
RF24 radyo(CE, CSN);
nRF24 radyoModulu;

#pragma region MZ80 Constructorlari
MZ80 optik(MZPIN);   // sağ
MZ80 optik1(MZPIN1); // sol 
MZ80 optik2(MZPIN2); // ön sağ
MZ80 optik3(MZPIN3); // ön orta
MZ80 optik4(MZPIN4); // ön sol
#pragma endregion

#pragma region Motor Constructorlari
BTS7960B motor(RPWM, LPWM);
BTS7960B motor2(RPWM2, LPWM2);
#pragma endregion

#pragma region MZ80 arrayleri

MZ80 onOptikler[onOptikSayisi] = {
  optik2,
  optik3,
  optik4
};
MZ80 yanOptikler[yanOptikSayisi] = {
  optik,
  optik1,
};
#pragma endregion

void setup(){
  Serial.begin(9600);
  radyo = radyoModulu.nRF24AliciKurulum(radyo, RF24_PA_MIN, 9600, RF24_250KBPS);
}

void loop(){
  Kontrol();
}

void Kontrol(){
  radyoModulu.nRF24VeriAl(radyo);
  /*int x;
  // Her optikten gelen 1 veya 0 verilerini al ve toplar.
  for(int i = 0; i < onOptikSayisi; i++){
    x += onOptikler[i].MZ80_OKU();
  }*/
  // Eğer optiklerden herhangi biri 1 verdiyse kontrolü reddeder. 
  // NOT: 1 olduktan sonra kontrol mümkün olmayacak.
  if(radyoModulu.alinanVeri[0] == sabitDeger){
    motor.CLKWTURN(0);
    motor.CCLKWTURN(0);
  }
  if(radyoModulu.alinanVeri[1] == sabitDeger2){
    motor2.CLKWTURN(0);
    motor2.CCLKWTURN(0);
  }
  if(radyoModulu.alinanVeri[0] > sabitDeger){
    motor.CCLKWTURN(map(radyoModulu.alinanVeri[0], sabitDeger, 255, 0, 255));
  }else if(radyoModulu.alinanVeri[0] < sabitDeger){
    motor.CLKWTURN(map(radyoModulu.alinanVeri[0], 0, sabitDeger, -255, 0) * -1);
  }
  if(radyoModulu.alinanVeri[1] > sabitDeger2){
    motor2.CCLKWTURN(map(radyoModulu.alinanVeri[1], sabitDeger2, 255, 0, 255));
  }else if(radyoModulu.alinanVeri[1] < sabitDeger2){
    motor2.CLKWTURN(map(radyoModulu.alinanVeri[1], 0, sabitDeger2, -255, 0) * -1);
  }
  if(radyoModulu.alinanVeri[0] == 0 && radyoModulu.alinanVeri[1] == 0){
    motor.CCLKWTURN(0);
    motor2.CCLKWTURN(0);
  }
}