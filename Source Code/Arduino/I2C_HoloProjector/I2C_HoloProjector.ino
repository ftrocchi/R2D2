// To upload, select:
//     Tools->Board->Arduino Mini
//     Tools->Processor->Atmega328
//     Tools->Programmer->USBtinyISP
//     File->Upload Using Programmer

#include "I2C_Common.h"
#include <Wire.h>
#include <Servo.h>

#define REDPIN 10
#define GREENPIN 11
#define BLUEPIN 9

// 1=Front(BLUE), 2=TOP(Red), 3=Rear(Green);
#define HP 3

// Amount of time to show the color designation for the HP
#define STARTUPDELAY 3000

// servo mins and max
#define SERVOMIN 20
#define SERVOMAX 150

Servo hp1Servo;
Servo hp2Servo;
bool isTwitchOn = true;
unsigned long currentTime;
unsigned long endTime;

void setLed(I2C_HP_Color::Value color, int timeInMilliseconds = 0) {
    digitalWrite(REDPIN, color & bit(0));
    digitalWrite(GREENPIN, color & bit(1));
    digitalWrite(BLUEPIN, color & bit(2));

    if (timeInMilliseconds > 0) {
      delay(timeInMilliseconds);
      setLed(I2C_HP_Color::Off);
    }
}

void setServo1(int value) {
    hp1Servo.write(clamp(value, SERVOMIN, SERVOMAX));
}

void setServo2(int value) {
    hp2Servo.write(clamp(value, SERVOMIN, SERVOMAX));
}

void setup() {
    randomSeed(analogRead(0));
    
    pinMode(REDPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);

    hp1Servo.attach(3);
    hp2Servo.attach(5);

    switch (HP) {
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
    processTwitch();
}

void processTwitch() {
    if (isTwitchOn) {
        // get the current time
        currentTime = millis();

        if (endTime < currentTime) {  
            // twitch here 
            setServo1(random(180));
            setServo2(random(180));

            // set new end time
            endTime = currentTime + (random(5000)) + 3000;
        }
    }
}

//------------------------------------------------------------------------------
// I2C
//------------------------------------------------------------------------------
// Adding braces after case statements to fix 'jumping the case label'error - ugh
void receiveEvent(int eventCode) {
    I2C_HP_Mode::Value currentMode = (I2C_HP_Mode::Value)Wire.read();

    switch (currentMode) {
        case I2C_HP_Mode::Color:
        {
            I2C_HP_Color::Value color = (I2C_HP_Color::Value)Wire.read();
            setLed(color);
        }
        break;

        case I2C_HP_Mode::Servo:
        {
            I2C_HP_Servo::Value servo = (I2C_HP_Servo::Value)Wire.read();

            int hp1Value = 90;
            int hp2Value = 90;

            // set position values based on the enum
            switch (servo) {
                case I2C_HP_Servo::TopLeft: { hp1Value = 0; hp2Value = 0; } break;
                case I2C_HP_Servo::Top: { hp1Value = 90; hp2Value = 0; } break;
                case I2C_HP_Servo::TopRight: { hp1Value = 180; hp2Value = 0; } break;
                case I2C_HP_Servo::Left: { hp1Value = 0; hp2Value = 90; } break;
                case I2C_HP_Servo::Center: { hp1Value = 90; hp2Value = 90; } break;
                case I2C_HP_Servo::Right: { hp1Value = 180; hp2Value = 90; } break;
                case I2C_HP_Servo::BottomLeft: { hp1Value = 0; hp2Value = 180; } break;
                case I2C_HP_Servo::Bottom: { hp1Value = 90; hp2Value = 180; } break;
                case I2C_HP_Servo::BottomRight: { hp1Value = 180; hp2Value = 180; } break;
                case I2C_HP_Servo::MoveTo: { hp1Value = Wire.read(); hp2Value = Wire.read(); } break;
                case I2C_HP_Servo::TwitchOn: { isTwitchOn = true; } break;
                case I2C_HP_Servo::TwitchOff: { isTwitchOn = false; } break;
            }

            // move servo
            setServo1(clamp(hp1Value, SERVOMIN, SERVOMAX));
            setServo2(clamp(hp2Value, SERVOMIN, SERVOMAX));
        }
    }
}

//------------------------------------------------------------------------------
// HELPER FUNCTIONS
//------------------------------------------------------------------------------
int clamp(int value, int min, int max) {
    if (value < min) 
        return min;
    else if (value > max)
        return max;

    return value;
}



