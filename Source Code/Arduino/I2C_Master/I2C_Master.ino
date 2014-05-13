#include <Wire.h>
#include <Sabertooth.h>
#include <SendOnlySoftwareSerial.h>
#include <SoftwareSerial.h>
#include "PS2.h"
#include "I2C_Common.h"

//-------------------------------------------------------------------------------------------
// PS2 DECLARATIONS
//-------------------------------------------------------------------------------------------
PS2 ps2 = PS2();

//-------------------------------------------------------------------------------------------
// MOTOR DECLARATIONS
//-------------------------------------------------------------------------------------------
#define DOME_MOTOR_PIN 11
#define FOOT_MOTOR_PIN 12
SendOnlySoftwareSerial domeSerial(DOME_MOTOR_PIN);
SendOnlySoftwareSerial footSerial(FOOT_MOTOR_PIN);
Sabertooth domeMotor(128, domeSerial);
Sabertooth footMotor(128, footSerial);

//-------------------------------------------------------------------------------------------
// SETUP AND LOOP
//-------------------------------------------------------------------------------------------
void setup()
{
    ps2.Init(115200, 8, 9);
    MotorSetup();
    I2CSetup();
    
    delay(5000);
    
    // play the startup sound so we know we're ready to go
    PlayTrackSolo(52);
}

void loop()
{
    ProcessPS2();
    
    ProcessDomeMotor();
    ProcessFootMotor();

    delay(20);
}

//-------------------------------------------------------------------------------------------
// I2C FUNCTIONS
//-------------------------------------------------------------------------------------------
void I2CSetup()
{
    Wire.begin();
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
    domeSerial.begin(9600);
    footSerial.begin(9600);
    domeMotor.autobaud();
    footMotor.autobaud();
}

void ProcessDomeMotor()
{
    int value = map(ps2.GetStickValue(PS2_STATE_LX), 0, 255, -127, 127);
    
    domeMotor.motor(value);
}

void ProcessFootMotor()
{
    int driveValue = map(ps2.GetStickValue(PS2_STATE_RX), 0, 255, -127, 127);
    int turnValue = map(ps2.GetStickValue(PS2_STATE_RY), 0, 255, -127, 127);
    
    footMotor.drive(driveValue);
    footMotor.turn(turnValue);
}

//-------------------------------------------------------------------------------------------
// WAV FUNCTIONS
//-------------------------------------------------------------------------------------------
void PlayTrackSolo(int trackNumber)
{
    Wire.beginTransmission(I2C_DeviceAddress::WAV);
    Wire.write((byte)I2C_WAV_Command::PlaySolo);
    Wire.write(trackNumber);
    Wire.endTransmission();
}

void PlayTrackPoly(int trackNumber)
{
    Wire.beginTransmission(I2C_DeviceAddress::WAV);
    Wire.write((byte)I2C_WAV_Command::PlayPoly);
    Wire.write(trackNumber);
    Wire.endTransmission();
}

void StopAllTracks()
{
    Wire.beginTransmission(I2C_DeviceAddress::WAV);
    Wire.write((byte)I2C_WAV_Command::StopAll);
    Wire.write(0);
    Wire.endTransmission();
}


