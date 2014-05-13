#ifndef __PS2_H__
#define __PS2_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

// defines here
#define PS2_SELECT 0
#define PS2_L3 1
#define PS2_R3 2
#define PS2_START 3
#define PS2_PAD_UP 4
#define PS2_PAD_RIGHT 5
#define PS2_PAD_DOWN 6
#define PS2_PAD_LEFT 7
#define PS2_L2 8
#define PS2_R2 9
#define PS2_L1 10
#define PS2_R1 11
#define PS2_TRIANGLE 12
#define PS2_CIRCLE 13
#define PS2_CROSS 14
#define PS2_SQUARE 15
#define PS2_LX 16
#define PS2_LY 17
#define PS2_RX 18
#define PS2_RY 19
#define PS2_L_UP 20
#define PS2_L_DOWN 21
#define PS2_L_LEFT 22
#define PS2_L_RIGHT 23
#define PS2_R_UP 24
#define PS2_R_DOWN 25
#define PS2_R_LEFT 26
#define PS2_R_RIGHT 27
#define PS2_CONNECTION_STATUS 28
#define PS2_SMALL_MOTOR 29
#define PS2_LARGE_MOTOR 30
#define PS2_GET_ALL 31

class PS2
{
    private:
        char currentState[6];
    
    public:
        PS2();
        
        void Init(long baudrate, byte receivePin, byte transmitPin);
        byte GetValue(byte key);
        bool GetAllValues();
        void Vibrate(byte motor, byte value);
        void Reset(byte reset);

    protected:
        SoftwareSerial* ps2Serial;
        void Uart_Send(byte data);
        byte Uart_Recv(void);
};

#endif
