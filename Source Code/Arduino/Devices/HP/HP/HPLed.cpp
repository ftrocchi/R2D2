#include "HPLed.h"

HPLed::HPLed(int red, int green, int blue)
{
    redPin = red;
    greenPin = green;
    bluePin = blue;
    
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    currentColorRed = LOW;
    currentColorGreen = LOW;
    currentColorBlue = LOW;
    
    currentMode = I2C_HP_Command::Off;
}

void HPLed::Update()
{
    switch (currentMode)
    {
        case I2C_HP_Command::Off:
            SetLedOff();
            break;
            
        case I2C_HP_Command::On:
            SetLedOn();
            break;
            
        case I2C_HP_Command::Alarm:
            AnimateAlarm();
            break;
            
        default:
            break;
    }
}

void HPLed::SetColor(bool red, bool green, bool blue)
{
    digitalWrite(redPin, red);
    digitalWrite(greenPin, green);
    digitalWrite(bluePin, blue);
}

void HPLed::SetCurrentColor(I2C_HP_Command::Value command)
{
    switch (command)
    {
        case I2C_HP_Command::Black:
            currentColorRed = false;
            currentColorGreen = false;
            currentColorBlue = false;
            break;
            
        case I2C_HP_Command::White:
            currentColorRed = true;
            currentColorGreen = true;
            currentColorBlue = true;
            break;
            
        case I2C_HP_Command::Red:
            currentColorRed = true;
            currentColorGreen = false;
            currentColorBlue = false;
            break;
            
        case I2C_HP_Command::Green:
            currentColorRed = false;
            currentColorGreen = true;
            currentColorBlue = false;
            break;
            
        case I2C_HP_Command::Blue:
            currentColorRed = false;
            currentColorGreen = false;
            currentColorBlue = true;
            break;
            
        case I2C_HP_Command::Magenta:
            currentColorRed = true;
            currentColorGreen = false;
            currentColorBlue = true;
            break;
            
        case I2C_HP_Command::Yellow:
            currentColorRed = true;
            currentColorGreen = true;
            currentColorBlue = false;
            break;
            
        case I2C_HP_Command::Cyan:
            currentColorRed = false;
            currentColorGreen = true;
            currentColorBlue = true;
            break;
    }
}

void HPLed::SetMode(I2C_HP_Command::Value command)
{
    switch (command)
    {
        case I2C_HP_Command::Off:
        case I2C_HP_Command::On:
            currentMode = command;
            break;
            
        case I2C_HP_Command::Alarm:
            currentMode = command;
            currentModeState = 1;
            break;
            
        default:
            break;
    }
}

void HPLed::SetLedOff()
{
    SetColor(LOW, LOW, LOW);
}

void HPLed::SetLedOn()
{
    SetColor(currentColorRed, currentColorGreen, currentColorBlue);
}

void HPLed::AnimateAlarm()
{
    if (!IsTimeForStateChange(250))
        return;
        
    if (currentModeState == 1)
        SetLedOn();
    else
        SetLedOff();
        
    currentModeState *= -1;
}

bool HPLed::IsTimeForStateChange(int delay)
{
    unsigned long timeNow = millis();
  
    if (timeNow - lastTimeCheck < delay)
        return false;

    lastTimeCheck = timeNow;

    return true;
}

