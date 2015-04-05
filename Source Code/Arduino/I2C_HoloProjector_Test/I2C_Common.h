#ifndef __I2C_COMMON_H__
#define __I2C_COMMON_H__

//------------------------------------------------------------------------------
// SYSTEM WIDE 
//------------------------------------------------------------------------------
struct I2C_DeviceAddress {
    enum Value {
        FrontHP = 25,
        TopHP = 26,
        RearHP = 27,
    };
};

struct I2C_SystemEvent {
    enum Value {
        Off = 0,
        LeiaMessage = 1,
        ImperialMarch = 2,
        TwitchHPColor =3,
    };
};

//------------------------------------------------------------------------------
// HOLO PROJECTORS
//------------------------------------------------------------------------------
struct I2C_HP_Color {
    enum Value {
        Off = 0,
        Red = 1,
        Green = 2,
        Yellow = 3,
        Blue = 4,
        Magenta = 5,
        Cyan = 6,
        White = 7
    };
};

struct I2C_HP_Mode {
    enum Value {
        Color = 0,
        SystemEvent = 99,
    };
};

#endif
