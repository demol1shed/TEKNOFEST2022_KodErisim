/*#include <nRF24.h>
#include <SPI.h>
#include <RF24.h>
#include <BTS7960B.h>

#define CE 48
#define CSN 43
#define RPWM 13
#define LPWM 12
#define RPWM2 11
#define LPWM2 10

const int sabitDeger = 135;
const int sabitDeger2 = 127;

// ALICI PARÇA
BTS7960B motor(RPWM, LPWM);
BTS7960B motor2(RPWM2, LPWM2);
RF24 radio(CE, CSN);
nRF24 radioModule;

void setup(){
  Serial.begin(9600);
  //radio = radioModule.nRF24ReceiverSetup(radio, RF24_PA_LOW, 9600, RF24_250KBPS);
}*/



//void loop(){
  #pragma region main
  /*radioModule.nRF24Receive(radio);
  if(radioModule.receivingValues[0] == sabitDeger){
    motor.CLKWTURN(0);
    motor.CCLKWTURN(0);
  }
  if(radioModule.receivingValues[1] == sabitDeger2){
    motor2.CLKWTURN(0);
    motor2.CCLKWTURN(0);
  }
  if(radioModule.receivingValues[0] > sabitDeger){
    motor.CCLKWTURN(map(radioModule.receivingValues[0], sabitDeger, 255, 0, 255));
  }else if(radioModule.receivingValues[0] < sabitDeger){
    motor.CLKWTURN(map(radioModule.receivingValues[0], 0, sabitDeger, -255, 0) * -1);
  }
  if(radioModule.receivingValues[1] > sabitDeger2){
    motor2.CCLKWTURN(map(radioModule.receivingValues[1], sabitDeger2, 255, 0, 255));
  }else if(radioModule.receivingValues[1] < sabitDeger2){
    motor2.CLKWTURN(map(radioModule.receivingValues[1], 0, sabitDeger2, -255, 0) * -1);
  }*/
  #pragma endregion
//}

#pragma region ornekekran
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
#pragma endregion

#pragma region ekran
#include <ekran.h>
#include "U8glib.h"

const int ekranSayisi = 3;
const char* mesajlar[3] = {"kod", "erisim", "kod erisim"};
const char* mesajlar1[3] = {"kodA", "erisimA", "kod erisimA"};
const char* mesajlar2[3] = {"kodB", "erisimB", "kod erisimB"};
byte durumlar[3][ekranSayisi] = {{LOW,LOW,LOW}, {HIGH,LOW,LOW}, {LOW,HIGH,LOW}};
OledEkran ekranModul(0, 20);

U8GLIB_SSD1306_128X64 ekran(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 ekran1(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 ekran2(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);


void setup(){
  ekranModul.SetupPins();
  // flip screen, if required
  ekran1.setRot180();
  ekran2.setRot180();
}

void loop(){
 ekranGuncelle();
}

void ekranGuncelle(){
  ekranModul.SetDisp(LOW, LOW, LOW);
  sprintf(ekranModul.bufferX, mesajlar[0]);
  sprintf(ekranModul.bufferY, mesajlar[1]);
  sprintf(ekranModul.bufferZ, mesajlar[2]);
  ekran.firstPage();
  do {
    ekranModul.DrawString(ekran);
  } while ( ekran.nextPage() );
 
  ekranModul.SetDisp(HIGH, LOW, LOW);
  sprintf(ekranModul.bufferX, mesajlar1[0]);
  sprintf(ekranModul.bufferY, mesajlar1[1]);
  sprintf(ekranModul.bufferZ, mesajlar1[2]);
  ekran1.firstPage();
  do {
      ekranModul.DrawString(ekran1);
  } while ( ekran1.nextPage() ); 
  
  ekranModul.SetDisp(LOW, HIGH, LOW);
  sprintf(ekranModul.bufferX, mesajlar2[0]);
  sprintf(ekranModul.bufferY, mesajlar2[1]);
  sprintf(ekranModul.bufferZ, mesajlar2[2]);
  ekran2.firstPage();
  do {
      ekranModul.DrawString(ekran2);
  } while ( ekran2.nextPage() ); 
}
#pragma endregion