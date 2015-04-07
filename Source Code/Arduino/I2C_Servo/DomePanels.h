#ifndef __DOMEPANELS_H__
#define __DOMEPANELS_H__

#if SERVOBOARD == 1

#define SERVOCOUNT 1

I2C_DeviceAddress::Value servoBoardAddress = I2C_DeviceAddress::DomePanelServo;
Servo servos[SERVOCOUNT];
int servoMins[SERVOCOUNT];
int servoMaxs[SERVOCOUNT];

void setupServo(int index, int min, int max) {
    servos[index].attach(index + 2);
    servoMins[index] = min;
    servoMaxs[index] = max;
}

void setupServoBoard() {
    for (int i=0; i<SERVOCOUNT; i++) {
        setupServo(i, 0, 180);
    }
}

#endif

#endif
