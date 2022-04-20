#include <nRF24.h>
#include <SPI.h>
#include <RF24.h>
#include <U8glib.h>
#include <ekran.h>


#define CE 9
#define CSN 8
// ALICI PARÇA
const int ekranSayisi = 2;
unsigned int mesajlar[5];
byte durumlar[ekranSayisi][3] = {{LOW,LOW,LOW}, {HIGH,LOW,LOW}};
OledEkran ekranModul(0, 20);
U8GLIB_SSD1306_128X64 ekran(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 ekran1(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 ekranlar[ekranSayisi]{
  ekran, 
  ekran1
};
RF24 radio(CE, CSN);
nRF24 radioModule;

void setup(){
  radio = radioModule.nRF24ReceiverSetup(radio, RF24_PA_MIN, 9600, RF24_1MBPS);
}

void loop(){
  radioModule.nRF24Receive(radio);
  ekranModul.SetDisp(durumlar[0]);
  ekranModul.WriteToScreen(ekranlar[0], radioModule.receivingValues);
}


/*// Inclusão das Bibliotecas
#include <Wire.h>
#include "U8glib.h"
//#include <logos.h>

//#include <joystick.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 u8g1(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 u8g2(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
// Define e Configura Display

char bufferX [20];
char bufferY [20];
char bufferZ [20];


// Definição de Pinos
const int pinAddA = 5;
const int pinAddB = 6;
const int pinAddC = 7;

void setup() {
  
  // Inicializa pinos
  pinMode(pinAddA, OUTPUT);
  pinMode(pinAddB, OUTPUT);
  pinMode(pinAddC, OUTPUT);
  

  // Inicializa Displays no endereço 0x3C do I2C
  Wire.begin();

}

void loop() {
  selDisp1();

  //sprintf(bufferX, "Robolink Akademi");
  sprintf(bufferY, "kod");
  //sprintf(bufferZ, "    Example");
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );

  selDisp2();
 // sprintf(bufferX, "Osman Erşahin");
  sprintf(bufferY, "erisim");
  //sprintf(bufferZ, "    sadasfasf");
  u8g1.firstPage();
  do {
    draw1();
  } while ( u8g1.nextPage() );

  selDisp3();

 // sprintf(bufferX, "iklim asoıfhasf");
  sprintf(bufferY, "2022");
 // sprintf(bufferZ, "    fgege");
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
#include <U8glib.h>

const int ekranSayisi = 3;
const char* mesajlar[5] = {"kod", "erisim", "kod erisim"};
byte durumlar[3][ekranSayisi] = {{LOW,LOW,LOW}, {HIGH,LOW,LOW}, {LOW,HIGH,LOW}};
OledEkran ekranModul(0, 20);

U8GLIB_SSD1306_128X64 ekran(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 ekran1(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 ekran2(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
U8GLIB_SSD1306_128X64 ekranlar[ekranSayisi]{
  ekran, 
  ekran1, 
  ekran2
};

void setup(){
  ekranModul.SetupPins();
}

void loop(){
  for(int i = 0; i < ekranSayisi; i++){
    ekranModul.SetDisp(durumlar[i]);
    ekranModul.WriteToScreen(ekranlar[i], mesajlar);
  }
}*/
#pragma endregion

