#include <Arduino.h>
#include <SoftwareSerial.h>
#include <XBee.h>
#include "Button.h"
#include "SingleLED.h"

#define FOOT_STATE 0
#define DOME_STATE 1
#define MUTE_STATE 2

int footButtonPin = 4;
int footLEDPin = 5;
int footMotorPin = 6;

int domeButtonPin = 7;
int domeLEDPin = 8;
int domeMotorPin = 9;

int muteButtonPin = 10;
int muteLEDPin = 11;

int battButtonPin = 12;
int battLEDPin = 13;

Button footButton = Button(footButtonPin);
SingleLED footLED = SingleLED(footLEDPin, SINGLE_LED_ON);
Button domeButton = Button(domeButtonPin);
SingleLED domeLED = SingleLED(domeLEDPin, SINGLE_LED_ON);
Button muteButton = Button(muteButtonPin);
SingleLED muteLED = SingleLED(muteLEDPin, SINGLE_LED_ON);
Button battButton = Button(battButtonPin);
SingleLED battLED = SingleLED(battLEDPin, SINGLE_LED_ON);
uint8_t overrideState;

// xbee
SoftwareSerial dlineSerial(2,3);
XBee xbee = XBee();
Rx16Response rx16 = Rx16Response();
uint8_t xbeeOption = 0;
uint8_t payload[4];
Tx16Request tx = Tx16Request(0x8251, payload, sizeof(payload));

// HANDLERS
void OnPressFoot(Button& b)
{
    FlipAndPark(FOOT_STATE);
}

void OnPressDome(Button& b)
{
    FlipAndPark(DOME_STATE);
}

void OnPressMute(Button& b)
{
    FlipAndPark(MUTE_STATE);
}

void OnPressBatt(Button& b)
{
}

// COMMON
void FlipAndPark(int state)
{
    FlipOverrideState(state);
    Park(state, bitRead(overrideState, state));
}

void FlipOverrideState(int state)
{
    bitWrite(overrideState, state, !bitRead(overrideState, state));
}

void SetOverrideState(int state, bool isOverride)
{
    bitWrite(overrideState, state, isOverride);
}

void SetLEDs()
{
    footLED.SetNewMode(!bitRead(overrideState, FOOT_STATE));
    domeLED.SetNewMode(!bitRead(overrideState, DOME_STATE));
    muteLED.SetNewMode(!bitRead(overrideState, MUTE_STATE));
}

void Park(int state, bool shouldPark)
{
    switch (state)
    {
        case FOOT_STATE: ParkFoot(shouldPark); break;
        case DOME_STATE: ParkDome(shouldPark); break;
        case MUTE_STATE: MuteAudio(shouldPark); break;
    }
}

// FOOT
void ParkFoot(bool shouldPark)
{
    // issue command to sabertooth 2x25
    digitalWrite(footMotorPin, shouldPark ? LOW : HIGH);
    
    SetLEDs();
}

// DOME
void ParkDome(bool shouldPark)
{
    // issue command to syren 10 
    digitalWrite(domeMotorPin, shouldPark ? LOW : HIGH);
   
   SetLEDs();
}

// MUTE
void MuteAudio(bool shouldMute)
{
    // issue command via I2C to mute audio
    SetLEDs();
}

void setup()
{
    // setup handlers
    footButton.PressHandler(OnPressFoot);
    domeButton.PressHandler(OnPressDome);
    muteButton.PressHandler(OnPressMute);
    battButton.PressHandler(OnPressBatt);
    
    // setup override state
    overrideState = 7;  // all on
    
    // set initial 
    ParkFoot(true);
    ParkDome(true);
    MuteAudio(true);
    
    // setup xbee
    dlineSerial.begin(9600);
    xbee.setSerial(dlineSerial);
}

void loop()
{
    // process buttons
    footButton.Process();
    domeButton.Process();
    muteButton.Process();
    battButton.Process();
    
    // update leds
    footLED.Update();
    
    // xbee communications here
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable())
    {
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE)
        {
            // get the data
            xbee.getResponse().getRx16Response(rx16);
            xbeeOption = rx16.getOption();
            for (int i=0; i<4; i++)
                payload[i] = rx16.getData(i);
                
            // do what is required by the received packet
            if (payload[0] != bitRead(overrideState, FOOT_STATE))
                FlipAndPark(FOOT_STATE);
                
            if (payload[1] != bitRead(overrideState, DOME_STATE))
                FlipAndPark(DOME_STATE);

            if (payload[2] != bitRead(overrideState, MUTE_STATE))
                FlipAndPark(MUTE_STATE);
            
            // prep the new packet
            payload[0] = bitRead(overrideState, FOOT_STATE);
            payload[1] = bitRead(overrideState, DOME_STATE);
            payload[2] = bitRead(overrideState, MUTE_STATE);
            payload[3] = 100; // battery
            
            // send the data back
            xbee.send(tx);
            
        }
    }
}
