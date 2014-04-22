#ifndef __PS2CONTROL_H
#define __PS2CONTROL_H

struct PS2Control
{
    enum Value
    {
        LX = 0,
        LY,
        RX,
        RY,
        BLUE,
        RED,
        GREEN,
        PINK,
        PAD_DOWN,
        PAD_RIGHT,
        PAD_UP,
        PAD_LEFT,
        SELECT,
        START,
        L1,
        R1,
        L2,
        R2,
        L3,
        R3
    };
};

#endif
