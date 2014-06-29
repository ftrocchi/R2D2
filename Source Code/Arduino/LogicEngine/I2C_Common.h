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

struct I2C_Logic_Mode {
    enum Value {
        Normal = 0
    };
};

#endif
