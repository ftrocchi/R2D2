#ifndef __PS2_H__
#define __PS2_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

// defines here

class PS2
{
    private:
        bool SERIAL_ERR;
    
    
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
