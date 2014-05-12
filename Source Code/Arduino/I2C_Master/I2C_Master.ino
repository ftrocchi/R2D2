#include <Wire.h>
#include <Sabertooth.h>
#include <SendOnlySoftwareSerial.h>
#include <SoftwareSerial.h>
#include "I2C_Common.h"
#include "PS2Control.h"

//-------------------------------------------------------------------------------------------
// MOTOR DECLARATIONS
//-------------------------------------------------------------------------------------------
#define DOME_MOTOR_PIN 11
#define FOOT_MOTOR_PIN 12
SendOnlySoftwareSerial domeSerial(DOME_MOTOR_PIN);
SendOnlySoftwareSerial footSerial(FOOT_MOTOR_PIN);
Sabertooth domeMotor(128, domeSerial);
Sabertooth footMotor(128, footSerial);
bool isDomeMoving = false;

//-------------------------------------------------------------------------------------------
// PS2 CONTROLLER DECLARATIONS
//-------------------------------------------------------------------------------------------
#define MAX_ARRAY_LENGTH 20
byte gamePadState[MAX_ARRAY_LENGTH];

//-------------------------------------------------------------------------------------------
// SETUP AND LOOP
//-------------------------------------------------------------------------------------------
void setup()
{
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
    Wire.requestFrom(I2C_DeviceAddress::PS2, 20);

    int index=0;
    while (Wire.available())
    {
        gamePadState[index] = Wire.read();
        index++;
    }
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
    int value = map(gamePadState[PS2Control::LX], 0, 255, -127, 127);
    
    if (isDomeMoving && value == 0)
    {
        // stop sound
        StopAllTracks();
        
        isDomeMoving = false;
    }
    else if (!isDomeMoving && value !=0)
    {
        // start sound
        PlayTrackPoly(102);
        
        isDomeMoving = true;
    }
    
    domeMotor.motor(value);
}

void ProcessFootMotor()
{
    int driveValue = map(gamePadState[PS2Control::RY], 0, 255, -127, 127);
    int turnValue = map(gamePadState[PS2Control::RX], 0, 255, -127, 127);
    
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


