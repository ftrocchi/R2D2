#ifndef __I2C_COMMON_H__
#define __I2C_COMMON_H__

struct I2C_DeviceAddress
{
    enum Value
    {
        WAV = 110
    };
};

struct I2C_WAV_Command
{
    enum Value
    {
        PlaySolo = 0,
        PlayPoly = 1,
        StopAll = 2,
        SetMasterVolume = 3
    };
};

#endif

