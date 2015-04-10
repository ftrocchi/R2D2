#ifndef __WAVTRIGGER_H__
#define __WAVTRIGGER_H__

#include "Arduino.h"
#include "SendOnlySoftwareSerial.h"

#define CMD_TRACK_CONTROL    3
#define CMD_STOP_ALL         4
#define CMD_VOLUME           5
#define CMD_TRACK_VOLUME     8
#define CMD_AMP_POWER        9

#define TRACK_PLAY_SOLO      0
#define TRACK_PLAY_POLY      1
#define TRACK_PAUSE          2
#define TRACK_RESUME         3
#define TRACK_STOP           4
#define TRACK_LOOP_ON        5
#define TRACK_LOOP_OFF       6

class WavTrigger {
    private:
        SendOnlySoftwareSerial *wavSerial;
        void trackControl(int track, int code);
    
    public:
        WavTrigger(SendOnlySoftwareSerial *wavSendOnlySoftwareSerial);
        
        void trackPlaySolo(int track);            // 0x03 - 0x00
        void stopAllTracks();                     // 0x04
        void setMasterVolume(int volume);         // 0x05        
        void ampPower(byte isOn);                 // 0x09
};


#endif
