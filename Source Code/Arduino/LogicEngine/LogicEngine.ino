#include <Wire.h>
#include <LedControl.h>
#include "I2C_Common.h"
#include "PSI.h"

#define FLD

LedControl ledControl = LedControl(7, 8, 9, 1);

#ifdef FLD
    PSI psi(I2C_DeviceAddress::FrontLogicDisplay, &ledControl, 1000, 2000, 200);
#else    
    PSI psi(I2C_DeviceAddress::RearLogicDisplay, &ledControl, 2000, 1000, 200);
#endif

void setup() {
    Wire.onReceive(receiveEvent);
}

void loop() {
    psi.update();
}

void receiveEvent(int eventCode) {
    I2C_Logic_Device::Value logicDevice = (I2C_Logic_Device::Value)Wire.read();
    
    if (logicDevice == I2C_Logic_Device::PSI)
        psi.processCommand();
}

