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
    
    debugPrint("Sending color twitch");
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::SystemEvent);
    Wire.write(I2C_SystemEvent::TwitchHPColor);
    Wire.endTransmission();
    delayPrint(30);

    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::Color);
    Wire.write(I2C_HP_Color::Off);
    Wire.endTransmission();
    
    delayPrint(3);

    /*
    // do full leia message
    debugPrint("Full Leia Message");
    debugPrint("Sending to %d mode %d sytem event: %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::SystemEvent, I2C_SystemEvent::LeiaMessage);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::SystemEvent);
    Wire.write(I2C_SystemEvent::LeiaMessage);
    Wire.endTransmission();
    delayPrint(38);
    
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
    
    // do partial leia message - color to yellow
    debugPrint("Partial Leia Message - color to yellow");
    debugPrint("Sending to %d mode %d sytem event: %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::SystemEvent, I2C_SystemEvent::LeiaMessage);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::SystemEvent);
    Wire.write(I2C_SystemEvent::LeiaMessage);
    Wire.endTransmission();
    delayPrint(10);
    debugPrint("Sending to %d mode %d color %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::Color, I2C_HP_Color::Yellow);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::Color);
    Wire.write(I2C_HP_Color::Yellow);
    Wire.endTransmission();
    
    delayPrint(3);
    
    // do partial imperial march - color to magenta
    debugPrint("Partial Imperial March - color to magenta");
    debugPrint("Sending to %d mode %d sytem event: %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::SystemEvent, I2C_SystemEvent::ImperialMarch);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::SystemEvent);
    Wire.write(I2C_SystemEvent::ImperialMarch);
    Wire.endTransmission();
    debugPrint("Sending to %d mode %d sytem event: %d", I2C_DeviceAddress::TopHP, I2C_HP_Mode::SystemEvent, I2C_SystemEvent::ImperialMarch);
    Wire.beginTransmission(I2C_DeviceAddress::TopHP);
    Wire.write(I2C_HP_Mode::SystemEvent);
    Wire.write(I2C_SystemEvent::ImperialMarch);
    Wire.endTransmission();
    debugPrint("Sending to %d mode %d sytem event: %d", I2C_DeviceAddress::RearHP, I2C_HP_Mode::SystemEvent, I2C_SystemEvent::ImperialMarch);
    Wire.beginTransmission(I2C_DeviceAddress::RearHP);
    Wire.write(I2C_HP_Mode::SystemEvent);
    Wire.write(I2C_SystemEvent::ImperialMarch);
    Wire.endTransmission();
    delayPrint(60);
    debugPrint("Sending to %d mode %d color %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::Color, I2C_HP_Color::Magenta);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::Color);
    Wire.write(I2C_HP_Color::Magenta);
    Wire.endTransmission();
    debugPrint("Sending to %d mode %d color %d", I2C_DeviceAddress::TopHP, I2C_HP_Mode::Color, I2C_HP_Color::Magenta);
    Wire.beginTransmission(I2C_DeviceAddress::TopHP);
    Wire.write(I2C_HP_Mode::Color);
    Wire.write(I2C_HP_Color::Magenta);
    Wire.endTransmission();
    debugPrint("Sending to %d mode %d color %d", I2C_DeviceAddress::RearHP, I2C_HP_Mode::Color, I2C_HP_Color::Magenta);
    Wire.beginTransmission(I2C_DeviceAddress::RearHP);
    Wire.write(I2C_HP_Mode::Color);
    Wire.write(I2C_HP_Color::Magenta);
    Wire.endTransmission();

    delayPrint(3);

    // turn all off    
    debugPrint("Sending to %d mode %d color %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::Color, I2C_HP_Color::Off);
    Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
    Wire.write(I2C_HP_Mode::Color);
    Wire.write(I2C_HP_Color::Off);
    Wire.endTransmission();
    debugPrint("Sending to %d mode %d color %d", I2C_DeviceAddress::TopHP, I2C_HP_Mode::Color, I2C_HP_Color::Off);
    Wire.beginTransmission(I2C_DeviceAddress::TopHP);
    Wire.write(I2C_HP_Mode::Color);
    Wire.write(I2C_HP_Color::Off);
    Wire.endTransmission();
    debugPrint("Sending to %d mode %d color %d", I2C_DeviceAddress::RearHP, I2C_HP_Mode::Color, I2C_HP_Color::Off);
    Wire.beginTransmission(I2C_DeviceAddress::RearHP);
    Wire.write(I2C_HP_Mode::Color);
    Wire.write(I2C_HP_Color::Off);
    Wire.endTransmission();
    */
}

void delayPrint(int max) {
    for (int i=1; i<max + 1; i++) {
        delay(1000);
        debugPrint("Count: %d", i);
    }
}
