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
    delay(2000);
    
    // alarm in dark purple using custom color
    sendToHP(9, 19, 14, 24);
    delay(2000);
	
    // leia
    sendToHP(23);
    delay(2000);
	
    // disco
    sendToHP(24);
    delay(2000);
	
    // failure
    sendToHP(25);
    delay(5000);
    
    // SERVO
    sendToHP(3);
    sendToHP(21);
    sendToHP(40);
    delay(500);
    sendToHP(41);
    delay(500);
    sendToHP(42);
    delay(500);
    sendToHP(41);
    delay(500);
    
    sendToHP(4);
    sendToHP(21);
    sendToHP(43);
    delay(500);
    sendToHP(44);
    delay(500);
    sendToHP(45);
    delay(500);
    sendToHP(44);
    delay(500);
    
    sendToHP(5);
    sendToHP(21);
    for (int i=50; i<59; i++)
    {
        sendToHP(i);
        delay(500);
    }
    
    for (int i=20;i<160;i++)
    {
        sendToHP(46, i);
        sendToHP(47, 160-i);
        delay(15);
    }
    
    for (int i=20;i<160;i++)
    {
        sendToHP(59,i,160-i);
        delay(15);
    }
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

void sendToHP(byte command, byte value)
{
    Wire.beginTransmission(25);
    Wire.write(command);
    Wire.write(value);
    Wire.endTransmission();
}

void sendToHP(byte command, byte x, byte y)
{
    Wire.beginTransmission(25);
    Wire.write(command);
    Wire.write(x);
    Wire.write(y);
    Wire.endTransmission();
}
