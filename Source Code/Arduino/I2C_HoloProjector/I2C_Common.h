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
        SystemEvent = 99,
    };
};

struct I2C_HP_Servo {
    enum Value {
        XServo = 0,
        YServo = 1,
    };
};

struct I2C_HP_ServoMode {
  enum Value {
      // no value
      TopLeft = 0,
      Top = 1,
      TopRight = 2,
      Left = 3,
      Center = 4,
      Right = 5,
      BottomLeft = 6,
      Bottom = 7, 
      BottomRight = 8,
      TwitchOff = 9,
      TwicthOn = 10,
      // single value
      HorizontalOnly = 100,
      VerticalOnly = 101,
      // doubel value
      HorizontalAndVertical = 200,
  };  
};


#endif
