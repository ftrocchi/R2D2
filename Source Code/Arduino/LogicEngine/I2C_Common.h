#ifndef __I2C_COMMON_H__
#define __I2C_COMMON_H__

struct I2C_Device_Address {
    enum Value {
        RearLogicDisplay = 10,
        FrontLogicDisplay = 11,
        MagicPanel = 20
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
        Spin = 4,
        Ring = 5
    };
};


#endif
