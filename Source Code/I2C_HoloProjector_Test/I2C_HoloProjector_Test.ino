#define USEDEBUGPRINT

#include "DebugHelper.h"
#include <Wire.h>
#include "I2C_Common.h"

void setup() {
    Wire.begin();
}

void loop() {
    for (int i=7; i>=0; i--) {
        debugPrint("Sending to %d mode %d color %d", I2C_DeviceAddress::FrontHP, I2C_HP_Mode::Color, (I2C_HP_Color::Value)i);
        Wire.beginTransmission(I2C_DeviceAddress::FrontHP);
        Wire.write(I2C_HP_Mode::Color);
        Wire.write((I2C_HP_Color::Value)i);
        Wire.endTransmission();
        delay(2000);
    }

}
