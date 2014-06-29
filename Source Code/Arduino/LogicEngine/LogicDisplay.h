#ifndef __LOGIC_DISPLAY_H__
#define __LOGIC_DISPLAY_H__

#include "Arduino.h"
#include <FastSPI_LED2.h>
#include <avr/pgmspace.h>
#include "I2C_Common.h"

#define SPEEDPIN 3
#define DATA_PIN 6

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
        bool isModeActive;
        I2C_Logic_Mode::Value currentMode;
        CRGB primaryColor;
        unsigned long lastTimeCheck;
        
        void generateAllColors();
        void updateLed(byte ledNum, byte hueVal);
        void setInitialColors();
        void showStartupAnimation();
        void clear();
        bool IsTimeForStateChange(int delay);
        
        
        void animateNormal();
        
        // march together
        void animateMarchTogether();
        void animateMarchSeparate();
        bool firstColor;
    
    public:
        void setup(I2C_Device_Address::Value address, bool isRLDLogic);
        
        void update();
        void processCommand();
        
        void on();
        void off();
        void setBrightness(byte brightness);
        void setMode(I2C_Logic_Mode::Value mode);
};

#endif
