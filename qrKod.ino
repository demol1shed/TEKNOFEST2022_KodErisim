
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

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
            char input = mySerial.read();
            Serial.write(input);
            delay(5);
        }
    }
}