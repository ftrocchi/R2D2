#ifndef __I2C_COMMON_H__
#define __I2C_COMMON_H__

struct I2C_Device_Address {
    enum Value {
        RearLogicDisplay = 10,
        FrontLogicDisplay = 11,
        MagicPanel = 20,
        FrontHoloprojector = 25,
        TopHoloprojector = 26,
        RearHoloprojector = 27
    };
};

struct I2C_Logic_Device {
    enum Value {
        Logic = 0,
        PSI = 1
    };
};

struct I2C_PSI_Command {
    enum Value {
        On = 0,
        Off = 1,
        Brightness = 2,
        Mode = 3
    };
};

struct I2C_PSI_Mode {
    enum Value {
        Normal = 0,
        March = 1, 
        Spin = 2,
        Ring = 3,
        UpDown = 4,
        LeftRight = 5
    };
};

struct I2C_Logic_Command {
    enum Value {
        Brightness = 0,
        Mode = 1
    };
};

struct I2C_Logic_Display_Selection {
    enum Value {
        FLDTop = 0,
        FLDBottom = 1,
        FLDBoth = 2,
        RLD = 3,
        All= 4
    };
};

struct I2C_Logic_Mode {
    enum Value {
        On = 0,
        Off = 1,
        Normal = 2,
        March = 3,
        March_Separate = 4,
        Color_Shift = 5,
        Left_Shift = 6,
        Right_Shift = 7,
        Left_Right_Shift = 8,
        Down_Shift = 9,
        Down_Shift_Separate = 10,
        Up_Shift = 11,
        Up_Shift_Separate = 12,
        Up_Down_Shift = 13,
        Up_Down_Shift_Separate = 14,
        Random_Pixel = 19,
        Text = 20
    };
};

struct I2C_HP_Command {
    enum Value {
        Black = 1,
        White = 2,
        Red = 3,
        Green = 4,
        Blue = 5,
        Magenta = 6,
        Yellow = 7,
        Cyan = 8,
        CustomColor = 9,
        Off = 20,
        On = 21,
        Alarm = 22,
        Leia = 23,
        Disco = 24,
        Failure = 25,
        XLeft = 40,
        XCenter = 41,
        XRight = 42,
        YTop = 43,
        YCenter = 44,
        YBottom = 45,
        XCustom = 46,
        YCustom = 47,
        TopLeft = 50,
        TopCenter = 51,
        TopRight = 52,
        CenterLeft = 53,
        CenterCenter = 54,
        CenterRight = 55,
        BottomLeft = 56,
        BottomCenter = 57,
        BottomRight = 58,
        Both = 59,
        TwitchOff = 60,
        TwitchOn = 61,
        TwitchFrequency = 62
    };
};
    
#endif

