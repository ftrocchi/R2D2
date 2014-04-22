#include "SingleLED.h"

SingleLED::SingleLED(int ledPin, int startMode, int on, int off)
{
    pin = ledPin;
    pinMode(pin, OUTPUT);
    
    previousMode = -1;
    
    SetNewMode(startMode, on , off);
}

void SingleLED::SetNewMode(int newMode)
{
    mode = newMode;
}

void SingleLED::SetNewMode(int newMode, int on, int off)
{
    mode = newMode;
    blinkTime[0] = off;
    blinkTime[1] = on;
}

void SingleLED::Update()
{
    // if there is a change in mode, do something
    if (previousMode != mode)
    {
        previousMode = mode;

        if (mode == SINGLE_LED_ON)
            digitalWrite(pin, HIGH);
        else if (mode == SINGLE_LED_OFF)
            digitalWrite(pin, LOW);
        else if (mode == SINGLE_LED_BLINK || mode == SINGLE_LED_BLINK_ONCE)
        {
            isBlinkOn = true;
            lastTime = millis();
            digitalWrite(pin, HIGH);
        }
    }
    else if (mode == SINGLE_LED_BLINK || mode == SINGLE_LED_BLINK_ONCE)
    {
        // no change in mode so all we need to check is the blinking mode
        if (lastTime + blinkTime[isBlinkOn] < millis())
        {
            lastTime = millis();
            isBlinkOn = !isBlinkOn;
            
            digitalWrite(pin, isBlinkOn ? HIGH : LOW);
            
            if (mode == SINGLE_LED_BLINK_ONCE)
                mode = SINGLE_LED_OFF;
        }
    }
}

