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
}

void loop() {
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
    if (servo > maxServosIndex)
        return;
    
    servos[servo].write(clamp(value, servoMins[servo], servoMaxs[servo]));
}

