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
    
    // alarm in dark purple using custom color
    sendToHP(9, 19, 14, 24);
    delay(5000);
	
    // leia
    sendToHP(23);
    delay(5000);
	
    // disco
    sendToHP(24);
    delay(5000);
	
    // failure
    sendToHP(25);
    delay(5000);
}

void sendToHP(byte command)
{
    Wire.beginTransmission(25);
    Wire.write(command);
    Wire.endTransmission();
}

void sendToHP(byte command, byte red, byte green, byte blue)
{
    Wire.beginTransmission(25);
    Wire.write(command);
    Wire.write(red);
    Wire.write(green);
    Wire.write(blue);
    Wire.endTransmission();
}
