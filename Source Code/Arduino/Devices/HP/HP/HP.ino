#include <Wire.h>
#include "HPLed.h"
#include "I2C_Common.h"

#define PIN_RED 10
#define PIN_GREEN 11
#define PIN_BLUE 9

#define HP 25    // 25=Front FHP (BLUE), 26=Top THP (GREEN), 27=Rear RHP (RED)

HPLed hpLed = HPLed(PIN_RED, PIN_GREEN, PIN_BLUE);

void setup()
{
    Wire.begin(HP);
    
    hpLed.SetCurrentColor(  HP == 27 ? I2C_HP_Command::Red : HP == 26 ? I2C_HP_Command::Green : I2C_HP_Command::Blue);
    hpLed.SetMode(I2C_HP_Command::On);
    hpLed.Update();
    delay(5000);
    hpLed.SetMode(I2C_HP_Command::Off);
    hpLed.Update();
    
    Wire.onReceive(receiveEvent);
}

void loop()
{
    hpLed.Update();
}

void receiveEvent(int eventCode)
{
    I2C_HP_Command::Value command = (I2C_HP_Command::Value)Wire.read();
    
    switch (command)
    {
        case I2C_HP_Command::Black:
        case I2C_HP_Command::White:
        case I2C_HP_Command::Red:
        case I2C_HP_Command::Green:
        case I2C_HP_Command::Blue:
        case I2C_HP_Command::Magenta:
        case I2C_HP_Command::Yellow:
        case I2C_HP_Command::Cyan:
            hpLed.SetCurrentColor(command);
            break;
            
        case I2C_HP_Command::CustomColor:
            hpLed.SetCurrentColor(Wire.read(), Wire.read(), Wire.read());
            break;
            
        case I2C_HP_Command::Off:
        case I2C_HP_Command::On:
        case I2C_HP_Command::Alarm:
	case I2C_HP_Command::Leia:
	case I2C_HP_Command::Disco:
	case I2C_HP_Command::Failure:
            hpLed.SetMode(command);
            break;
           
        default:
            break;
    }
}

