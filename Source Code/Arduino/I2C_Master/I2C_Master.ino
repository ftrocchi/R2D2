#include <Wire.h>
#include <Sabertooth.h>
#include <SendOnlySoftwareSerial.h>
#include <SoftwareSerial.h>
#include "PS2.h"
#include "WavTrigger.h"

//-------------------------------------------------------------------------------------------
// PS2 DECLARATIONS
//-------------------------------------------------------------------------------------------
PS2 ps2 = PS2();

//-------------------------------------------------------------------------------------------
// MOTOR DECLARATIONS
//-------------------------------------------------------------------------------------------
#define MOTOR_PIN 7
SendOnlySoftwareSerial motorSerial(MOTOR_PIN);
Sabertooth domeMotor(128, motorSerial);
//Sabertooth footMotor(129, motorSerial);

//-------------------------------------------------------------------------------------------
// WAVTRIGGER DECLARATIONS
//-------------------------------------------------------------------------------------------
#define WAV_PIN 6
SendOnlySoftwareSerial wavSerial(WAV_PIN);
WAVTrigger wavTrigger(&wavSerial);

//-------------------------------------------------------------------------------------------
// SETUP AND LOOP
//-------------------------------------------------------------------------------------------
void setup()
{
    ps2.Init(9600, 8, 9);
    MotorSetup();
    WavTriggerSetup();
    
    // play the startup sound so we know we're ready to go
    wavTrigger.TrackPlaySolo(52);
}

void loop()
{
    ProcessPS2();
    
    ProcessDomeMotor();
//    ProcessFootMotor();

    delay(20);
}

//-------------------------------------------------------------------------------------------
// I2C FUNCTIONS
//-------------------------------------------------------------------------------------------
void I2CSetup()
{
//    Wire.begin();
}

//-------------------------------------------------------------------------------------------
// PS2 CONTROLLER FUNCTIONS
//-------------------------------------------------------------------------------------------
void ProcessPS2()
{
    ps2.Update();
}

//-------------------------------------------------------------------------------------------
// MOTOR FUNCTIONS
//-------------------------------------------------------------------------------------------
void MotorSetup()
{
    motorSerial.begin(9600);
    domeMotor.autobaud();
//    footMotor.autobaud();
}

void ProcessDomeMotor()
{
    int value = map(ps2.GetStickValue(PS2_STATE_LX), 0, 255, -127, 127);
    
    // anything within 20 of 0 consider it to be zero
    if (abs(value) <= 20)
        value = 0;
    
    domeMotor.motor(value);
}

void ProcessFootMotor()
{
//    int driveValue = map(ps2.GetStickValue(PS2_STATE_RX), 0, 255, -127, 127);
//    int turnValue = map(ps2.GetStickValue(PS2_STATE_RY), 0, 255, -127, 127);
    
//    footMotor.drive(driveValue);
//    footMotor.turn(turnValue);
}

//-------------------------------------------------------------------------------------------
// WAVTRIGGER FUNCTIONS
//-------------------------------------------------------------------------------------------
void WavTriggerSetup()
{
    wavSerial.begin(57600);
    delay(2000);
    wavTrigger.SetMasterVolume(200);
}


