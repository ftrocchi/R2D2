#define USEDEBUGPRINT

#include "DebugHelper.h"
#include <Wire.h>
#include "I2C_Common.h"

void setup() {
    Wire.begin();
}

void loop() {
    debugPrint("Testing all 8 colors");
    for (int i=0; i<7; i++) {
        debugPrint("Sending to %d mode %d color %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::Color, (I2C_HP_Color::Value)i);
        Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
        Wire.write(I2C_HP_Mode::Color);
        Wire.write((I2C_HP_Color::Value)i);
        Wire.endTransmission();
        delayPrint(1);
    }
    
    // do full leia message
    debugPrint("Full Leia Message");
    debugPrint("Sending to %d mode %d sytem event: %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::SystemEvent, I2C_SystemEvent::LeiaMessage);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::SystemEvent);
    Wire.write(I2C_SystemEvent::LeiaMessage);
    Wire.endTransmission();
    delayPrint(32);
    
    // do partial leia message - system event off
    debugPrint("Partial Leia Message - system event off");
    debugPrint("Sending to %d mode %d sytem event: %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::SystemEvent, I2C_SystemEvent::LeiaMessage);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::SystemEvent);
    Wire.write(I2C_SystemEvent::LeiaMessage);
    Wire.endTransmission();
    delayPrint(10);
    debugPrint("Sending to %d mode %d sytem event: %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::SystemEvent, I2C_SystemEvent::Off);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::SystemEvent);
    Wire.write(I2C_SystemEvent::Off);
    Wire.endTransmission();
    
    delayPrint(3);
    
    // do partial leia message - color to red
    debugPrint("Partial Leia Message - color to red");
    debugPrint("Sending to %d mode %d sytem event: %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::SystemEvent, I2C_SystemEvent::LeiaMessage);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::SystemEvent);
    Wire.write(I2C_SystemEvent::LeiaMessage);
    Wire.endTransmission();
    delayPrint(10);
    debugPrint("Sending to %d mode %d color %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::Color, I2C_HP_Color::Red);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::Color);
    Wire.write(I2C_HP_Color::Red);
    Wire.endTransmission();
    
    delayPrint(3);
}

void delayPrint(int max) {
    for (int i=1; i<max + 1; i++) {
        delay(1000);
        debugPrint("Count: %d", i);
    }
}
