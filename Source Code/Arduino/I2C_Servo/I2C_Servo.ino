// There are (at this time) 3 servo boards expected to be in place
// change this to set the correct settings for the servo boards
// 1 = Dome Panels, 2 = Dome HPs, 3 = Body
#define SERVOBOARD 2

#include <Wire.h>
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
