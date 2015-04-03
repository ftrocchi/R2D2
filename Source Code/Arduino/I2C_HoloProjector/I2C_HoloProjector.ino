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
#define HP 1

// Amount of time to show the color designation for the HP
#define STARTUPDELAY 3000

I2C_HP_Mode::Value currentMode = I2C_HP_Mode::Color;
I2C_SystemEvent::Value currentSystemEvent = I2C_SystemEvent::Off;
unsigned long systemEventStartTime;
unsigned long systemEventEndTime;
unsigned long systemEventCurrentTime;
unsigned long systemEventPhaseEndTime;
unsigned long systemEventPhase;

Servo servos[2];

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

  servos[0].attach(3);
  servos[1].attach(5);

  Wire.onReceive(receiveEvent);
}

void loop() {
  processSystemEvent();
}

//------------------------------------------------------------------------------
// LEIA MESSAGE
//------------------------------------------------------------------------------
void setupLeiaMessage() {
  // the top and rear HPs need to turn off
  if (HP == 2 || HP == 3)
    setLed(I2C_HP_Color::Off);

  // TODO - if it is front HP, lower the hp

  // set the end time
  systemEventEndTime = systemEventStartTime + 35000;
  systemEventPhase = 0;
  systemEventPhaseEndTime = 0;
}

void updateLeiaMessage() {
  // get the current time
  systemEventCurrentTime = millis();

  // if time has expired, turn off and exit
  if (systemEventCurrentTime > systemEventEndTime) {
    currentSystemEvent = I2C_SystemEvent::Off;
    return;
  }

  // if delay time has expired, change the color randomly
  if (systemEventPhaseEndTime < systemEventCurrentTime) {
    switch (systemEventPhase) {
      case 0:
        setLed(I2C_HP_Color::Cyan);
        systemEventPhaseEndTime = systemEventCurrentTime + random(1, 10) * 10;
        systemEventPhase = 1;
        break;

      case 1:
        setLed(I2C_HP_Color::White);
        systemEventPhaseEndTime = systemEventCurrentTime + random(1, 5) * 10;
        systemEventPhase = 2;
        break;

      case 2:
        setLed(I2C_HP_Color::Blue);
        systemEventPhaseEndTime = systemEventCurrentTime + random(4, 9) * 10;
        systemEventPhase = 0;
        break;
    }
  }
}

//------------------------------------------------------------------------------
// IMPERIAL MARCH
//------------------------------------------------------------------------------
void setupImperialMarch() {
  // set the end time
  systemEventEndTime = systemEventStartTime + 185000;
  systemEventPhase = 0;
  systemEventPhaseEndTime = 0;
}

void updateImperialMarch() {
  // get the current time
  systemEventCurrentTime = millis();

  // if time has expired, turn off and exit
  if (systemEventCurrentTime > systemEventEndTime) {
    currentSystemEvent = I2C_SystemEvent::Off;
    return;
  }

  int beatInMilliseconds = 577;

  // if delay time has expired, change the color randomly
  if (systemEventPhaseEndTime < systemEventCurrentTime) {
    switch (systemEventPhase) {
      case 0:
        setLed(I2C_HP_Color::Red);
        systemEventPhaseEndTime = systemEventCurrentTime + beatInMilliseconds;
        systemEventPhase = 1;
        break;

      case 1:
        setLed(I2C_HP_Color::Off);
        systemEventPhaseEndTime = systemEventCurrentTime + beatInMilliseconds;
        systemEventPhase = 0;
        break;
    }
  }
}

//------------------------------------------------------------------------------
// SERVOS
//------------------------------------------------------------------------------
byte clampServoPosition(byte position) {
  return position > 180 ? 180 : position;
}

void setServoLocation(I2C_HP_Servo::Value servo, byte position) {
  position = clampServoPosition(position);
  servos[(byte)servo].write(position);
}

void setTargettedPosition(I2C_HP_ServoMode::Value servoMode) {
    // targeted positions are only TopLeft to BottomRight
    if (servoMode < I2C_HP_ServoMode::TopLeft || servoMode > I2C_HP_ServoMode::BottomRight)
        return;

    byte x = ((byte)servoMode % 3) * 90;
    byte y = ((byte)servoMode / 3) * 90;    
        
    setServoLocation(I2C_HP_Servo::XServo, x);
    setServoLocation(I2C_HP_Servo::YServo, y);
}

//------------------------------------------------------------------------------
// I2C
//------------------------------------------------------------------------------
// Adding braces after case statements to fix 'jumping the case label'error - ugh
void receiveEvent(int eventCode) {
  currentMode = (I2C_HP_Mode::Value)Wire.read();

  switch (currentMode) {
    case I2C_HP_Mode::Color:
      {
        I2C_HP_Color::Value color = (I2C_HP_Color::Value)Wire.read();
        setLed(color);
      }
      break;
      
    case I2C_HP_Mode::Servo:
        {
            I2C_HP_ServoMode::Value servoMode = (I2C_HP_ServoMode::Value)Wire.read();
            setTargettedPosition(servoMode);
        }
        break;

    case I2C_HP_Mode::SystemEvent:
        {
            currentSystemEvent = (I2C_SystemEvent::Value)Wire.read();
            systemEventStartTime = millis();
    
            switch (currentSystemEvent) {
              case I2C_SystemEvent::Off:
                // TODO implement this
                break;
    
              case I2C_SystemEvent::LeiaMessage:
                setupLeiaMessage();
                break;
    
              case I2C_SystemEvent::ImperialMarch:
                setupImperialMarch();
                break;
            }
        }
        break;
  }
}

void processSystemEvent() {
  if (currentMode == I2C_HP_Mode::SystemEvent) {
    switch (currentSystemEvent) {
      case I2C_SystemEvent::Off:
        setLed(I2C_HP_Color::Off);
        break;

      case I2C_SystemEvent::LeiaMessage:
        updateLeiaMessage();
        break;

      case I2C_SystemEvent::ImperialMarch:
        updateImperialMarch();
        break;
    }
  }
}


