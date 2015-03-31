#ifndef __LOGIC_DISPLAY_H__
#define __LOGIC_DISPLAY_H__

#include "Arduino.h"
#include <FastSPI_LED2.h>
#include <avr/pgmspace.h>
#include "I2C_Common.h"

#define SPEEDPIN 3
#define DATA_PIN 6
#define TOP_FLD_RLD 0
#define BOTTOM_FLD 1

const byte fldColors[6][3]PROGMEM = { {170,0,0} , {170,255,54} , {170,255,120} , {166,255,200} , {154,84,150} , {174,0,200} }; // black, dark blue, medium blue, blue, bright grey, white
const byte rldColors[5][3]PROGMEM = { {87,0,0} , {87,206,105} , {79,255,184} , {18,255,250} , {0,255,214} };  // black, dark green, green, yellow, red
const byte tweens = 6;
const byte rldMap[]PROGMEM = {
 0, 1, 2, 3, 4, 5, 6, 7,48,49,50,51,52,53,54,55,
15,14,13,12,11,10, 9, 8,63,62,61,60,59,58,57,56,
16,17,18,19,20,21,22,23,64,65,66,67,68,69,70,71,
31,30,29,28,27,26,25,24,79,78,77,76,75,74,73,72,
32,33,34,35,36,37,38,39,80,81,82,83,84,85,86,87,
47,46,45,44,43,42,41,40,95,94,93,92,91,90,89,88};
const byte fldMap[]PROGMEM = {
15,14,13,12,11,10, 9, 8,
16,17,18,19,20,21,22,23,
31,30,29,28,27,26,25,24,
32,33,34,35,36,37,38,39,
47,46,45,44,43,42,41,40,
//
88,89,90,91,92,93,94,95,
87,86,85,84,83,82,81,80,
72,73,74,75,76,77,78,79,
71,70,69,68,67,66,65,64,
56,57,58,59,60,61,62,63};

// RLD display map
const byte rldRows[4][24]PROGMEM = {
    { 0, 1, 2, 12, 3, 11, 4, 10, 5, 9, 6, 7, 48, 49, 62, 50, 61, 51, 60, 52, 59, 53, 54, 55 },
    { 15, 14, 13, 16, 17, 18, 19, 20, 21, 22, 23, 8, 63, 64, 65, 66, 67, 68, 69, 70, 71, 58, 57, 56 },
    { 32, 33, 34, 31, 30, 29, 28, 27, 26, 25, 24, 39, 80, 79, 78, 77, 76, 75, 74, 73, 72, 85, 86, 87 }, 
    { 47, 46, 45, 35, 44, 36, 43, 37, 42, 38, 41, 40, 95, 94, 81, 93, 82, 92, 83, 91, 84, 90, 89, 88 },
};

// text
const byte letters[41][6]PROGMEM = {
{ B11110, B00101, B00101, B11110, B00000, B00000 }, // 00 - A
{ B11111, B10101, B10101, B01010, B00000, B00000 }, // 01 - B
{ B01110, B10001, B10001, B10001, B00000, B00000 }, // 02 - C
{ B11111, B10001, B10001, B01110, B00000, B00000 }, // 03 - D
{ B11111, B10101, B10001, B00000, B00000, B00000 }, // 04 - E
{ B11111, B00101, B00001, B00000, B00000, B00000 }, // 05 - F
{ B01110, B10001, B11001, B01010, B00000, B00000 }, // 06 - G
{ B11111, B00100, B00100, B11111, B00000, B00000 }, // 07 - H
{ B10001, B11111, B10001, B00000, B00000, B00000 }, // 08 - I
{ B01000, B10001, B01111, B00001, B00000, B00000 }, // 09 - J
{ B11111, B00100, B11011, B00000, B00000, B00000 }, // 10 - K
{ B11111, B10000, B10000, B00000, B00000, B00000 }, // 11 - L
{ B11111, B00010, B00100, B00010, B11111, B00000 }, // 12 - M
{ B11111, B00110, B01100, B11111, B00000, B00000 }, // 13 - N
{ B01110, B10001, B10001, B01110, B00000, B00000 }, // 14 - O
{ B11111, B00101, B00101, B00010, B00000, B00000 }, // 15 - P
{ B01110, B10001, B11001, B11110, B00000, B00000 }, // 16 - Q
{ B11111, B00101, B00101, B11010, B00000, B00000 }, // 17 - R
{ B10110, B10101, B01101, B00000, B00000, B00000 }, // 18 - S
{ B00001, B11111, B00001, B00000, B00000, B00000 }, // 19 - T
{ B01111, B10000, B10000, B01111, B00000, B00000 }, // 20 - U
{ B01111, B11000, B01111, B00000, B00000, B00000 }, // 21 - V
{ B11111, B01000, B00110, B01000, B11111, B00000 }, // 22 - W
{ B10001, B11011, B00100, B11011, B10001, B00000 }, // 23 - X
{ B00011, B11110, B00011, B00000, B00000, B00000 }, // 24 - Y
{ B10001, B11101, B10111, B10001, B00000, B00000 }, // 25 - Z
{ B01110, B10001, B01110, B00000, B00000, B00000 }, // 26 - 0
{ B10010, B11111, B10000, B00000, B00000, B00000 }, // 27 - 1
{ B10010, B11001, B10110, B00000, B00000, B00000 }, // 28 - 2
{ B10001, B10101, B01010, B00000, B00000, B00000 }, // 29 - 3
{ B01100, B01010, B11111, B01000, B00000, B00000 }, // 30 - 4
{ B10111, B10101, B01001, B00000, B00000, B00000 }, // 31 - 5
{ B01110, B10101, B01001, B00000, B00000, B00000 }, // 32 - 6
{ B10001, B01101, B00011, B00000, B00000, B00000 }, // 33 - 7
{ B01010, B10101, B10101, B01010, B00000, B00000 }, // 34 - 8
{ B10010, B10101, B01110, B00000, B00000, B00000 }, // 35 - 9
{ B00100, B00100, B00000, B00000, B00000, B00000 }, // 36 - - 
{ B100000, B100000, B00000, B00000, B00000, B00000 }, // 37 
{ B10111, B00000, B00000, B00000, B00000, B00000 }, // 38 - !
{ B10000, B00000, B00000, B00000, B00000, B00000 }, // 39 - .
{ B00011, B00000, B00000, B00000, B00000, B00000 }  // 40 - +
};

class LogicDisplay {
    private:
        I2C_Device_Address::Value i2cAddress;
        bool isRLD;
        byte tweenPause;
        int keyPause;
        byte keys;
        byte totalColors;
        bool speeds;
        byte allColors[45][3];
        CRGB leds[96];
        byte LEDstat[96][3];
        byte hueVal;
        
        I2C_Logic_Mode::Value currentMode[2]; // 0 = top / rld, 1 = bottom
        CRGB primaryColor[2];
        unsigned long lastTimeCheck[2];
        
        void generateAllColors();
        void updateLed(byte ledNum, byte hueVal);
        void setInitialColors();
        void showStartupAnimation();
        void clear(byte isTopOrBottom);
        bool IsTimeForStateChange(byte isTopOrBottom, int delay, bool shouldClear);
        
        // on mode
        void animateOn(byte isTopOrBottom);
        
        // off mode
        void animateOff(byte isTopOrBottom);
        
        // normal mode
        void animateNormal(byte isTopOrBottom);
        
        // march
        void animateMarch(byte isTopOrBottom);
        bool marchState[2];
        
        // color shift
        void animateColorShift(byte isTopOrBottom);
        byte colorShiftHue[2];
        
        // left shift
        void animateLeftShift(byte isTopOrBottom);
        byte leftShiftPosition[2];
        
        // right shift
        void animateRightShift(byte isTopOrBottom);
        byte rightShiftPosition[2];
        
        // left right shift
        void animateLeftRightShift(byte isTopOrBottom);
        bool isMovingLeft[2];
        
        // randome pixel
        void animateRandomPixel(byte isTopOrBottom);
        byte currentPixel[2];
        
        
        // text
        void animateText(byte isTopOrBottom);
        byte getLetterIndex(char letter);
        void shiftFLDRow(byte left, byte right, bool moveLeft);
        void shiftRLDRow(byte row, bool moveLeft);
        String textString[2];
        unsigned int textPosition[2];
        int letterPosition[2];
        void shiftDisplay(byte isTopOrBottom, bool moveLeft);
        
    public:
        void setup(I2C_Device_Address::Value address, bool isRLDLogic);
        
        void update();
        void processCommand();
        
        void setBrightness(byte brightness);
        void setMode(I2C_Logic_Display_Selection::Value display, I2C_Logic_Mode::Value mode);
        void setText(I2C_Logic_Display_Selection::Value display, String text);
        void setPrimaryColor(I2C_Logic_Display_Selection::Value display, byte hue, byte saturation, byte value);
        void setPrimaryColor(I2C_Logic_Display_Selection::Value display, CRGB value);
        void setPrimaryColor(I2C_Logic_Display_Selection::Value display);
};

#endif
