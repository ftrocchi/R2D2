// BOF preprocessor bug prevent - insert me on top of your arduino-code
// From: http://www.a-control.de/arduino-fehler/?lang=en
#if 1
__asm volatile ("nop");
#endif

#include <Wire.h>
#include <LedControl.h>
#include <FastSPI_LED2.h>
#include <avr/pgmspace.h>
#include "I2C_Common.h"
#include "PSI.h"
#include "LogicDisplay.h"

#define ENABLE_FREE_RAM
#define TOGGLEPIN 4

#ifdef ENABLE_FREE_RAM
    unsigned long lastTimeCheck;
#endif

LedControl psiLedControl = LedControl(7, 8, 9, 1);
PSI psi(&psiLedControl);
LogicDisplay logicDisplay;

void setup() {
    #ifdef ENABLE_FREE_RAM
        Serial.begin(9600);
    #endif
    
    bool isRLD = isTogglePinHigh();
    I2C_Device_Address::Value address = isRLD ? I2C_Device_Address::RearLogicDisplay : I2C_Device_Address::FrontLogicDisplay;
    
    logicDisplay.setup(address, isRLD);
    psi.setup(address, isRLD);
    
    Wire.onReceive(receiveEvent);

//    logicDisplay.setPrimaryColor(I2C_Logic_Display_Selection::All, CRGB::Red);
//    logicDisplay.setPrimaryColor(I2C_Logic_Display_Selection::FLDBottom, CRGB::DarkViolet);
//    logicDisplay.setText(I2C_Logic_Display_Selection::FLDTop, "R2 IS COOL");
//    logicDisplay.setText(I2C_Logic_Display_Selection::FLDBottom, "MIDWEST R2S");
//    logicDisplay.setMode(I2C_Logic_Display_Selection::FLDBoth, I2C_Logic_Mode::Text);
    
//        logicDisplay.setMode(I2C_Logic_Display_Selection::FLDTop, I2C_Logic_Mode::Left_Right_Shift);
//        logicDisplay.setMode(I2C_Logic_Display_Selection::FLDBottom, I2C_Logic_Mode::Left_Right_Shift);

        logicDisplay.setMode(I2C_Logic_Display_Selection::All, I2C_Logic_Mode::Random_Pixel);

}

void loop() {
    psi.update();
    logicDisplay.update();
    
    
    #ifdef ENABLE_FREE_RAM
        unsigned long timeNow = millis();
        if (timeNow - lastTimeCheck < 5000)
            return;
            
        Serial.print("FreeRAM:");
        Serial.println(freeRam());

        lastTimeCheck = timeNow;
    #endif
    
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

#ifdef ENABLE_FREE_RAM
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
#endif

