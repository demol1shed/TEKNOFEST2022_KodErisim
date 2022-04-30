
//#include <MFRC522.h>
//#include <SPI.h>

/*int rst = 3;
int sda = 53;
boolean izin = true;
MFRC522 rfid(sda, rst);
byte ID[4] = {99, 130, 23, 12};

void setup() {


  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();



}

void loop() {


  if (rfid.PICC_IsNewCardPresent()) {
    if (rfid.PICC_ReadCardSerial()) {
      Serial.println("Kart Tespit Edildi-Kart ID: ");
      Serial.print(rfid.uid.uidByte[0]);
      Serial.print(",");
      Serial.print(rfid.uid.uidByte[1]);
      Serial.print(",");
      Serial.print(rfid.uid.uidByte[2]);
      Serial.print(",");
      Serial.println(rfid.uid.uidByte[3]);

    }
    int x = 0;
    for(int i = 0; i < 4; i++){
      if(ID[i] == rfid.uid.uidByte[i]){
        x++;
      }
    }

    if(x == 4){
      Serial.println("Elementler doğrulandı");
    }else{
      Serial.println("Elementler doğrulanamadı");
    }

    rfid.PICC_HaltA();

  }














}*/
