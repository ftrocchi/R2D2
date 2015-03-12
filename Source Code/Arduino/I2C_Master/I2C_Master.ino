#include <Wire.h>
#include <Sabertooth.h>
#include <SendOnlySoftwareSerial.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Ethernet.h>

#define MAX_FRAME_LENGTH 64

#include <WebSocket.h>
#include "PS2.h"
#include "WavTrigger.h"

//-------------------------------------------------------------------------------------------
// WEBSOCKET DECLARATIONS
//-------------------------------------------------------------------------------------------
#define SOURCE_PS2 1
#define SOURCE_TABLET 2
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
Sabertooth footMotor(129, motorSerial);

//-------------------------------------------------------------------------------------------
// WAVTRIGGER DECLARATIONS
//-------------------------------------------------------------------------------------------
#define WAV_PIN 6
SendOnlySoftwareSerial wavSerial(WAV_PIN);
WAVTrigger wavTrigger(&wavSerial);
bool enteringWAVCode;
byte wavCode;
byte masterVolume;
#define WAV_STARTUP 138
#define WAV_NOTOUCH 139
#define WAV_RANDOM_BEGIN 13
#define WAV_RANDOM_END 53
#define WAV_RANDOM_MILLISECONDS 20000
bool isWavRandomOn;
unsigned long wavLastTimeCheck;

//-------------------------------------------------------------------------------------------
// SETUP AND LOOP
//-------------------------------------------------------------------------------------------
void setup()
{
    randomSeed(analogRead(0));
    WebSocketSetup();
    ps2.Init(9600, 8, 9);
    MotorSetup();
    WavTriggerSetup();
    I2CSetup();
    
    // play the startup sound so we know we're ready to go
    wavCode = WAV_STARTUP;
}

void loop()
{
    webSocket.listen();
    
    ProcessPS2();
    
    ProcessWavTrigger();
    
    ProcessDomeMotor(SOURCE_PS2);
    ProcessFootMotor();

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
        //Serial.print("COMMAND RECEIVED: ");
        //Serial.println(command[0]);
        Wire.beginTransmission(command[0]);
        for (int i=1; i < index; i++)
        {
            //Serial.println(command[i]);
            Wire.write(command[i]);
        }
            
        //Serial.println("ENDING");
        Wire.endTransmission();
        //Serial.println("ENDED");
        return;
    }
    
    // 128 is the dome motor
    if (command[0] == 128)
    {
        ProcessDomeMotor(SOURCE_TABLET, command[1]);
        return;
    }
    
    // 129 is the foot motor, but we are going to ignore it
    if (command[0] == 129)
        return;
        
    // 130 is body sound
    if (command[0] == 130)
    {
        if (command[1] == 0)
            wavTrigger.StopAllTracks();
        else
            wavTrigger.TrackPlaySolo(command[1]);
        return;
    }
    
    // 131 is volume
    if (command[0] == 131)
    {
        masterVolume = command[1];
        AdjustMasterVolume(masterVolume, false);
        return;
    }
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
    motorSerial.begin(9600);
    domeMotor.autobaud();
    footMotor.autobaud();
}

void ProcessDomeMotor(int source)
{
    ProcessDomeMotor(source, 127);
}

void ProcessDomeMotor(int source, int value)
{
    // if the source is the PS2 controller, and it is moving, 
    // then use it.
    if (source == SOURCE_PS2)
    {
        if (!ps2.IsControllerIdle())
        {
            MoveDome(ps2.GetStickValue(PS2_STATE_LX));
            return;
        }
    }
    
    if (source == SOURCE_TABLET && ps2.IsControllerIdle())
        MoveDome(value);
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
    
    int driveValue = map(ps2.GetStickValue(PS2_STATE_RX), 0, 255, -64, 64);
    int turnValue = map(ps2.GetStickValue(PS2_STATE_RY), 0, 255, -64, 64);

    footMotor.drive(driveValue);
    footMotor.turn(turnValue);
}

//-------------------------------------------------------------------------------------------
// WAVTRIGGER FUNCTIONS
//-------------------------------------------------------------------------------------------
void WavTriggerSetup()
{
    wavSerial.begin(57600);
    delay(2000);
    wavTrigger.AmpPower(false);
    masterVolume = 150;
    wavTrigger.SetMasterVolume(masterVolume);
    enteringWAVCode = false;
}

void ProcessWavTrigger()
{
    // play a random sound if we should
    if (isWavRandomOn)
    {
        // check if it is time to play
        unsigned long timeNow = millis();
        
        if (timeNow - wavLastTimeCheck > WAV_RANDOM_MILLISECONDS)
        {
            // time to play
            wavLastTimeCheck= timeNow;
            
            // generate a random number
            int trackToPlay = random(40) + 13;
            
            // play it
            wavTrigger.TrackPlaySolo(trackToPlay);
        }
    }
    
    if (ps2.IsButtonJustPressed(PS2_STATE_R1))
    {
        enteringWAVCode = true;
        wavCode = 0;
    }
        
    if (enteringWAVCode)
    {
        // use bit shifting to bring in the code
        if (ps2.IsButtonJustPressed(PS2_STATE_CROSS)) 
            wavCode = wavCode << 2;

        if (ps2.IsButtonJustPressed(PS2_STATE_CIRCLE)) 
        {
            wavCode = wavCode << 2;
            wavCode += 1;
        }
        
        if (ps2.IsButtonJustPressed(PS2_STATE_TRIANGLE)) 
        {
            wavCode = wavCode << 2;
            wavCode += 2;
        }
        
        if (ps2.IsButtonJustPressed(PS2_STATE_SQUARE) )
        {
            wavCode = wavCode << 2;
            wavCode += 3;
        }
    }
    
    if (ps2.IsButtonJustReleased(PS2_STATE_R1))
    {
        enteringWAVCode = false;
        
        if (wavCode == 0)
            wavTrigger.StopAllTracks();
        
        // play it here!
        wavTrigger.TrackPlaySolo(wavCode);
    }
    
    if (ps2.IsButtonPressed(PS2_STATE_PAD_UP) && masterVolume < 250)
    {
        masterVolume+=5;
        AdjustMasterVolume(masterVolume, true);
    }
    else  if (ps2.IsButtonPressed(PS2_STATE_PAD_DOWN) && masterVolume > 5)
    {
        masterVolume-=5;
        AdjustMasterVolume(masterVolume, true);
    }
    
    if (ps2.IsButtonJustPressed(PS2_STATE_START))
    {
        wavTrigger.TrackPlaySolo(WAV_NOTOUCH);
    }
    
    if (ps2.IsButtonJustPressed(PS2_STATE_R2))
    {
        isWavRandomOn = !isWavRandomOn;
        wavLastTimeCheck = millis() - WAV_RANDOM_MILLISECONDS;
    }
}

void AdjustMasterVolume(byte newVolume, bool updateTablet)
{
    if (updateTablet && webSocket.isConnected())
    {
        char buf[7];
        sprintf(buf, "131/%03u", masterVolume);
        webSocket.send(buf, 7);
    }    
    
    wavTrigger.SetMasterVolume(masterVolume);
}
