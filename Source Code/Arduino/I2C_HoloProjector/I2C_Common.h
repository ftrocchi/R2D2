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
        Servo = 1,
    };
};

struct I2C_HP_Servo {
    enum Value {
        TopLeft = 0,
        Top = 1,
        TopRight = 2,
        Left = 3,
        Center = 4,
        Right = 5,
        BottomLeft = 6,
        Bottom = 7,
        BottomRight = 8,
        MoveTo = 9,
        TwitchOn = 10,
        TwitchOff = 11,
    };
};

#endif
