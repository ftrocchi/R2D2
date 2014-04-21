#ifndef __WAV_TRIGGER_H__
#define __WAV_TRIGGER_H__

#include <arduino.h>
#include <SendOnlySoftwareSerial.h>

#define CMD_TRACK_CONTROL      3
#define CMD_STOP_ALL           4
#define CMD_VOLUME             5

#define TRACK_PLAY_SOLO        0
#define TRACK_PLAY_POLY        1

class WAVTrigger
{
private:
  void TrackControl(int track, int code);
  SendOnlySoftwareSerial *wavSerial;
  
public:
  WAVTrigger(SendOnlySoftwareSerial *wavSendOnlySoftwareSerial);
  void TrackPlaySolo(int track);
  void TrackPlayPoly(int track);
  void StopAllTracks();
  void SetMasterVolume(int volume);
};

#endif

