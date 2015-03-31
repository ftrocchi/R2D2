// To upload, select:
//     Tools->Board->Arduino Mini
//     Tools->Processor->Atmega328
//     Tools->Programmer->USBtinyISP
//     File->Upload Using Programmer

#include "I2C_Common.h"
#include <Wire.h>

#define REDPIN 10
#define GREENPIN 11
#define BLUEPIN 9

#define HP 1 // 1=Front(BLUE), 2=TOP(Red), 3=Rear(Green);
#define STARTUPDELAY 3000

void setLed(I2C_HP_Color::Value color, int timeInMilliseconds = 0) {
    digitalWrite(REDPIN, color & bit(0));
    digitalWrite(GREENPIN, color & bit(1));
    digitalWrite(BLUEPIN, color & bit(2));
    
    if (timeInMilliseconds > 0) {
        delay(timeInMilliseconds);
        setLed(I2C_HP_Color::Off);
    }
}


void setup() {
    pinMode(REDPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);
    
    switch(HP) {
        case 1:
            Wire.begin(I2C_DeviceAddress::FrontHP);
            setLed(I2C_HP_Color::Blue, STARTUPDELAY);
            break;
            
        case 2:
            Wire.begin(I2C_DeviceAddress::TopHP);
            setLed(I2C_HP_Color::Red, STARTUPDELAY);
            break;
            
        case 3:
            Wire.begin(I2C_DeviceAddress::RearHP);
            setLed(I2C_HP_Color::Green, STARTUPDELAY);
            break;
    }
    
    Wire.onReceive(receiveEvent);
}

void loop() {
}

void receiveEvent(int eventCode) {
    I2C_HP_Mode::Value mode = (I2C_HP_Mode::Value)Wire.read();
    
    switch (mode) {
        case I2C_HP_Mode::Color:
            I2C_HP_Color::Value color = (I2C_HP_Color::Value)Wire.read();
            setLed(color);
            break;
    }
}


