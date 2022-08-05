/*
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
int k;
char qrGelen = '0';
char *a;
void setup()
{
    Serial.begin(9600);
    mySerial.begin(9600);
}
void loop()
{

    if (mySerial.available())
    {
        while (mySerial.available())
        {
            qrGelen = mySerial.read();
            a[k] = qrGelen;
            k++;
        }
        Serial.write(a);
        delay(5);
    }
}
*/