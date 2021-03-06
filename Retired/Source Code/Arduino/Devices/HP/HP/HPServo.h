#ifndef __HPSERVO_H__
#define __HPSERVO_H__

#include "Arduino.h"
#include <Servo.h>
#include "I2C_Common.h"

#define POS_MIN 20
#define POS_MAX 160
#define POS_CENTER 90

class HPServo
{
    private:
        Servo servo;
        int servoPin;
        bool twitch;
        byte twitchFrequency;
        unsigned long lastTimeCheck;

        void Twitch(bool isLarge);     
        void MoveServo(int pos);
        
        
    public:
        void Init(int pin);
        void MoveServoManual(int pos);
	void Update();
        void SetTwitch(bool isOn);
        void SetTwitchFrequency(byte frequency);
};

#endif
