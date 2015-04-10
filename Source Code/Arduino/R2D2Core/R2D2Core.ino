#include "DebugHelper.h"
#include "I2C_Common.h"
#include "WavTrigger.h"

//------------------------------------------------------------------------------
// PS4 Includes
//------------------------------------------------------------------------------
#include "SPI.h"
#include "PS4BT.h"

//------------------------------------------------------------------------------
// PS4 DECLARATIONS
//------------------------------------------------------------------------------
USB usb;
BTD btd(&usb);
PS4BT ps4(&btd);
int connectedState = 0;
unsigned long lastRumbleTime;

//------------------------------------------------------------------------------
// WAVTRIGGER DECLARATIONS
//------------------------------------------------------------------------------
#define WAV_PIN 6
#define WAV_STARTUP 138
#define WAV_NOTOUCH 139
#define WAV_RANDOM_MILLISECONDS 20000
SendOnlySoftwareSerial wavSerial(WAV_PIN);
WavTrigger wavTrigger(&wavSerial);
byte masterVolume;
bool enteringWavCode;
byte wavCode;

// wav trigger random releated
bool isWavRandomOn;
unsigned long wavLastTimeCheck;

//------------------------------------------------------------------------------
// SETUP AND LOOP
//------------------------------------------------------------------------------
void setup() {
    debugPrint("Begin");
    
    debugPrint("Initializing Usb");
    if (usb.Init() == -1) {
        debugPrint("Usb initialization failure!  Halting!");
        while (1);
    }
    debugPrint("PS4 Bluetooth Library Started");

    // setup modules    
    wavTriggerSetup();
    
    // play the startup sound
    wavCode = WAV_STARTUP;
    
    debugPrint("End");
}

void loop() {
    // every loop cycle process usb task
    usb.Task();
    
    if (ps4.connected()) {
        processConnectedRumble();

        // process modules    
        processWavTrigger();
    }
}

//------------------------------------------------------------------------------
// PS4 FUNCTIONS
//------------------------------------------------------------------------------
void processConnectedRumble() {
    if (connectedState < 4) {
        unsigned long timeNow = millis();
        
        if (timeNow - lastRumbleTime > 200) {
            debugPrint("Connected rumble state: %d", connectedState);
            switch (connectedState) {
                case 0: ps4.setLed(0, 0, 0); ps4.setRumbleOn(255, 255); break;
                case 1: ps4.setRumbleOn(0, 0); break;
                case 2: ps4.setRumbleOn(255, 255); break;
                case 3: ps4.setRumbleOn(0, 0); break;
            }
            
            connectedState++;
            lastRumbleTime = millis();
        }
    }
}

//------------------------------------------------------------------------------
// WAVTRIGGER FUNCTIONS
//------------------------------------------------------------------------------
void wavTriggerSetup() {
    debugPrint("Begin");
    wavSerial.begin(57600);
    delay(2000);
    wavTrigger.ampPower(false);
    masterVolume = 150;
    wavTrigger.setMasterVolume(masterVolume);
    enteringWavCode = false;
    debugPrint("End");
}

void processWavTrigger() {
    // play random sound
    if (isWavRandomOn) {
        unsigned long timeNow = millis();
        if (timeNow - wavLastTimeCheck > WAV_RANDOM_MILLISECONDS) {
            wavLastTimeCheck = timeNow;
            int trackToPlay = random(40) + 13;
            debugPrint("Playing random track %d", trackToPlay);
            wavTrigger.trackPlaySolo(trackToPlay);
        }
    }
    
    // encoding and playing sound manually
    if (ps4.getButtonClick(R1)) {
        debugPrint("Begin entering code");
        enteringWavCode = true;
        wavCode = 0;
    }
    
    if (enteringWavCode) {
        if (ps4.getButtonClick(CROSS))
            wavCode = wavCode << 2;
            
        if (ps4.getButtonClick(CIRCLE)) {
            wavCode = wavCode << 2;
            wavCode += 1;
        }
        
        if (ps4.getButtonClick(TRIANGLE)) {
            wavCode = wavCode << 2;
            wavCode += 2;
        }
        
        if (ps4.getButtonClick(SQUARE)) {
            wavCode = wavCode << 2;
            wavCode += 3;
        }
    }
    
    if (!ps4.getButtonPress(R1) && enteringWavCode) {
        debugPrint("End entering code: %d", wavCode);
        enteringWavCode = false;
        
        if (wavCode == 0)
            wavTrigger.stopAllTracks();
            
        wavTrigger.trackPlaySolo(wavCode);
    }
    
    // volume control
    if (ps4.getButtonClick(UP) && masterVolume < 250) {
        masterVolume += 5;
        debugPrint("Volume Up - set to: %d", masterVolume);
        wavTrigger.setMasterVolume(masterVolume);
    } else if (ps4.getButtonClick(DOWN) && masterVolume > 0) {
        masterVolume -= 5;
        debugPrint("Volume Down - set to: %d", masterVolume);
        wavTrigger.setMasterVolume(masterVolume);
    }
    
    // no touch
    if (ps4.getButtonClick(SHARE)) {
        debugPrint("NO TOUCH");
        wavTrigger.trackPlaySolo(WAV_NOTOUCH);
    }
    
    // toggle random
    if (ps4.getButtonClick(R2)) {
        isWavRandomOn = !isWavRandomOn;
        debugPrint("isWavRandomOn set to %d", isWavRandomOn);
        wavLastTimeCheck = millis() - WAV_RANDOM_MILLISECONDS;
    }
}
