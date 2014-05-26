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
    
    lastTimeControllerActive = millis();
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
    {
        currentState[i] = ps2Serial->read();
    }
    
    return true;
}

void PS2::Vibrate(byte motor, byte value)
{
    if (motor != PS2_LARGE_MOTOR && motor != PS2_SMALL_MOTOR)
        return;
        
    Uart_Send(motor);
    Uart_Send(value);
}

// my functions here
void PS2::Update()
{
    strncpy(previousState, currentState, 6);
    GetAllValues();
}

byte PS2::GetStickValue(byte stick)
{
    if (stick != PS2_STATE_RX && stick != PS2_STATE_RY && stick != PS2_STATE_LX && stick != PS2_STATE_LY)
        return 0;
        
    return currentState[stick];
}

bool PS2::GetButtonBit(char* state, byte button)
{
    if (button > PS2_STATE_SQUARE) 
        return false;
        
    return (state[button / 8] & (1 << button % 8)) > 0;
}

bool PS2::IsButtonPressed(byte button)
{
    return GetButtonBit(currentState, button);
}

bool PS2::IsButtonReleased(byte button)
{
    return !IsButtonPressed(button);
}

bool PS2::IsButtonJustPressed(byte button)
{
    bool current = GetButtonBit(currentState, button);
    bool previous = GetButtonBit(previousState, button);
    
    return (current && !previous);
}

bool PS2::IsButtonJustReleased(byte button)
{
    bool current = GetButtonBit(currentState, button);
    bool previous = GetButtonBit(previousState, button);

    return (!current && previous);    
}

bool PS2::IsStickCentered(byte stick)
{
    if (stick != PS2_STATE_RX && stick != PS2_STATE_RY && stick != PS2_STATE_LX && stick != PS2_STATE_LY)
        return false;
     
     return abs(GetStickValue(stick) - PS2_STICK_CENTERED_VALUE) <= PS2_STICK_CENTERED_TOLERANCE;
}

bool PS2::IsControllerIdle()
{
    return lastTimeControllerActive - millis() > PS2_NOT_USED_INTERVAL;
}

void PS2::DetermineActivity()
{
    bool isActive = false;
    // if any stick is not centered then the controller is active
    if (!IsStickCentered(PS2_STATE_RX) ||
        !IsStickCentered(PS2_STATE_RY) ||
        !IsStickCentered(PS2_STATE_LX) ||
        !IsStickCentered(PS2_STATE_LY))
        isActive = true;
        
    // now check the buttons
    if (currentState[0] > 0 || currentState[1] > 0)
        isActive = true;
    
    // if active is true, reset the timer
    if (isActive)
        lastTimeControllerActive = millis();
}

