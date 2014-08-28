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
        
    public:
        void Init(int pin);
        
        void MoveServo(int pos);
};


#endif
