#ifndef __PSI_H__
#define __PSI_H__

#include <Wire.h>
#include <LedControl.h>
#include "I2C_Common.h"

class PSI {
    private:
        static const int patternAtStage[9];
        LedControl *ledControl;
        I2C_DeviceAddress::Value i2cAddress;
        unsigned long lastTimeCheck;
        I2C_PSI_Mode::Value currentMode;
        bool isModeActive;
        bool IsTimeForStateChange(int delay);
        
        // normal
        void resetModes(int first, int second, int transition);
        void animateNormal();
        int delayAtStage[9];
        int stage;
        int slideDirection;
        int maxStage;
        
        // march
        void animateMarch();
        bool firstColor;
        
        // spin
        void animateSpin();
        int spinState;
        
        // ring
        void animateRing();
        int ringState;
        int ringDirection;
        
        // up down
        void animateUpDown();
        int upDownState;
        int upDownDirection;
        
        // left right
        void animateLeftRight();
        int leftRightState;
        int leftRightDirection;
        
    public:
        PSI(I2C_DeviceAddress::Value address, LedControl *led, int first, int second, int transition);
        
        void update();
        void processCommand();
        
        void on();
        void off();
        void setBrightness(int brightness);
        void setMode(I2C_PSI_Mode::Value mode);
};

#endif
