#include <Wire.h>
#include <Sabertooth.h>
#include <SendOnlySoftwareSerial.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#define MAX_FRAME_LENGTH 64

#include <WebSocket.h>
#include "PS2.h"
#include "WavTrigger.h"

//-------------------------------------------------------------------------------------------
// WEBSOCKET DECLARATIONS
//-------------------------------------------------------------------------------------------
byte mac[6];
byte ip[4];
WebSocket webSocket;

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
// WEBSOCKET FUNCTIONS
//-------------------------------------------------------------------------------------------
void WebSocketSetup()
{
    byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x5F, 0xFB };
    byte ip[] = { 192, 168, 1, 82 };
    
    Ethernet.begin(mac, ip);
    
    webSocket.registerConnectCallback(&OnConnect);
    webSocket.registerDataCallback(&OnData);
    webSocket.registerDisconnectCallback(&OnDisconnect);
    
    webSocket.begin();
    
    delay(100);    
}

void OnConnect(WebSocket &socket)
{
}

void OnDisconnect(WebSocket &socket)
{
}

void OnData(WebSocket &socket, char* dataString, byte frameLength)
{
    // split the data
    byte command[12];
    int index = 0;
    char *savePointer;
    char *token;
    
    for (token = strtok_r(dataString, "/", &savePointer); token; token = strtok_r(NULL, "/", &savePointer))
    {
        command[index] = atoi(token);
        index++;
    }
    
    // if it is less than 128 it is an i2c command so send it over i2c
    if (command[0] < 128)
    {
        Wire.beginTransmission(command[0]);
        for (int i=1; i < index; i++)
            Wire.write(command[index]);
        Wire.endTransmission();
        return;
    }
    
    // 128 is the dome motor
    if (command[0] == 128)
    {
        MoveDome(command[1]);
        return;
    }
    
    // 129 is the foot motor, but we are going to ignore it
    if (command[0] == 129)
        return;
        
    // 130 is body sound
    if (command[0] == 130)
    {
        return;
    }
    
    // 131 is volume
    if (command[0] == 131)
    {
        return;
    }
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
    MoveDome(ps2.GetStickValue(PS2_STATE_LX));
}

void MoveDome(int value)
{
    int mappedValue = map(value, 0, 255, -127, 127);
    
    // anything within 20 of 0 consider it to be zero
    if (abs(mappedValue) <= 20)
        mappedValue = 0;
    
    domeMotor.motor(mappedValue);
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


