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
#define MZPIN 32
#define MZPIN1 34
#define MZPIN2 36
#define MZPIN3 38
#define MZPIN4 40
#pragma endregion

#pragma region Qrkod
// qrKod Değerleri
SoftwareSerial mySerial(41, 42);
char qrGelen = '0'; // Kaydedilcek Değiken Yeri
int k = 0;
char a[] = {"00"}; // char* a ile aynı deger
char *qrBeklenenler[] = {"1;", "2;", "3;", "4;", "5;", "6;", "7;", "8;", "9;", "10", "11", "12", "13",
                         "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26",
                         "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
                         "40", "41", "42", "43", "45", "46", "47", "48", "49", "50", "51", "52"};
#pragma endregion

#pragma region Optik Sensör Sayisi
const int onOptikSayisi = 3;
const int yanOptikSayisi = 2;
const int alinanVeriSayisi = 4;
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
MZ80 optik(MZPIN);
// sol optik
MZ80 optik1(MZPIN1);
// ön sağ optik
MZ80 optik2(MZPIN2);
// ön orta optik
MZ80 optik3(MZPIN3);
// ön sol optik
MZ80 optik4(MZPIN4);
#pragma endregion
#pragma region Motor Constructorlari
BTS7960B motor(RPWM, LPWM);    // sağ
BTS7960B motor2(RPWM2, LPWM2); // sol
BTS7960B motorKirko(RPWM3, LPWM3);
#pragma endregion
#pragma region MZ80 Arrayleri
MZ80 onOptikler[onOptikSayisi] = {
    optik,
    optik1,
    optik2};
MZ80 yanOptikler[yanOptikSayisi] = {
    optik,
    optik1,
};
#pragma endregion
#pragma region Motor Arrayleri
BTS7960B motorlar[3]{
    motor,
    motor2,
    motorKirko};
#pragma endregion

#pragma region Neopixel Degiskenler
#define NUM_LEDS 82    // Kullanılan Led Sayısı
#define DATA_PIN 4     // Hangi Pine Bağlancağı
#define BRIGHTNES 128  // Parlaklık Ayarı
#define CHIPSET WS2812 // Ledin Modeli
CRGB leds[NUM_LEDS];   // Kütüphane Fonksiyonu
#pragma endregion
#pragma region Gyro Degiskenler
const int MPU_addr = 0x68;                 // Sensörün Adresi
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; // Okumakta Kullanılcak Olan Değişkenler
int minVal = 265;                          // Değerler
int maxVal = 402;
double xEkseni, yEkseni, zEkseni; // Değerlerin Kaydedildiği Değişkenler
#pragma endregion
void setup()
{
  Serial.begin(9600);   // Seri Haberleşme Başlar
  mySerial.begin(9600); // Seri Kanal Açılır
  // radyo = radyoModulu.nRF24AliciKurulum(radyo, RF24_PA_HIGH, 9600, RF24_250KBPS); // Radyo Frekans Değeri

#pragma region Gyro Setup
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
#pragma endregion

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop()
{
  OtonomHareket();
  // Radyodan veriyi alır.
  /*radyoModulu.nRF24VeriAl(radyo, alinanVeri, 4);
  switchDurumu = alinanVeri[3];
  // Engel gorulmedigi surece kontrole devam et, eger engel soz konusu ise dur.
  if (EngelKontrol() == 0)
  {
    Kontrol();
  }
  else
  {
    Serial.println("engel var hocam");
    motor.CCLKWTURN(0);
    motor2.CCLKWTURN(0);
  }*/

#pragma region GyroOku
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);
  xEkseni = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  yEkseni = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  zEkseni = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
// Gyro değerlerini okuyor ve açısal değerlere çeviriyor(xEkseni)
#pragma endregion
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
    // Serial.println("Hareket modu");
    Hareket();
  }
  else
  {
    // Serial.println("Kriko modu");
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
      // Serial.print("motor " + i); Serial.println(" ileri");
      motorlar[i].CCLKWTURN(map(alinanVeri[i], sabitDeger[i], 255, 0, 255));
    }
    else if (alinanVeri[i] < sabitDeger[i])
    {
      // Serial.print("motor" + i); Serial.println(" geri");
      motorlar[i].CLKWTURN(map(alinanVeri[i], 0, sabitDeger[i], -255, 0) * -1);
    }
  }
}

void KrikoHareket()
{
  if (alinanVeri[1] > sabitDeger[1])
  {
    // Serial.print("motor " + 2); Serial.println(" yukari");
    motorlar[2].CCLKWTURN(map(alinanVeri[1], sabitDeger[1], 255, 0, 255));
  }
  else if (alinanVeri[1] < sabitDeger[1])
  {
    // Serial.print("motor " + 2); Serial.println(" asagi");
    motorlar[2].CLKWTURN(map(alinanVeri[1], 0, sabitDeger[1], -255, 0) * -1);
  }
  else if (alinanVeri[1] == sabitDeger[1])
  {
    motorlar[2].CCLKWTURN(0);
    motorlar[2].CLKWTURN(0);
  }
}
#pragma endregion

#pragma region Qr Kod Fonksiyonlari
int qrKodKontrol()
{
  // Qr Sensörü Okuyor Ve qrGelen Değişkine Kaydediyor
  if (mySerial.available())
  {
    qrGelen = mySerial.read();
    a[k] = qrGelen;
    k++;
    // Serial.print(qrGelen);
  }
  else
  {
    for (int i = 0; i < sizeof(qrBeklenenler) / sizeof(qrBeklenenler[0]); i++)
    {
      int n = memcmp(a, qrBeklenenler[i], sizeof(a));
      if (n)
      {
        return i + 1;
      }
    }
  }
}

void qrKarar(int gelenDeger)
{
  switch (gelenDeger)
  {
  case 5:
    motorlar[2].CCLKWTURN(255);
    delay(5000);
  }
}

int EngelKontrol()
{
  int x = 0;
  for (int i = 0; i < onOptikSayisi; i++)
  {
    x += onOptikler[i].MZ80_OKU();
  }
  return x;

  if (x != 0)
  {
    while (xEkseni != 90) // 90'a eşit olana kadar yapıyor.
    {
      motor.CLKWTURN(31);
      motor2.CCLKWTURN(100); // robot belirli bir hızda sağa dönüyor
    }
  }
  if (yanOptikler[1].MZ80_OKU() == 1) // robotun sol tarafındaki optik engel algılamayı kesene kadar ileri gidyor
  {
    motor.CCLKWTURN(100);
    motor2.CCLKWTURN(100);
  }
  if (yanOptikler[1].MZ80_OKU() == 0)
  {
    while (xEkseni != 0) // robot engel algılamadığı için sola dönüyor ve çizgiye giriyor
    {
      motor2.CLKWTURN(31);
      motor.CCLKWTURN(100);
    }
    motor.CCLKWTURN(100);
    motor2.CCLKWTURN(100);
    delay(2000);
  }
}

#pragma region NeoPixel
void NeoPixel()
{
}
#pragma endregion

#pragma region Otonom Kontrol
const char *PiVerisiOku()
{
  if (Serial.available() > 0)
  {
    Serial.println("Serial available");
    String data = Serial.readStringUntil('\n');
    Serial.print("You sent me");
    Serial.println(data);
    const char *dataC = data.c_str();
    return dataC;
  }
  else
  {
    Serial.println("Serial not available");
    return " ";
  }
}

void OtonomHareket()
{
  const char *val = PiVerisiOku();
  Serial.println("Data read");
  switch (val[0])
  {
  case 'N':
    /*motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);*/
    Serial.println("Ileri");
  case 'R':
    /*motor.CCLKWTURN(75);
    motor2.CCLKWTURN(25);*/
    Serial.println("Saga");
  case 'L':
    /*motor.CCLKWTURN(25);
    motor2.CCLKWTURN(75);*/
    Serial.println("Sola");
  case 'X':
    /*motor.CCLKWTURN(0);
    motor2.CCLKWTURN(0);*/
    Serial.println("Cizgi gorunmuyor");
  default:
    /*motor.CCLKWTURN(0);
    motor2.CCLKWTURN(0);*/
    Serial.println("Hatali");
  }
}
#pragma endregion
