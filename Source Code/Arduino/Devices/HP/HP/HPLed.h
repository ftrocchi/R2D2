#ifndef __HPLED_H__
#define __HPLED_H__

#include "Arduino.h"
#include "I2C_Common.h"

class HPLed {
    private:
        byte redPin;
        byte greenPin;
        byte bluePin;
        
        byte currentColorRed;
        byte currentColorGreen;
        byte currentColorBlue;
        
        I2C_HP_Command::Value currentMode;
        
        void SetColor(byte red, byte green, byte blue);
        void SetLedOff();
        void SetLedOn();
        void AnimateAlarm();
        void AnimateLeia();
        void AnimateDisco();
        void AnimateFailure();
        
        int currentModeState;
        unsigned long lastTimeCheck;
        bool IsTimeForStateChange(int delay);
        int leiaDelay;

    public:
        HPLed(int red, int green, int blue);
        void Update();
        
        void SetCurrentColor(I2C_HP_Command::Value command);
        void SetCurrentColor(byte red, byte green, byte blue);
        
        void SetMode(I2C_HP_Command::Value command);
};


#endif
