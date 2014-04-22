#ifndef _BUTTONPAD_h
#define _BUTTONPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Keypad.h>

// Button Grounds
const int buttonMatrix0 = A1;
const int buttonMatrix1 = A2;
const int buttonMatrix2 = A3;
const int buttonMatrix3 = A4;

// LED Grounds
const int ledGnd0 = 9;
const int ledGnd1 = 10;
const int ledGnd2 = 11;
const int ledGnd3 = 5;

// RGB pins
const int redLED = 8;
const int greenLED = 7;
const int blueLED = 6;

// Color definitions
const int red[] = {255, 0, 0};
const int green[] = {0, 255, 0};
const int blue[] = {0, 0, 255};
const int purple[] = {255, 0, 150};
const int yellow[] = {255, 255, 0};
const int black[] = {0, 0, 0};
const int white[] = {255, 255, 255};

const uint8_t ROWS=1;
const uint8_t COLS=4;
const byte rowPins[ROWS] = {A0};
const byte colPins[COLS] = {buttonMatrix0, buttonMatrix1, buttonMatrix2, buttonMatrix3};
const char keys[ROWS][COLS] = {{1, 2, 3, 4}};
static Keypad buttons = Keypad(makeKeymap(keys), (byte*)rowPins, (byte*)colPins, ROWS, COLS);

class ButtonPad
{
    private:
        int buttonState[4];  // 0=OFF, 1=GOING ON, 2=ON, 3=GOING OFF
        bool blinkOn;
        unsigned long elapsed;

        void SetLEDs();
        void SetLED(int ledGnd, int buttonState);
        void SetLEDColor(int ledGnd, const int ledColor[]);
        const int* GetButtonStateColor(int buttonState);

    public:
        ButtonPad();
        void Update();
        void SetButtonState(int buttonNumber, int buttonState);
        int GetButtonState(int buttonNumber);
};

extern ButtonPad BUTTONPAD;

#endif


