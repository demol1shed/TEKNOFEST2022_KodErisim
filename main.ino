#include <nRF24.h>
#include <SPI.h>
#include <RF24.h>
#include <BTS7960B.h>


#define RPWM 13
#define LPWM 12
#define RPWM2 11
#define LPWM2 10

// ALICI PARÇA
BTS7960B motor(RPWM, LPWM);
BTS7960B motor2(RPWM2, LPWM2);
// RF24 radio(CE, CSN);
// nRF24 radioModule;
int durum = 0;

void setup(){
  Serial.begin(9600);
  // radio = radioModule.nRF24ReceiverSetup(radio, RF24_PA_MIN, 9600, RF24_250KBPS);
}

void loop(){
  // radioModule.nRF24Receive(radio);
  //Serial.println(radioModule.receivingValues[0]);
  unsigned long started = millis();
  while(millis() - started < 2000){
    motor.CCLKWTURN(255);
    motor2.CCLKWTURN(255);
    //Serial.println("cclockwise");
    //Serial.println("cclockwise");
  }
  unsigned long started2 = millis();
  while(millis() - started2 < 5000){
    motor.CLKWTURN(255);
    motor2.CLKWTURN(255);
  }
}

/*// Inclusão das Bibliotecas
#include <Wire.h>
#include "U8glib.h"
//#include <logos.h>

//#include <joystick.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 u8g1(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 u8g2(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);


char bufferX [20];
char bufferY [20];
char bufferZ [20];



const int pinAddA = 5;
const int pinAddB = 6;
const int pinAddC = 7;

void setup() {

  pinMode(pinAddA, OUTPUT);
  pinMode(pinAddB, OUTPUT);
  pinMode(pinAddC, OUTPUT);

  Wire.begin();

}

void loop() {
  selDisp1();

  sprintf(bufferX, "Robolink Akademi");
  sprintf(bufferY, "sik");
  sprintf(bufferZ, "Example");
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );

  selDisp2();
  sprintf(bufferX, "Osman Ersahin");
  sprintf(bufferY, "durum");
  sprintf(bufferZ, "    sadasfasf");
  u8g1.firstPage();
  do {
    draw1();
  } while ( u8g1.nextPage() );

  selDisp3();

  sprintf(bufferX, "iklim asoifhasf");
  sprintf(bufferY, "2022");
  sprintf(bufferZ, "    fgege");
  u8g2.firstPage();
  do {
    draw2();
  } while ( u8g2.nextPage() );
}

void selDisp1() {
  digitalWrite(pinAddA, LOW);
  digitalWrite(pinAddB, LOW);
  digitalWrite(pinAddC, LOW);
}

void selDisp2() {
  digitalWrite(pinAddA, HIGH);
  digitalWrite(pinAddB, LOW);
  digitalWrite(pinAddC, LOW);
}
void selDisp3() {
  digitalWrite(pinAddA, LOW);
  digitalWrite(pinAddB, HIGH);
  digitalWrite(pinAddC, LOW);
}

void draw(void)
{


  u8g.setFont(u8g_font_gdr10);
  u8g.drawStr( 0, 40, bufferX);
  u8g.drawStr( 0, 20, bufferY);
  u8g.drawStr( 0, 60, bufferZ);
}

void draw1(void)
{


  u8g1.setFont(u8g_font_gdr10);
  u8g1.drawStr( 0, 40, bufferX);
  u8g1.drawStr( 0, 20, bufferY);
  u8g1.drawStr( 0, 60, bufferZ);
}

void draw2(void)
{


  u8g2.setFont(u8g_font_gdr10);
  u8g2.drawStr( 0, 40, bufferX);
  u8g2.drawStr( 0, 20, bufferY);
  u8g2.drawStr( 0, 60, bufferZ);
}*/
#pragma region ekran
/*#include <ekran.h>
#include "U8glib.h"

const int ekranSayisi = 3;
const char* mesajlar[3] = {"kod", "erisim", "kod erisim"};
byte durumlar[3][ekranSayisi] = {{LOW,LOW,LOW}, {HIGH,LOW,LOW}, {LOW,HIGH,LOW}};
OledEkran ekranModul(0, 20);

U8GLIB_SSD1306_128X64 ekran(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 ekran1(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);


void setup(){
  ekranModul.SetupPins();
  // flip screen, if required
  ekran.setRot180();
  ekran1.setRot180();
}

void loop(){
  ekranModul.SetDisp(durumlar[1]);
  ekranModul.WriteToScreen(ekran, mesajlar);
  ekranModul.SetDisp(durumlar[0]);
  ekranModul.WriteToScreen(ekran1, mesajlar);
}*/
#pragma endregion