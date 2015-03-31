#include <PS2X_lib.h>
#include <Wire.h>
#include "I2C_Common.h"

#define MAX_ARRAY_LENGTH 20
#define PS2_CLK 15
#define PS2_CMD 16
#define PS2_ATT 14
#define PS2_DAT 10

PS2X ps2x;
byte gamePadState[MAX_ARRAY_LENGTH];

void setup()
{
    if (SetupGamePad() != 0)
    {
        while (true)
        {
            TXLED1;
            delay(100);
            TXLED0;
            delay(100);
        }
    }
    
    Wire.begin(I2C_DeviceAddress::PS2);
    Wire.onRequest(RequestEvent);
}

void loop()
{
    ps2x.read_gamepad();
    
    // sticks
    gamePadState[0] = ps2x.Analog(PSS_LX);
    gamePadState[1] = ps2x.Analog(PSS_LY);
    gamePadState[2] = ps2x.Analog(PSS_RX);
    gamePadState[3] = ps2x.Analog(PSS_RY);
  
    // buttons
    gamePadState[4] = ps2x.Button(PSB_BLUE);
    gamePadState[5] = ps2x.Button(PSB_RED);
    gamePadState[6] = ps2x.Button(PSB_GREEN);
    gamePadState[7] = ps2x.Button(PSB_PINK);
  
    // dpad
    gamePadState[8] = ps2x.Button(PSB_PAD_DOWN);
    gamePadState[9] = ps2x.Button(PSB_PAD_RIGHT);
    gamePadState[10] = ps2x.Button(PSB_PAD_UP);
    gamePadState[11] = ps2x.Button(PSB_PAD_LEFT);
  
    // other
    gamePadState[12] = ps2x.Button(PSB_SELECT);
    gamePadState[13] = ps2x.Button(PSB_START);
    gamePadState[14] = ps2x.Button(PSB_L1);
    gamePadState[15] = ps2x.Button(PSB_R1);
    gamePadState[16] = ps2x.Button(PSB_L2);
    gamePadState[17] = ps2x.Button(PSB_R2);
    gamePadState[18] = ps2x.Button(PSB_L3);
    gamePadState[19] = ps2x.Button(PSB_R3);

    if (gamePadState[4] == true)
        TXLED1;
    else
        TXLED0;

    delay(10);
}

int SetupGamePad()
{
    return ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT, false, false);
}

void RequestEvent()
{
    Wire.write(gamePadState, MAX_ARRAY_LENGTH);
}

