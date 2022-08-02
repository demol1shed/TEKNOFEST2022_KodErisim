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

#pragma region Optik Sensör Sayisi
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
BTS7960B motor(RPWM, LPWM);//sag
BTS7960B motor2(RPWM2, LPWM2);//sol
BTS7960B motorKirko(RPWM3, LPWM3);
#pragma endregion
#pragma region QrKod Constructoru
SoftwareSerial mySerial(41, 42);
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
#define NUM_LEDS 92    // Kullanılan Led Sayısı
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
  Serial.begin(9600); // Seri Haberleşme Başlar
  // radyo = radyoModulu.nRF24AliciKurulum(radyo, RF24_PA_HIGH, 9600, RF24_250KBPS); // Radyo Frekans Değeri
  mySerial.begin(9600); // Seri Kanal Açılır

#pragma region Gyro Setup
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
#pragma endregion

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

int veri;
void loop()
{
  /* Veri duzensiz sekilde geliyor, rastgele.
    Arduinoda birden fazla serial kullanilmasindan dolayi olusabilecek bir sorun oldugunu dusunuyorum
    Raspberry pi'da baslattigim program arduino'daki veriyi de yansitiyor
    Arduino nadiren veriyi isliyor, sayisiz resetler sonrasinda isleyebiliyor.
    Islemesi soz konusu oldugunda ise asd.py dosyasinda belirttigim if statement sorunu ile,
    bu main.ino dosyasinda switch case'te motorlara gonderilecek olan komutlar kullanilmiyor.
    Onun yerine robot kendi kendine ilerliyor ya da bazi durumlara sola gidiyor.
  */
  PiVerisiOku(veri);
  OtonomHareket(veri);

  qrKodKontrol();
  qrKarar(qrOku());
  // void Gyro();

  //  Radyodan veriyi alır.
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

void Gyro()
{
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
}

#pragma region Qrkod
// qrKod Değerleri
char qrGelen = '0'; // Kaydedilcek Değiken Yeri
int k = 0;
char a[] = {"00"}; // char* a ile aynı deger
#pragma endregion
void qrKodKontrol()
{
  // Qr Sensörü Okuyor Ve qrGelen Değişkine Kaydediyor
  if (mySerial.available())
  {
    qrGelen = mySerial.read();
    a[k] = qrGelen;
    k++;
    // Serial.print(qrGelen);
  }
}

int qrOku(){
  if(a[2] == ';'){
    Serial.print("; karakteri bulundu, returlenecek olan karakter: ");
    Serial.println(a[1]);
    return a[1] - '0';
  }else{
    Serial.println("; karakteri bulunamadi");
    // butun arrayi ara
    for(int i = 0; i < sizeof(a) / sizeof(a[0]); i++){
      // elementte Q karakterini bul
      if(a[i] == 'Q'){
        Serial.println("Q karakteri bulundu");
        // Q karakteri haricindeki butun elementleri basa sar 
        for(int j = i; j < (sizeof(a) / sizeof(a[0])) - 1;  j++){
          a[j] = a[j + 1];
        }
        i--;
      }
    }
    int gonderilecek;
    // a dizisini int'e donustur ve gonderilecek olan degiskene yaz
    for(int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
      gonderilecek = a[i] - '0';
    
    Serial.print("gonderiliyor: ");
    Serial.println(gonderilecek);
    return gonderilecek;
  }
}

void qrKarar(int gelenDeger)
{ 
  switch (gelenDeger)
  {
  case 0:
    Serial.print("anan");
  }
}

/**
 * @note x değerini 0a eşit yaptım DİKKAT
 *
 * @return int
 */
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
/**
 * @brief raspberry pi'dan gelen int degerine gore motorlarin hareketini saglar
 * @todo motorlarin karar sistemi bozuk, calistirir calistrmaz robot sola hareket etmeye basliyor.
 * @param val
 */
void OtonomHareket(int val){
  Serial.print("karar veriliyor: ");
  Serial.println(val);
  switch (val)
  {
  case 1:
    motor.CCLKWTURN(25);
    motor2.CCLKWTURN(25);
    break;
  case 2:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(25);
    break;
  case 3:
    motor.CCLKWTURN(25);
    motor2.CCLKWTURN(50);
    break;
  case 0:
    motor.CCLKWTURN(0);
    motor2.CCLKWTURN(0);
    break;
  default:
    motor.CCLKWTURN(0);
    motor2.CCLKWTURN(0);
    break;
  }
}

/**
 * @brief Raspberry pi'dan gelen verileri data degiskeninin adresine yazar, Serial.read'den gelen char veriyi integere donusturur.
 * 
 * @param data 
 */
void PiVerisiOku(int& data){
  if (Serial.available() > 0)
  {
    data = Serial.read() - '0';
  }
  delay(50);
}
#pragma endregion

#pragma region NeoPixel
void NeoPixel()
{
  if (onOptikler[0].MZ80_OKU() == 0 && onOptikler[1].MZ80_OKU() == 0 && onOptikler[2].MZ80_OKU() == 1)
  {

    for (int i = 35; i >= 0; i--)
    {
      leds[i] = CRGB(225, 70, 0);
      FastLED.show();
      delay(10);
    }

    for (int i = 0; i <= 35; i++)
    {
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(10);
    }

    for (int i = 35; i >= 0; i--)
    {
      leds[i] = CRGB(225, 70, 0);
      FastLED.show();
      delay(10);
    }

    for (int i = 0; i <= 35; i++)
    {
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(10);
    }
  }
  else if (onOptikler[0].MZ80_OKU() == 1 && onOptikler[1].MZ80_OKU() == 0 && onOptikler[2].MZ80_OKU() == 0)
  {

    for (int i = 35; i >= 0; i--)
    {
      leds[81 - i] = CRGB(225, 70, 0);
      FastLED.show();
      delay(10);
    }

    for (int i = 0; i <= 35; i++)
    {
      leds[81 - i] = CRGB::Black;
      FastLED.show();
      delay(10);
    }
    for (int i = 35; i >= 0; i--)
    {
      leds[81 - i] = CRGB(225, 70, 0);
      FastLED.show();
      delay(10);
    }

    for (int i = 0; i <= 35; i++)
    {
      leds[81 - i] = CRGB::Black;
      FastLED.show();
      delay(10);
    }
  }

  else if (onOptikler[0].MZ80_OKU() == 0 && onOptikler[1].MZ80_OKU() == 1 && onOptikler[2].MZ80_OKU() == 0)
  {

    for (int i = 35; i >= 0; i--)
    {
      leds[i] = CRGB(225, 70, 0);
      leds[81 - i] = CRGB(225, 70, 0);
      FastLED.show();
      delay(10);
    }

    for (int i = 0; i <= 35; i++)
    {
      leds[i] = CRGB::Black;
      leds[81 - i] = CRGB::Black;
      FastLED.show();
      delay(10);
    }

    for (int i = 35; i >= 0; i--)
    {
      leds[i] = CRGB(225, 70, 0);
      leds[81 - i] = CRGB(225, 70, 0);
      FastLED.show();
      delay(10);
    }

    for (int i = 0; i <= 35; i++)
    {
      leds[i] = CRGB::Black;
      leds[81 - i] = CRGB::Black;
      FastLED.show();
      delay(10);
    }
    for (int i = 83; i <= 92; i++)
    {

      leds[i] = CRGB::White;
      FastLED.show();
    }
  }
}
#pragma endregion

#pragma region Sarj
void Sarj()
{
  for (int i = 0; i < 30; i++)
  {
    float okunanA = analogRead(A12);
    float okunanB = analogRead(A13);
    vA = okunanA * 5 / 1024;
    vB = okunanB * 5 / 1024;
    vA = vA * ((RA + RB + RC) / 20);
    vB = vB * ((RA + RB + RC) / 10);
    vOrtalama = (vA + vB) / 2;
    vToplam = vToplam + vOrtalama;
    float v = vToplam / 30;
    int yuzde = map(v, 22, 25, 0, 100);
  }
}
#pragma endregion