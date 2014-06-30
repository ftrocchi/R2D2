#include <Wire.h>
#include <LedControl.h>
#include <FastSPI_LED2.h>
#include <avr/pgmspace.h>
#include "I2C_Common.h"
#include "PSI.h"
#include "LogicDisplay.h"

#define TOGGLEPIN 4

LedControl psiLedControl = LedControl(7, 8, 9, 1);
PSI psi(&psiLedControl);
LogicDisplay logicDisplay;

void setup() {
    bool isRLD = isTogglePinHigh();
    I2C_Device_Address::Value address = isRLD ? I2C_Device_Address::RearLogicDisplay : I2C_Device_Address::FrontLogicDisplay;
    
    logicDisplay.setup(address, isRLD);
    psi.setup(address, isRLD);
    
    Wire.onReceive(receiveEvent);
    
//    logicDisplay.setText(I2C_Logic_Display_Selection::FLDTop, "HELLO R2 BUILDERS!");
//    logicDisplay.setText(I2C_Logic_Display_Selection::FLDBottom, "MIDWESTR2.COM");
//    logicDisplay.setMode(I2C_Logic_Display_Selection::FLDBoth, I2C_Logic_Mode::Text);
        logicDisplay.setMode(I2C_Logic_Display_Selection::FLDBoth, I2C_Logic_Mode::Color_Shift);
}

void loop() {
    psi.update();
    logicDisplay.update();
}

void receiveEvent(int eventCode) {
    I2C_Logic_Device::Value logicDevice = (I2C_Logic_Device::Value)Wire.read();
    
    if (logicDevice == I2C_Logic_Device::PSI)
        psi.processCommand();
        
    if (logicDevice == I2C_Logic_Device::Logic)
        logicDisplay.processCommand();
}

bool isTogglePinHigh() {
    pinMode(TOGGLEPIN, INPUT);
    return digitalRead(TOGGLEPIN) == HIGH;
}
