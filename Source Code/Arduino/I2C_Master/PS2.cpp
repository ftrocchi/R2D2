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
        ps2Serial->read();
    
    ps2Serial->write(data);
}

byte PS2::Uart_Recv()
{
    long waitCount = 0;
    
    while(true)
    {
        if (ps2Serial->available() > 0)
            return ps2Serial->read();
        
        waitCount++;
        
        if (waitCount > 50000)
            return 0xFF;
    }
}

byte PS2::GetValue(byte key)
{
    Uart_Send(key);
    return Uart_Recv();
}

bool PS2::GetAllValues()
{
    long waitCount = 0;
    
    Uart_Send(PS2_GET_ALL);
    
    while (ps2Serial->available() < 6)
    {
        waitCount++;
        if (waitCount > 50000)
            return false;
    }
    
    for (int i=0; i<6; i++)
        currentState[i] = ps2Serial->read();
    
    return true;
}

void PS2::Vibrate(byte motor, byte value)
{
    if (motor != PS2_LARGE_MOTOR && motor != PS2_SMALL_MOTOR)
        return;
        
    Uart_Send(motor);
    Uart_Send(value);
}

void PS2::Reset(byte reset)
{
    digitalWrite(A1, reset == 1 ? LOW : HIGH);
}
