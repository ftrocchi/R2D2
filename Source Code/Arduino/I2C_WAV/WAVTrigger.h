#ifndef __WAV_TRIGGER_H__
#define __WAV_TRIGGER_H__

#include <arduino.h>
#include <SendOnlySoftwareSerial.h>

#define CMD_TRACK_CONTROL      3
#define CMD_STOP_ALL           4
#define CMD_VOLUME             5
#define CMD_TRACK_VOLUME 8
#define CMD_AMP_POWER 9

#define TRACK_PLAY_SOLO 0
#define TRACK_PLAY_POLY 1
#define TRACK_PAUSE 2
#define TRACK_RESUME 3
#define TRACK_STOP 4
#define TRACK_LOOP_ON 5
#define TRACK_LOOP_OFF 6

class WAVTrigger
{
    private:
        void TrackControl(int track, int code);
        SendOnlySoftwareSerial *wavSerial;
  
    public:
        WAVTrigger(SendOnlySoftwareSerial *wavSendOnlySoftwareSerial);
        
        void GetVersion(); // 0x01
        void GetSysInfo(); // 0x02
        
        // CONTROL_TRACK 0x03
        void TrackPlaySolo(int track); // 0x00
        void TrackPlayPoly(int track); // 0x01
        void TrackPause(int track); // 0x02
        void TrackResume(int track); // 0x03
        void TrackStop(int track); // 0x04
        void TrackLoopOn(int track); // 0x05
        void TrackLoopOff(int track); // 0x06
        
        void StopAllTracks(); // 0x04
        void SetMasterVolume(int volume); // 0x05
        
        void GetStatus(); // 0x07
        void TrackVolume(int track, int volume); // 0x08
        void AmpPower(byte isOn); // 0x09
        
};

#endif

