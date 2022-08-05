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

#define Role 42

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
#pragma region Gyro Degiskenler
const int MPU_addr = 0x68;                 // Sensörün Adresi
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; // Okumakta Kullanılcak Olan Değişkenler
int minVal = 265;                          // Değerler
int maxVal = 402;
double xEkseni, yEkseni, zEkseni, Egim; // Değerlerin Kaydedildiği Değişkenler
int buzzer = 22;
#pragma endregion

void setup()
{
  Serial.begin(9600); // Seri Haberleşme Başlar
  radyo = radyoModulu.nRF24AliciKurulum(radyo, RF24_PA_HIGH, RF24_250KBPS); // Radyo Frekans Değeri
  mySerial.begin(9600); // Seri Kanal Açılır
  pinMode(buzzer, OUTPUT);
#pragma region Gyro Setup
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
#pragma endregion

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

char a[] = {"Q1;"}; // char* a ile aynı deger
void loop()
{

  /*qrKodKontrol();
  int deger = qrOku();
  qrKarar(deger);

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

  Gyro();*/
  if (Sarj() <= 12)
  {
    ustsagrenk = CRGB::OrangeRed;
    ust_sag();
  }
  Gyro();

  //  Radyodan veriyi alır.
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
  Egim = map(zEkseni, 0, 90, 0, 100);
  // Gyro değerlerini okuyor ve açısal değerlere çeviriyor(xEkseni)
}

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
#pragma region Senaryo 1
void qrKarar(int gelenDeger)
{
  switch (gelenDeger)
  {
  case 1:

    break;
  case 2:

    break;
  case 3:

    break;

  case 4:

    break;
  case 5:

    break;
  case 6:

    break;
  case 7:

    break;
  case 8:

    break;
  case 9:

    break;
  case 10:

    break;
  case 11:

    break;
  case 12:

    break;
  case 13:

    break;
  case 14:

    break;
  case 15:

    break;

  case 16:

    break;
  case 17:

    break;
  case 18:

    break;
  case 19:

    break;
  case 20:

    break;
  case 21:

    break;
  case 22:
    motor.CCLKWTURN(175);
    motor2.CLKWTURN(50);
    break;
  case 23:
    motor.CCLKWTURN(50);
    motor2.CCLKWTURN(50);
    break;
  case 24:

    break;
  case 25:

    break;
  case 26:

    break;
  case 27:

    break;
  case 28:

    break;
  case 29:

    break;
  case 30:

    break;
  case 31:

    break;
  case 32:

    break;
  }
}

/**
 * @brief MZ80leri tarar, degerlerini kaydeder.
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
  int yuzde = 0;
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
    yuzde = map(v, 23.5, 25, 0, 100); // yüzde olarak veriyor
  }
  return yuzde;
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