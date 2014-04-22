#include "Button.h"

Button::Button(uint8_t buttonPin)
{
    // init the pin
    pin = buttonPin;
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
    
    // init handlers
    pressHandlerCallback = 0;
    releaseHandlerCallback = 0;
    
    // init debounce
    debounceDuration = 50;
    debounceStartTime = millis();
    
    // init state
    state = 0;
    bitWrite(state, CURRENT, LOW);
}

void Button::PressHandler(buttonEventHandler handler)
{
    pressHandlerCallback = handler;
}

void Button::ReleaseHandler(buttonEventHandler handler)
{
    releaseHandlerCallback = handler;
}

void Button::Process()
{
    GetNewState();
    
    HandleStateChanges();
}

void Button::GetNewState()
{
    // save the previous value
    bitWrite(state, PREVIOUS, bitRead(state, CURRENT));
    
    // get the current status
    bitWrite(state, CURRENT, digitalRead(pin) != HIGH);
}

void Button::HandleStateChanges()
{
    if (bitRead(state, CURRENT) != bitRead(state, PREVIOUS))
    {
        if (DebounceEarlyExit())
            return;

        CallCallbacks();
    }    
}

bool Button::DebounceEarlyExit()
{   
    if (millis() - debounceStartTime < debounceDuration)
        return true;
            
    debounceStartTime = millis();
    
    return false;
}

void Button::CallCallbacks()
{
    if (bitRead(state, CURRENT) == true && pressHandlerCallback)
        pressHandlerCallback(*this);
    else if (bitRead(state, CURRENT) == false && releaseHandlerCallback)
        releaseHandlerCallback(*this);    
}
