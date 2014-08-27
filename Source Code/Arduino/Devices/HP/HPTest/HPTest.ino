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
        sendToHP(21);
        delay(500);
        sendToHP(20);
        delay(500);
    }
    
    // alarm in blue
    sendToHP(5);
    sendToHP(22);
    delay(5000);
    
    // alarm in red
    sendToHP(3);
    delay(5000);
    
}

void sendToHP(byte command)
{
    Wire.beginTransmission(25);
    Wire.write(command);
    Wire.endTransmission();
}
