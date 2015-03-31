#include "WAVTrigger.h"

WAVTrigger::WAVTrigger(SendOnlySoftwareSerial *wavSendOnlySoftwareSerial) {
    wavSerial = wavSendOnlySoftwareSerial;
}

void WAVTrigger::TrackPlaySolo(int track) 
{
    TrackControl(track, TRACK_PLAY_SOLO);
}

void WAVTrigger::TrackPlayPoly(int track) 
{
    TrackControl(track, TRACK_PLAY_POLY);
}

void WAVTrigger::TrackPause(int track)
{
    TrackControl(track, TRACK_PAUSE);
}

void WAVTrigger::TrackResume(int track)
{
    TrackControl(track, TRACK_RESUME);
}

void WAVTrigger::TrackStop(int track)
{
    TrackControl(track, TRACK_STOP);
}

void WAVTrigger::TrackLoopOn(int track)
{
    TrackControl(track, TRACK_LOOP_ON);
}

void WAVTrigger::TrackLoopOff(int track)
{
    TrackControl(track, TRACK_LOOP_OFF);
}

void WAVTrigger::StopAllTracks() 
{
    wavSerial->write(0xf0);
    wavSerial->write(0xaa);
    wavSerial->write(0x05);
    wavSerial->write(CMD_STOP_ALL);
    wavSerial->write(0x55);
}

void WAVTrigger::SetMasterVolume(int volume) 
{
    int mappedVolume = map(volume, 0, 255, -70, 10);  
    
    if (mappedVolume > 200)
        mappedVolume = 200;
    
    wavSerial->write(0xf0);
    wavSerial->write(0xaa);
    wavSerial->write(0x07);
    wavSerial->write(CMD_VOLUME);
    wavSerial->write((byte)mappedVolume);
    wavSerial->write((byte)(mappedVolume >> 8));
    wavSerial->write(0x55);
}

void WAVTrigger::TrackVolume(int track, int volume)
{
    int mappedVolume = map(volume, 0, 255, -70, 10);  
    
    if (mappedVolume > 200)
        mappedVolume = 200;
    
    wavSerial->write(0xf0);
    wavSerial->write(0xaa);
    wavSerial->write(0x09);
    wavSerial->write(CMD_TRACK_VOLUME);
    wavSerial->write((byte)track);
    wavSerial->write((byte)(track >> 8));
    wavSerial->write((byte)mappedVolume);
    wavSerial->write((byte)(mappedVolume >> 8));
    wavSerial->write(0x55);
}

void WAVTrigger::AmpPower(byte isOn)
{
    wavSerial->write(0xf0);
    wavSerial->write(0xaa);
    wavSerial->write(0x06);
    wavSerial->write(CMD_AMP_POWER);
    wavSerial->write(isOn);
    wavSerial->write(0x55);
}

void WAVTrigger::TrackControl(int track, int code) 
{
    wavSerial->write(0xf0);
    wavSerial->write(0xaa);
    wavSerial->write(0x08);
    wavSerial->write(CMD_TRACK_CONTROL);
    wavSerial->write((byte)code);
    wavSerial->write((byte)track);
    wavSerial->write((byte)(track >> 8));
    wavSerial->write(0x55);
}





