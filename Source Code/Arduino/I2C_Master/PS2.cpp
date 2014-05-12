#include "PS2.h"

PS2::PS2()
{
}

void PS2::Init(long baudrate, byte receivePin, byte transmitPin)
{
    pinMode(receivePin, INPUT);
    pinMode(transmitPin, OUTPUT);
    ps2Serial = new SoftwareSerial(receivePin, transmitPin);
    ps2Serial->begin(baudrate);
    pinMode(A1, OUTPUT);
    digitalWrite(A1, HIGH);
}

void PS2::Uart_Send(byte data)
{
    while(ps2Serial->available() > 0)
    {
        ps2Serial->read();
    }
    
    ps2Serial->write(data);
}

byte PS2::Uart_Recv()
{
    byte receivedData;
    long waitCount = 0;
    
    while(true)
    {
        if (ps2Serial->available() > 0)
        {
            receivedData = ps2Serial->read();
            SERIAL_ERR = false;
            return receivedData;
        }
        
        waitCount++;
        if (waitCount > 50000)
        {
            SERIAL_ERR = true;
            return 0xFF;
        }
    }
}

byte PS2::GetValue(byte key)
{
    Uart_Send(key);
    return Uart_Recv();
}
