#include "HPServo.h"

void HPServo::Init(int pin)
{
    servoPin = pin;
    
    servo.attach(servoPin);
    
    MoveServo(POS_CENTER);
    
    twitch = false;
    twitchFrequency = 15;
}

void HPServo::Update()
{
    if (!twitch)
        return;
        
    unsigned long timeNow = millis();
  
    if (timeNow - lastTimeCheck < twitchFrequency * 1000)
        return;

    lastTimeCheck = timeNow;
    
    MoveServo(random(POS_MIN, POS_MAX));
}

void HPServo::MoveServo(int pos)
{
    if (pos < POS_MIN) pos = POS_MIN;
    if (pos >POS_MAX) pos = POS_MAX;
    
    servo.write(pos);
}

void HPServo::MoveServoManual(int pos)
{
    twitch = false;
    MoveServo(pos);
}

void HPServo::SetTwitch(bool isOn)
{
    twitch = isOn;
}

void HPServo::SetTwitchFrequency(byte frequency)
{
    twitchFrequency = frequency;
}

void HPServo::Twitch(bool isLarge)
{
}
