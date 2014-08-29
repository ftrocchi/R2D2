#include "HPServo.h"

void HPServo::Init(int pin)
{
    servoPin = pin;
    
    servo.attach(servoPin);
    
    MoveServo(POS_CENTER);
}

void HPServo::Update()
{
}

void HPServo::MoveServo(int pos)
{
    if (pos < POS_MIN) pos = POS_MIN;
    if (pos >POS_MAX) pos = POS_MAX;
    
    servo.write(pos);
}