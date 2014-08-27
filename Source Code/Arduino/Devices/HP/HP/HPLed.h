#ifndef __HPLED_H__
#define __HPLED_H__

#include "Arduino.h"
#include "I2C_Common.h"

class HPLed {
    private:
        byte redPin;
        byte greenPin;
        byte bluePin;
        
        bool currentColorRed;
        bool currentColorGreen;
        bool currentColorBlue;
        
        I2C_HP_Command::Value currentMode;
        
        void SetColor(bool red, bool green, bool blue);
        void SetLedOff();
        void SetLedOn();
        void AnimateAlarm();
        
        int currentModeState;
        unsigned long lastTimeCheck;
        bool IsTimeForStateChange(int delay);

    public:
        HPLed(int red, int green, int blue);
        void Update();
        
        void SetCurrentColor(I2C_HP_Command::Value command);
        
        void SetMode(I2C_HP_Command::Value command);
};


#endif
