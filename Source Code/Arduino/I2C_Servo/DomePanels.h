#ifndef __DOMEPANELS_H__
#define __DOMEPANELS_H__

#if SERVOBOARD == 1

#define SERVOCOUNT 10

I2C_DeviceAddress::Value servoBoardAddress = I2C_DeviceAddress::DomePanelServo;
Servo servos[SERVOCOUNT];
int servoMins[SERVOCOUNT];
int servoMaxs[SERVOCOUNT];

void setupServoBoard() {
    for (int i=0; i<maxServosIndex; i++) {
        servos[i].attach(i+2);
        servoMins[i] = 0;
        servoMaxs[i] = 180;
    }
}

#endif

#endif
