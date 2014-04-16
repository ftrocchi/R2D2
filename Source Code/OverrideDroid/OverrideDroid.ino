#include <Arduino.h>
#include "Button.h"

#define FOOT_STATE 0
#define DOME_STATE 1
#define MUTE_STATE 2

int footButtonPin = 4;
int footLEDPin = 5;
int domeButtonPin = 6;
int domeLEDPin = 7;
int muteButtonPin = 8;
int muteLEDPin = 9;
int battButtonPin = 10;
int battLEDPin = 11;

Button footButton = Button(footButtonPin);
Button domeButton = Button(domeButtonPin);
Button muteButton = Button(muteButtonPin);
Button battButton = Button(battButtonPin);

uint8_t overrideState;

// HANDLERS
void OnPressFoot(Button& b)
{
    Serial.println("ONPRESSFOOT");
    FlipOverrideState(FOOT_STATE);
    SetLEDs();
    ParkFoot(bitRead(overrideState, FOOT_STATE));
}

void OnPressDome(Button& b)
{
    Serial.println("ONPRESSDOME");
    FlipOverrideState(DOME_STATE);
    SetLEDs();    
    ParkDome(bitRead(overrideState, DOME_STATE));
}

void OnPressMute(Button& b)
{
    Serial.println("ONPRESSMUTE");
    FlipOverrideState(MUTE_STATE);
    SetLEDs();
    MuteAudio(bitRead(overrideState, MUTE_STATE));
}

void OnPressBatt(Button& b)
{
    Serial.println("ONPRESSBATT");
}

// COMMON
void FlipOverrideState(int state)
{
    bitWrite(overrideState, state, !bitRead(overrideState, state));
}

void SetLEDs()
{
    digitalWrite(footLEDPin, bitRead(overrideState, FOOT_STATE) == 1 ? HIGH : LOW);
    digitalWrite(domeLEDPin, bitRead(overrideState, DOME_STATE) == 1 ? HIGH : LOW);
    digitalWrite(muteLEDPin, bitRead(overrideState, MUTE_STATE) == 1 ? HIGH : LOW);
}

// FOOT
void ParkFoot(bool shouldPark)
{
    // issue command to sabertooth 2x25
}

// DOME
void ParkDome(bool shouldPark)
{
    // issue command to syren 10  
}

// MUTE
void MuteAudio(bool shouldMute)
{
    // issue command via I2C to mute audio
}

void setup()
{
    Serial.begin(9600);
    Serial.println("BEGIN");
    
    // setup pins
    pinMode(footLEDPin, OUTPUT);
    pinMode(domeLEDPin, OUTPUT);
    pinMode(muteLEDPin, OUTPUT);
    pinMode(battLEDPin, OUTPUT);
    
    // setup handlers
    footButton.PressHandler(OnPressFoot);
    domeButton.PressHandler(OnPressDome);
    muteButton.PressHandler(OnPressMute);
    battButton.PressHandler(OnPressBatt);
    
    // setup override state
    overrideState = 7;  // all on
    
    // set the LEDs
    SetLEDs();
}

void loop()
{
    // process buttons
    footButton.Process();
    domeButton.Process();
    muteButton.Process();
    battButton.Process();
    
    // xbee communications here
}
