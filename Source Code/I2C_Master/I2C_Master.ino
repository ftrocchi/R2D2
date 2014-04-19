#include <Wire.h>
#include <Sabertooth.h>
#include <SendOnlySoftwareSerial.h>
#include "I2C_Common.h"

SendOnlySoftwareSerial syren(11);
Sabertooth ST(128, syren);

#define MAX_ARRAY_LENGTH 20
byte gamePadState[MAX_ARRAY_LENGTH];

void setup()
{
    syren.begin(9600);
    ST.autobaud();

    Wire.begin();
}

void loop()
{
    ProcessPS2();
    
    // take the left stick x axis value / 2 and send to the motor
    int motorValue = gamePadState[0] - 128;
    
    ST.motor(motorValue);

    delay(20);
}

void ProcessPS2()
{
    Wire.requestFrom(I2C_DeviceAddress::PS2, 20);
    
    int index=0;
    while (Wire.available())
    {
        gamePadState[index] = Wire.read();
        index++;
    }
}
