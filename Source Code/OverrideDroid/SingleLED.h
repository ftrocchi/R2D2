#ifndef __SINGLELED_H__
#define __SINGLELED_H__

#include <Arduino.h>

#define SINGLE_LED_ON 0
#define SINGLE_LED_OFF 1
#define SINGLE_LED_BLINK 2
#define SINGLE_LED_BLINK_ONCE 3

class SingleLED
{
    private:
        int pin;
        int mode;
        int blinkTime[2];
        int previousMode;
        bool isBlinkOn;
        unsigned long lastTime;
        bool isTimeToChange();
    
    public:
        SingleLED(int pin, int mode, int on = 500, int off = 500);
        void SetNewMode(int newMode);
        void SetNewMode(int newMode, int on, int off);
        void Update();
};

#endif
