#include <Wire.h>

void setup()
{
    Wire.begin();
}

void loop()
{
    for (int i=1; i<9; i++)
    {
        sendToHP(i);
        delay(500);
    }
}

void sendToHP(byte command)
{
    Wire.beginTransmission(25);
    Wire.write(command);
    Wire.endTransmission();
}
