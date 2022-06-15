#pragma region Kütüphaneler
// Hazır kütüphaneler
#include <SPI.h>
#include <RF24.h>
#include <string.h>
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
#define RPWM3 6
#define LPWM3 9
#pragma endregion
#pragma region MZ80 Pinleri
#define MZPIN 32
#define MZPIN1 34
#define MZPIN2 36
#define MZPIN3 38
#define MZPIN4 40
#pragma endregion

// Motorların sabit durma değerleri
const int sabitDeger[2] = {125, 129};
// Optik sensör sayısı.
const int onOptikSayisi = 3;
const int yanOptikSayisi = 2;
const int alinanVeriSayisi = 4;

bool switchDurumu;
bool veriDurumu = true;
int alinanVeri[alinanVeriSayisi];

#pragma region nRF Constructorlari
RF24 radyo(CE, CSN);
nRF24 radyoModulu;
#pragma endregion
#pragma region MZ80 Constructorlari
MZ80 optik(MZPIN);   // sağ optik
MZ80 optik1(MZPIN1); // sol optik 
MZ80 optik2(MZPIN2); // ön sağ optik
MZ80 optik3(MZPIN3); // ön orta optik
MZ80 optik4(MZPIN4); // ön sol optik
#pragma endregion
#pragma region Motor Constructorlari
BTS7960B motor(RPWM, LPWM); 
BTS7960B motor2(RPWM2, LPWM2);
BTS7960B motorKirko(RPWM3, LPWM3);
#pragma endregion
#pragma region MZ80 Arrayleri
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
#pragma region Motor Arrayleri
BTS7960B motorlar[3]{
  motor,
  motor2,
  motorKirko
};
#pragma endregion

void setup(){
  Serial.begin(9600);
  radyo = radyoModulu.nRF24AliciKurulum(radyo, RF24_PA_HIGH, 9600, RF24_250KBPS);
}

void loop(){
  // Radyodan veriyi alır.
  radyoModulu.nRF24VeriAl(radyo, alinanVeri, 4);
  switchDurumu = alinanVeri[3];
  Kontrol();
  /*// Sayıcı değişken.
  int x;
  // Öndeki her MZ80'den gelen verileri sayıcı değişkene kaydeder.
  for(int i = 0; i < onOptikSayisi; i++){
    x += onOptikler[i].MZ80_OKU();
  }
  // Eğer herhangi bir sensör 1 vermediyse...
  if(x != 0){
    Kontrol();
  // Eğer herhangi bir sensör 1 verdiyse...  
  }else{
    motor.CCLKWTURN(0);
    motor2.CCLKWTURN(0);
  }*/
  /*if(!veriDurumu){
    motor.CCLKWTURN(0);
    motor2.CCLKWTURN(0);
    motorKirko.CCLKWTURN(0);
  }*/
}

void Kontrol(){
  /*int x;
  // Her optikten gelen 1 veya 0 verilerini al ve toplar.
  for(int i = 0; i < onOptikSayisi; i++){
    x += onOptikler[i].MZ80_OKU();
  }*/
  // Eğer optiklerden herhangi biri 1 verdiyse kontrolü reddeder. 
  // NOT: 1 olduktan sonra kontrol mümkün olmayacak.,
  /**
   * @note Çok fazla if kullanıldı, optimize edilmeli
   * @todo switch() kullanılabilir.
   */
  SabitKal();
  if(!switchDurumu){
    //Serial.println("Hareket modu");
    Hareket();
  }else{
    //Serial.println("Kriko modu");
    KrikoHareket();
  }
  // Veri yoksa...
  if((alinanVeri[0] == 0 && alinanVeri[1] == 0) || (alinanVeri[0] > 255 && alinanVeri[1] > 255) || (alinanVeri[0] < 0 && alinanVeri[1] < 0)){
    motor.CCLKWTURN(0);
    motor2.CCLKWTURN(0);
  }
}

void SabitKal(){
  if(alinanVeri[0] == sabitDeger[0]){
    motor.CLKWTURN(0);
    motor.CCLKWTURN(0);
  }
  if(alinanVeri[1] == sabitDeger[1]){
    motor2.CLKWTURN(0);
    motor2.CCLKWTURN(0);
  }
}

void Hareket(){
  for(int i = 0; i < 2; i++){
    if(alinanVeri[i] > sabitDeger[i]){
      //Serial.print("motor " + i); Serial.println(" ileri");
      motorlar[i].CCLKWTURN(map(alinanVeri[i], sabitDeger[i], 255, 0, 255));
    }else if(alinanVeri[i] < sabitDeger[i]){
      //Serial.print("motor" + i); Serial.println(" geri");
      motorlar[i].CLKWTURN(map(alinanVeri[i], 0, sabitDeger[i], -255, 0) * -1);
    }
  }
}

void KrikoHareket(){
  if(alinanVeri[1] > sabitDeger[1]){
    //Serial.print("motor " + 2); Serial.println(" yukari");
    motorlar[2].CCLKWTURN(map(alinanVeri[1], sabitDeger[1], 255, 0, 255));
  }else if(alinanVeri[1] < sabitDeger[1]){
    //Serial.print("motor " + 2); Serial.println(" asagi");
    motorlar[2].CLKWTURN(map(alinanVeri[1], 0, sabitDeger[1], -255, 0) * -1);
  }else if(alinanVeri[1] == sabitDeger[1]){
    motorlar[2].CCLKWTURN(0);
    motorlar[2].CLKWTURN(0);
  }
}