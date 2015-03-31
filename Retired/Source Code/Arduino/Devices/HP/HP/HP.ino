#include <Wire.h>
#include <Servo.h>
#include "HPLed.h"
#include "HPServo.h"
#include "I2C_Common.h"

#define PIN_RED 10
#define PIN_GREEN 11
#define PIN_BLUE 9
#define PIN_X_SERVO 3
#define PIN_Y_SERVO 5

#define HP 25    // 25=Front FHP (BLUE), 26=Top THP (GREEN), 27=Rear RHP (RED)

HPLed hpLed = HPLed(PIN_RED, PIN_GREEN, PIN_BLUE);
HPServo xServo = HPServo();
HPServo yServo = HPServo();

void setup()
{
    Wire.begin(HP);
    
    xServo.Init(PIN_X_SERVO);
    yServo.Init(PIN_Y_SERVO);
    
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
	xServo.Update();
	yServo.Update();
}

void receiveEvent(int eventCode)
{
    I2C_HP_Command::Value command = (I2C_HP_Command::Value)Wire.read();
    byte frequency;
    
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
            
        case I2C_HP_Command::XLeft: xServo.MoveServoManual(POS_MIN); break;
        case I2C_HP_Command::XCenter: xServo.MoveServoManual(POS_CENTER); break;
        case I2C_HP_Command::XRight: xServo.MoveServoManual(POS_MAX); break;
        case I2C_HP_Command::YTop: yServo.MoveServoManual(POS_MIN); break;
        case I2C_HP_Command::YCenter: yServo.MoveServoManual(POS_CENTER); break;
        case I2C_HP_Command::YBottom: yServo.MoveServoManual(POS_MAX); break;
	case I2C_HP_Command::XCustom: xServo.MoveServoManual(Wire.read()); break;
	case I2C_HP_Command::YCustom: yServo.MoveServoManual(Wire.read()); break;
        
        case I2C_HP_Command::TopLeft: xServo.MoveServoManual(POS_MIN); yServo.MoveServoManual(POS_MIN); break;
        case I2C_HP_Command::TopCenter: xServo.MoveServoManual(POS_CENTER); yServo.MoveServoManual(POS_MIN); break;
        case I2C_HP_Command::TopRight: xServo.MoveServoManual(POS_MAX); yServo.MoveServoManual(POS_MIN); break;
        case I2C_HP_Command::CenterLeft: xServo.MoveServoManual(POS_MIN); yServo.MoveServoManual(POS_CENTER); break;
        case I2C_HP_Command::CenterCenter: xServo.MoveServoManual(POS_CENTER); yServo.MoveServoManual(POS_CENTER); break;
        case I2C_HP_Command::CenterRight: xServo.MoveServoManual(POS_MAX); yServo.MoveServoManual(POS_CENTER); break;
        case I2C_HP_Command::BottomLeft: xServo.MoveServoManual(POS_MIN); yServo.MoveServoManual(POS_MAX); break;
        case I2C_HP_Command::BottomCenter: xServo.MoveServoManual(POS_CENTER); yServo.MoveServoManual(POS_MAX); break;
        case I2C_HP_Command::BottomRight: xServo.MoveServoManual(POS_MAX); yServo.MoveServoManual(POS_MAX); break;
	case I2C_HP_Command::Both: xServo.MoveServoManual(Wire.read()); yServo.MoveServoManual(Wire.read()); break;

        case I2C_HP_Command::TwitchOff:
            xServo.SetTwitch(false);
            yServo.SetTwitch(false);
            break;
            
        case I2C_HP_Command::TwitchOn:
            xServo.SetTwitch(true);
            yServo.SetTwitch(true);
            break;
            
        case I2C_HP_Command::TwitchFrequency:
            frequency = Wire.read();
            xServo.SetTwitchFrequency(frequency);
            yServo.SetTwitchFrequency(frequency);
            break;

        default:
            break;
    }
}
