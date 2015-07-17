// There are (at this time) 3 servo boards expected to be in place
// change this to set the correct settings for the servo boards
// 1 = Dome Panels, 2 = Dome HPs, 3 = Body
#define SERVOBOARD 1

#include <Wire.h>
#include <Servo.h>
#include "DebugHelper.h"
#include "I2C_Common.h"
#include "DomePanels.h"
#include "DomeHPs.h"

void setup() {
    setupServoBoard();
    
    Wire.begin(servoBoardAddress);
    Wire.onReceive(receiveEvent);
}

void loop() {
    int i=0;
    
    for (i=0; i<11; i++) {
    setServoPosition(i, 0);
    delay(2000);
    setServoPosition(i, 180);
    delay(2000);
    }
}

int clamp(int value, int min, int max) {
    if (value < min) 
        return min;
    else if (value > max)
        return max;
    else
        return value;
}

void setServoPosition(int servo, int value) {
    if (servo > SERVOCOUNT - 1)
        return;
    
    servos[servo].write(clamp(value, servoMins[servo], servoMaxs[servo]));
}

//------------------------------------------------------------------------------
// I2C
//------------------------------------------------------------------------------
void receiveEvent(int eventCode) {
    I2C_ServoBoard_Command::Value servoBoardCommand = (I2C_ServoBoard_Command::Value)Wire.read();
    
    switch (servoBoardCommand) {
        case I2C_ServoBoard_Command::MoveTo:
            int servoIndex = Wire.read();
            int angle = Wire.read();
            setServoPosition(servoIndex, angle);
            break;
    };
}

