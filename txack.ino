// TxAck verici kod.
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

// Adres
const byte slaveAddress[5] = {'R','x','A','A','A'};
// Radyoyu oluştur.
RF24 txTrans(CE_PIN, CSN_PIN);

// mesaj
char dataToSend[10] = "mesaj";
char txNum = '0';
int ackData[2] = {-1, -1}; // alıcıdan gelen verileri kaydetmek için.
bool newData = false;

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // saniyede bir gönder.

void setup() {
    Serial.begin(9600);
    Serial.println("TxAck başlatildi.");

    txTrans.begin();
    // Veri hızını 250KBPS'e ayarla.
    txTrans.setDataRate(RF24_250KBPS);
    // ACK sistemini aç.
    txTrans.enableAckPayload();
    // Tekrar sayısı ve her tekrar arasındaki süre(mikrosaniye).
    txTrans.setRetries(5,5); // delay, count
    // 5 gives a 1500 µsec delay which is needed for a 32 byte ackPayload
    txTrans.openWritingPipe(slaveAddress);
}

void loop() {
    // Başlangıçta geçen zamanı son verinin gönderildiği zamandan çıkararak planlanmış 1sn'lik süreyi beklemesini sağlar.
    currentMillis = millis();
    // Eğer süre geçtiyse gönderme işlemine başla.
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
    }
    // Gönderme işlemi sonrasında verileri göster.
    showData();
}

void send() {
    bool rslt;
    // Gönderme işlemi sonrası sonuç.
    rslt = txTrans.write(&dataToSend, sizeof(dataToSend));

    Serial.print("Veri gönderildi: ");
    Serial.print(dataToSend);
    // Eğer gönderim başarılı ise;
    if (rslt) {
        // ACK verisi var mı?
        if (txTrans.isAckPayloadAvailable()) {
            // Var ise gelen veriyi kaydet.
            txTrans.read(&ackData, sizeof(ackData));
            // Yeni veri bulundu.
            newData = true;
        }
        else {
            Serial.println("Kabul edildi fakat veri yok.");
        }
        updateMessage();
    }
    else {
        Serial.println("Gönderici başarisiz.");
    }
    prevMillis = millis();
 }

void showData() {
    if (newData == true) {
        Serial.print("Veri farkedildi.");
        Serial.print(ackData[0]);
        Serial.print(",");
        Serial.println(ackData[1]);
        Serial.println();
        newData = false;
    }
}

void updateMessage() {
    // Gelen yeni veriyi görebilmek için. 
    txNum += 1;
    if (txNum > '9') {
        txNum = '0';
    }
    dataToSend[8] = txNum;
}