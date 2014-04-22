#include "ButtonPad.h"

ButtonPad::ButtonPad()
{
    // Switch Grounds
    pinMode(buttonMatrix0, INPUT);
    pinMode(buttonMatrix1, INPUT);
    pinMode(buttonMatrix2, INPUT);
    pinMode(buttonMatrix3, INPUT);

    // Led grounds
    pinMode(ledGnd0, OUTPUT);
    pinMode(ledGnd1, OUTPUT);
    pinMode(ledGnd2, OUTPUT);
    pinMode(ledGnd3, OUTPUT);

    // RGB pins
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(blueLED, OUTPUT);

    // set initial button states to on
    buttonState[0] = 2;
    buttonState[1] = 2;
    buttonState[2] = 2;
    buttonState[3] = 2;

    // elapsed for blink
    elapsed = 0;
}

void ButtonPad::Update()
{
    // set the colors first
    SetLEDs();

    // read keys
    char key = buttons.getKey();
    if (key && key <=4)
    {
        buttonState[key-1]++;
        if (buttonState[key-1] > 3)
            buttonState[key-1] = 0;
    }

    // toggle blink
    if (elapsed + 100 < millis())
    {
        blinkOn = !blinkOn;
        elapsed = millis();
    }
}

void ButtonPad::SetLEDs()
{
    SetLED(ledGnd0, buttonState[0]);
    SetLED(ledGnd1, buttonState[1]);
    SetLED(ledGnd2, buttonState[2]);
    SetLED(ledGnd3, buttonState[3]);
}

void ButtonPad::SetLED(int ledGnd, int buttonState)
{
    if (buttonState != 1 && buttonState != 3)
    {
        SetLEDColor(ledGnd, GetButtonStateColor(buttonState));
        return;
    }

    if (blinkOn)
        SetLEDColor(ledGnd, GetButtonStateColor(buttonState));
    else
        SetLEDColor(ledGnd, black);
}

void ButtonPad::SetLEDColor(int ledGnd, const int ledColor[])
{
    analogWrite(redLED, ledColor[0]);
    analogWrite(greenLED, ledColor[1]);
    analogWrite(blueLED, ledColor[2]);

    // flicker control
    delay(2);
    digitalWrite(ledGnd, LOW);

    // flicker control
    delayMicroseconds(1100);
    digitalWrite(ledGnd, HIGH);
}

const int* ButtonPad::GetButtonStateColor(int buttonState)
{
    if (buttonState == 0)
        return red;
    else if (buttonState == 1 || buttonState == 3)
        return blue;
    else 
        return green;
}

void ButtonPad::SetButtonState(int buttonNumber, int state)
{
    buttonState[buttonNumber] = state;
}

int ButtonPad::GetButtonState(int buttonNumber)
{
    return buttonState[buttonNumber];
}

ButtonPad BUTTONPAD;


