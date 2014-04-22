#include <Arduino.h>
#include <Wire.h>
#include <SendOnlySoftwareSerial.h>
#include "WAVTrigger.h"
#include "I2C_Common.h"

#define WAV_PIN 3
#define LED_PIN 4

SendOnlySoftwareSerial wavSerial(WAV_PIN);
WAVTrigger wavTrigger(&wavSerial);

long ledStartTime;
int ledDelay = 500;
bool ledIsOn = false;

void setup()
{
    // setup the wav board
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    wavSerial.begin(57600);
    delay(2000);
    wavTrigger.SetMasterVolume(200);
    
    // setup I2C
    Wire.begin(I2C_DeviceAddress::WAV);
    Wire.onReceive(ReceiveEvent);
}

void loop()
{
    if (ledIsOn)
    {
        if (ledStartTime + ledDelay < millis())
        {
            ledIsOn = false;
            digitalWrite(LED_PIN, LOW);
        }
    }
}

void ReceiveEvent(int eventCode)
{
    StartLED();
    I2C_WAV_Command::Value command = (I2C_WAV_Command::Value)Wire.read();
    int value = Wire.read();
    
    switch(command)
    {
        case I2C_WAV_Command::PlaySolo: wavTrigger.TrackPlaySolo(value); break;
        case I2C_WAV_Command::PlayPoly: wavTrigger.TrackPlayPoly(value); break;
        case I2C_WAV_Command::StopAll: wavTrigger.StopAllTracks(); break;
        case I2C_WAV_Command::SetMasterVolume: wavTrigger.SetMasterVolume(value); break;
    };
}

void StartLED()
{
    ledIsOn = true;
    ledStartTime = millis();
    digitalWrite(LED_PIN, HIGH);
}
