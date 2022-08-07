#pragma region Kütüphaneler
// Hazır kütüphaneler
#include <SPI.h>
#include <RF24.h>
#include <string.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <Wire.h>
#include <FastLED.h>
// Özel kütüphaneler.
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
#define sol_on_optik 32
#define orta_optik 34
#define sag_on_optik 36
#define sag_optik 38
#define sol_optik 40
#pragma endregion

#pragma region Optik
const int onOptikSayisi = 3;
const int yanOptikSayisi = 2;
const int alinanVeriSayisi = 4;
#pragma endregion

#pragma region sarj
float vA = 0.0;
float vB = 0.0;
float vGiris = 0.0;
float vOrtalama = 0.0;
float RA = 99.2;
float RB = 9.7;
float RC = 9.8;
float vToplam = 0.0;
#pragma endregion

const int sabitDeger[2] = {125, 129}; // Motorların sabit durma değerleri
bool switchDurumu;
bool veriDurumu = true;
int alinanVeri[alinanVeriSayisi];

#pragma region nRF Constructorlari
RF24 radyo(CE, CSN);
nRF24 radyoModulu;
#pragma endregion
#pragma region MZ80 Constructorlari
// sağ optik
MZ80 sagOptik(sag_optik);
// sol optik
MZ80 solOptik(sol_optik);
// ön sağ optik
MZ80 sagOnOptik(sag_on_optik);
// ön orta optik
MZ80 ortaOptik(orta_optik);
// ön sol optik
MZ80 solOnOptik(sol_on_optik);
#pragma endregion
#pragma region Motor Constructorlari
BTS7960B motor(RPWM, LPWM);    // sag
BTS7960B motor2(RPWM2, LPWM2); // sol
BTS7960B motorKirko(RPWM3, LPWM3);
#pragma endregion
#pragma region QrKod Constructoru
SoftwareSerial mySerial(14, 15); // RX TX
#pragma endregion
#pragma region MZ80 Arrayleri
MZ80 onOptikler[onOptikSayisi] = {
    sagOnOptik,
    solOnOptik,
    ortaOptik};
MZ80 yanOptikler[yanOptikSayisi] = {
    sagOptik,
    solOptik,
};
#pragma endregion
#pragma region Motor Arrayleri
BTS7960B motorlar[3]{
    motor,
    motor2,
    motorKirko};
#pragma endregion

#pragma region Neopixel Degiskenler
#define NUM_LEDS 200   // Kullanılan Led Sayısı
#define DATA_PIN 4     // Hangi Pine Bağlancağı
#define BRIGHTNES 128  // Parlaklık Ayarı
#define CHIPSET WS2812 // Ledin Modeli
CRGB leds[NUM_LEDS];   // Kütüphane Fonksiyonu
int sagrenk = CRGB::Black;
int arkarenk = CRGB::Black;
int solrenk = CRGB::Black;
int onrenk = CRGB::Black;
int ustsagrenk = CRGB::Black;
int ustonrenk = CRGB::Black;
int ustsolrenk = CRGB::Black;
int ustarkarenk = CRGB::Black;

#pragma endregion

int buzzer = 20;

void setup()
{
  Serial.begin(9600);                                                       // Seri Haberleşme Başlar
  radyo = radyoModulu.nRF24AliciKurulum(radyo, RF24_PA_HIGH, RF24_250KBPS); // Radyo Frekans Değeri
  mySerial.begin(9600);                                                     // Seri Kanal Açılır
  pinMode(buzzer, OUTPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

char a[] = {"Q1;"}; // char* a ile aynı deger
void loop()
{
  /*
    qrKodKontrol();
    int deger = qrOku();
    qrKarar(deger);
//--------------------------------------------------------
    switch (EngelKontrol())
    {
    case 0:
      OtonomHareket();
      break;
    case 1:
      digitalWrite(buzzer, 1);
      sagrenk = CRGB ::Red;
      Ust_on();
      sagrenk = CRGB ::Red;
      On_led();
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      delay(15000);
      if (EngelKontrol() != 0)
      {

        motor.CLKWTURN(50);
        motor.CCLKWTURN(172);
      }

      break;
    case 2:
      digitalWrite(buzzer, 1);
      ustonrenk = CRGB ::Red;
      Ust_on();
      onrenk = CRGB ::Red;
      On_led();
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      delay(15000);
      break;
    case 3:
      digitalWrite(buzzer, 1);
      ustonrenk = CRGB ::Red;
      Ust_on();
      onrenk = CRGB ::Red;
      On_led();
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      delay(1500);
      break;
    default:
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      break;
    }
  */
  // Gyro();
  // Serial.println(zEkseni);
  /*
    if (Sarj() <= 23.71)
    {
      ustsagrenk = CRGB::OrangeRed;
      ust_sag();
    }
  */
  //  Radyodan veriyi alır.
  // Sarj();
  radyoModulu.nRF24VeriAl(radyo, alinanVeri, 4);
  switchDurumu = alinanVeri[3];
  Kontrol();
}

#pragma region Kumanda Kontrol
void Kontrol()
{
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
  if (!switchDurumu)
  {
    Hareket();
  }
  else
  {
    KrikoHareket();
  }
  // Veri yoksa...
  if ((alinanVeri[0] == 0 && alinanVeri[1] == 0) || (alinanVeri[0] > 255 && alinanVeri[1] > 255) || (alinanVeri[0] < 0 && alinanVeri[1] < 0))
  {
    motor.CCLKWTURN(0);
    motor2.CCLKWTURN(0);
  }
}

void SabitKal()
{
  if (alinanVeri[0] == sabitDeger[0])
  {
    motor.CLKWTURN(0);
    motor.CCLKWTURN(0);
  }
  if (alinanVeri[1] == sabitDeger[1])
  {
    motor2.CLKWTURN(0);
    motor2.CCLKWTURN(0);
  }
}

void Hareket()
{
  for (int i = 0; i < 2; i++)
  {
    if (alinanVeri[i] > sabitDeger[i])
    {
      // ileri
      motorlar[i].CCLKWTURN(map(alinanVeri[i], sabitDeger[i], 255, 0, 255));
    }
    else if (alinanVeri[i] < sabitDeger[i])
    {
      // geri
      motorlar[i].CLKWTURN(map(alinanVeri[i], 0, sabitDeger[i], -255, 0) * -1);
    }
  }
}

void KrikoHareket()
{
  if (alinanVeri[1] > sabitDeger[1])
  {
    // yukari
    motorlar[2].CCLKWTURN(map(alinanVeri[1], sabitDeger[1], 255, 0, 255));
  }
  else if (alinanVeri[1] < sabitDeger[1])
  {
    // asagi
    motorlar[2].CLKWTURN(map(alinanVeri[1], 0, sabitDeger[1], -255, 0) * -1);
  }
  else if (alinanVeri[1] == sabitDeger[1])
  {
    motorlar[2].CCLKWTURN(0);
    motorlar[2].CLKWTURN(0);
  }
}
#pragma endregion

#pragma region Qrkod
int k = 0;
char qrGelen = '0'; // Kaydedilcek Değisken Yeri
#pragma endregion
void qrKodKontrol()
{
  // Qr Sensörü Okuyor Ve qrGelen Değişkine Kaydediyor
  if (mySerial.available())
  {
    qrGelen = mySerial.read();
    a[k] = qrGelen;
    k++;
  }
}

int qrOku()
{
  if (a[2] == ';')
  {
    // ; karakteri bulundu
    return a[1] - '0';
  }
  else
  {
    int gonderilecek = (a[1] - '0') * 10 + (a[2] - '0');
    // ; karakteri bulunamadi
    return gonderilecek;
  }
}

#pragma region YuksuzSenaryo 1
/*
int q22 = 0;
void qrKarar(int gelenDeger)
{
  switch (gelenDeger)
  {
  case 1:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(1000);
    motor.CCLKWTURN(172);
    motor2.CLKWTURN(50);
    delay(500);
    break;
  case 2:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(1000);
    break;
  case 3:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(2000);
    break;
  case 4:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(1000);
    break;
  case 5:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(2000);
    break;
  case 6:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(1000);
    break;
  case 7:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(1000);
    break;
  case 8:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(1000);
    break;
  case 9:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(1000);
    break;
  case 10:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(2000);
    break;
  case 11:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(1000);
    break;
  case 12:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(2000);
    break;
  case 13:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(1000);
    break;
  case 18:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(2000);
    break;
  case 19:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(1000);
    break;
  case 20:
    motor.CCLKWTURN(50);
    motor.CCLKWTURN(50);
    delay(2000);
    break;
  case 21:
    motor.CCLKWTURN(50);
    motor2.CLKWTURN(50);
    delay(1000);
    break;
  case 22:
    if (q22 == 0)
    {
      motor.CCLKWTURN(50);
      motor2.CLKWTURN(50);
      delay(1000);
      q22++;
    }
    else if (q22 == 1)
    {
      motor.CCLKWTURN(0);
      motor2.CLKWTURN(0);
      delay(1000);
      q22++;
    }
    break;
  case 23:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 24:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 25:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 26:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 27:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 28:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 29:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    motor.CCLKWTURN(172);
    motor2.CLKWTURN(50);
    delay(500);
    break;
  }
}*/
#pragma endregion

#pragma region YuksuzSeneryo 2
/*
int q22 = 0;
void qrKarar(int gelenDeger)
{
  switch (gelenDeger)
  {
  case 1:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 2:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 3:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 4:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 5:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 6:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 7:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 8:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 9:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 10:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 11:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 12:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 15:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(500);
    break;
  case 16:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 17:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 18:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 19:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 20:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 21:
    motor.CCLKWTURN(50);
    motor2.CLKWTURN(50);
    delay(1000);
    break;
  case 22:
    if (q22 == 0)
    {
      motor.CCLKWTURN(50);
      motor2.CLKWTURN(50);
      delay(1000);
      q22++;
    }
    else if (q22 == 1)
    {
      motor.CCLKWTURN(0);
      motor2.CLKWTURN(0);
      delay(1000);
      q22++;
    }
    break;
  case 23:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 24:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 25:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 26:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 27:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 29:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 30:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  }
}
*/
#pragma endregion

#pragma region YuksuzSeneryo 3
/*
int q7 = 0;
void qrKarar(int gelenDeger)
{
  switch (gelenDeger)
  {
  case 2:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 3:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 4:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 5:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 6:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 7:
    if (q7 == 0)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(1000);
      q7++;
    }
    else if (q7 ==1)
    {
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      q7++;
    }
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 8:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 9:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 10:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 11:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 12:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 13:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 15:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 16:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(500);
    break;
  case 17:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 18:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 19:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 20:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 21:
    motor.CCLKWTURN(50);
    motor2.CLKWTURN(50);
    delay(1000);
    break;
  case 22:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 23:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 24:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 25:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 26:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 27:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 28:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 29:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(500);
    break;
  case 30:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  }
}
*/
#pragma endregion

#pragma region YuksuzSeneryo 4
/*
int q7 = 0;
void qrKarar(int gelenDeger)
{
  switch (gelenDeger)
  {
  case 1:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 2:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 3:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 4:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 5:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 6:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 7:
    if (q7 == 0)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(1000);
      q7++;
    }
    else if ( q7==1)
    {
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      delay(3000);
      q7++;
    }
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 8:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 9:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 10:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 11:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 12:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 14:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 15:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(200);
    break;
  case 16:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 17:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 18:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 19:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 20:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 21:
    motor.CCLKWTURN(50);
    motor2.CLKWTURN(50);
    delay(1000);
    break;
  case 22:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 23:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 24:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 25:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 26:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 27:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 29:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  }
}
*/
#pragma endregion

#pragma region YukluSenoryo 1
/*
int q22 = 0;
int q42 = 0;
int q41 = 0;
int q7 = 0;
int q9 = 0;
int q11 = 0;
int q24 = 0;
int q20 = 0;
int q33 = 0;
int q38 = 0;
int q45 = 0;
int q50 = 0;

void qrKarar(int gelenDeger)
{
  switch (gelenDeger)
  {
  case 1:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    motor.CCLKWTURN(172);
    motor2.CLKWTURN(50);
    delay(500);
    break;
  case 2:

    break;

  case 3:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    motor.CCLKWTURN(172);
    motor2.CLKWTURN(50);
    delay(500);

    break;

  case 4:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;

  case 5:

    break;

  case 6:

    break;
  case 7:
    if (q7 == 1)
    {
      q7++;
    }
    else if (q7 == 2)
    {

      q7++;
    }
    break;
  case 8:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    motor.CCLKWTURN(172);
    motor2.CLKWTURN(50);
    delay(500);
    break;
  case 9:
    if (q9 == 0)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(2000);
      q9++;
    }
    else if (q9 == 1)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(1000);
      q9++;
    }
    break;
  case 10:

    break;

  case 11:
  {
    if (q11 == 0)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(1000);
      motor.CLKWTURN(50);
      motor2.CCLKWTURN(172);
      delay(500);
      q11++;
    }
    else if (q11 == 1)
    {

      q11++;
    }
    break;

  case 18:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 19:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 20:
    if (q20 == 0)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(1000);
      q20++;
    }
    else if (q20 == 1)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(1000);
      motor.CLKWTURN(172);
      motor2.CLKWTURN(50);
      delay(500);
    }

    break;
  case 21:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 22:

    if (q22 == 0)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(3000);
      q22++;
    }
    else if (q22 == 1)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(1000);
      q22++;
    }
    else if (q22 == 2)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(1000);
      q22++;
    }
    else if (q22 == 3)
    {
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      delay(1000);
      q22++;
    }

    break;
  case 23:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 24:
    if (q24 == 0)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(1000);
      motor.CLKWTURN(50);
      motor2.CCLKWTURN(172);
      delay(500);
      q24++;
    }
    else if (q24 == 1)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(2000);
    }

  case 25:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(2000);
    break;
  case 26:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 27:

    break;
  case 35:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    motor.CLKWTURN(50);
    motor2.CCLKWTURN(172);
    delay(500);
    break;
  case 33:
    if (q33 == 0)
    {
      motor.CCLKWTURN(50);
      motor.CCLKWTURN(50);
      delay(500);
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      motorKirko.CCLKWTURN(200);
      delay(10000);
      q33++;
    }
    else if (q33 == 1)
    {
      motor.CCLKWTURN(50);
      motor.CCLKWTURN(50);
      delay(500);
      q33++;
    }
    break;
  case 31:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 36:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 38:
    if (q38 == 0)
    {
      motor.CCLKWTURN(50);
      motor.CCLKWTURN(50);
      delay(500);
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      motorKirko.CLKWTURN(200);
      delay(10000);
    }
    else if (q38 == 1)
    {
      motor.CCLKWTURN(50);
      motor.CCLKWTURN(50);
      delay(500);
      q38++;
    }
    break;
  case 39:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 40:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    motor.CCLKWTURN(172);
    motor2.CLKWTURN(50);
    break;
  case 41:
    if (q41 == 0)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(700);
      motor.CLKWTURN(50);
      motor2.CCLKWTURN(172);
      delay(1000);
      q41++;
    }
    else if (q41 == 1)
    {
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(50);
      delay(700);
      motor.CLKWTURN(50);
      motor2.CCLKWTURN(172);
      delay(500);
      q41++;
    }
    else if (q41 == 2)
    {
      motor.CCLKWTURN(50);
      motor.CCLKWTURN(50);
      delay(200);
      q41++;
    }
    break;
    //---------------------------
  case 42:
    if (q42 == 1)
    {
      q42++;
    }
    else if (q42 == 2)
    {
      motor.CCLKWTURN(50);
      motor.CCLKWTURN(50);
      delay(800);
      q42++;
    }
    break;
  case 45:
    if (q45 == 0)
    {
      motor.CCLKWTURN(50);
      motor.CCLKWTURN(50);
      delay(500);
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      motorKirko.CLKWTURN(200);
      delay(10000);
      q45++;
    }
    else if (q45 == 1)
    {
      motor.CCLKWTURN(50);
      motor.CCLKWTURN(50);
      delay(500);
      q45++;
    }
    break;
  case 47:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    motor.CCLKWTURN(172);
    motor2.CLKWTURN(50);
    delay(500);
    break;
  case 48:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 50:
    if (q50 == 0)
    {
      motor.CCLKWTURN(50);
      motor.CCLKWTURN(50);
      delay(500);
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      motorKirko.CCLKWTURN(200);
      delay(10000);
      q50++;
    }
    else if (q50 == 1)
    {
      motor.CCLKWTURN(50);
      motor.CCLKWTURN(50);
      delay(500);
      q50++;
    }
    break;
  case 51:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    break;
  case 52:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    delay(1000);
    motor.CLKWTURN(50);
    motor2.CCLKWTURN(172);
    delay(500);
    break;
  }
  }
}
*/
#pragma endregion

int EngelKontrol()
{
  int x = 0;
  for (int i = 0; i < onOptikSayisi; i++)
  {
    x += onOptikler[i].MZ80_OKU();
  }
  return x;
}

#pragma region Otonom Hareket

void OtonomHareket()
{
  if (Serial.available() > 0)
  {
    String data = Serial.readStringUntil('\n');
    switch (data[0])
    {
    case 'N':
      motor.CCLKWTURN(25);
      motor2.CCLKWTURN(25);
      break;
    case 'R':
      motor.CCLKWTURN(50);
      motor2.CCLKWTURN(25);
      break;
    case 'L':
      motor.CCLKWTURN(25);
      motor2.CCLKWTURN(50);
      break;
    case 'X':
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      break;
    default:
      motor.CCLKWTURN(0);
      motor2.CCLKWTURN(0);
      break;
    }
  }
}
#pragma endregion

#pragma region Sarj
int Sarj()
{
  for (int i = 0; i < 30; i++)
  {
    float okunanA = analogRead(A0);
    float okunanB = analogRead(A2);
    vA = okunanA * 5 / 1024;
    vB = okunanB * 5 / 1024;
    vA = vA * ((RA + RB + RC) / 20);
    vB = vB * ((RA + RB + RC) / 10);
    vOrtalama = (vA + vB) / 2;
    vToplam = vToplam + vOrtalama;
    float v = vToplam / 30;
    int yuzde = map(v, 23.5, 25, 0, 100); // yüzde olarak veriyor
    Serial.println(yuzde);
  }
}
#pragma endregion

#pragma region Sagled

void Sag_led()
{
  for (int i = 0; i <= 35; i++)
  {
    leds[i] = sagrenk;
  }
  FastLED.show();
}
#pragma endregion Sagled

#pragma region Arkaled
void Arka_led()
{ // 36-45
  for (int i = 36; i <= 45; i++)
  {
    leds[i] = arkarenk;
  }
  FastLED.show();
}

#pragma endregion Arkaled

#pragma region Solled

void Sol_led()
{ // 46-81
  for (int i = 46; i <= 81; i++)
  {
    leds[i] = solrenk;
  }
  FastLED.show();
}

#pragma endregion Solled

#pragma region Onled
void On_led()
{ // 82- 91
  for (int i = 82; i <= 91; i++)
  {
    leds[i] = onrenk;
  }
  FastLED.show();
}

#pragma endregion Onled

#pragma region ustsag
void ust_sag()
{ // 92 - 114
  for (int i = 92; i <= 114; i++)
  {
    leds[i] = ustsagrenk;
  }
  FastLED.show();
}
#pragma endregion ustsag

#pragma region uston

void Ust_on()
{ // 115 - 145
  for (int i = 115; i <= 145; i++)
  {
    leds[i] = ustonrenk;
  }
  FastLED.show();
}

#pragma endregion uston

#pragma region ustsol
void ust_sol()
{ // 146 - 168
  for (int i = 146; i <= 168; i++)
  {
    leds[i] = ustsolrenk;
  }
  FastLED.show();
}
#pragma endregion ustsol

#pragma region ustarka
void Ust_arka()
{ // 169 - 199
  for (int i = 169; i <= 199; i++)
  {
    leds[i] = ustarkarenk;
  }
  FastLED.show();
}
#pragma endregion ustarka