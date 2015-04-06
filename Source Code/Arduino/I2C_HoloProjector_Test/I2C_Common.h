#ifndef __I2C_COMMON_H__
#define __I2C_COMMON_H__

//------------------------------------------------------------------------------
// SYSTEM WIDE 
//------------------------------------------------------------------------------
struct I2C_DeviceAddress {
    enum Value {
        MagicPanel = 20,
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

//------------------------------------------------------------------------------
// MAGIC PANEL
//------------------------------------------------------------------------------
struct I2C_MagicPanel_Command
{
    enum Value 
    {
        On = 0,
        Off = 1,
        Brightness = 2,
        Mode = 3,
        Random = 4
    };
};

struct I2C_MagicPanel_Mode
{
    enum Value
    {
        Alert = 0,
        ToggleTopBottom = 1,
        ToggleLeftRight = 2,
        ToggleQuad = 3,
        QuadCycleCounterClockwise = 4,
        QuadCycleClockwise = 5,
        TraceUp = 6,
        TraceDown = 7,
        TraceUpDown = 8,
        TraceLeft = 9,
        TraceRight = 10,
        TraceLeftRight = 11,
        SingleLEDTest = 12,
        DoubleLEDTest = 13,
        RandomPixel = 14,
        String = 15
    };
};

#endif
