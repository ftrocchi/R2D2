#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>

#define CURRENT 0
#define PREVIOUS 1

class Button;
typedef void (*buttonEventHandler)(Button&);

class Button
{
    private:
        buttonEventHandler pressHandlerCallback;
        buttonEventHandler releaseHandlerCallback;
        uint8_t state;
        uint8_t pin;
        int debounceDuration;
        unsigned long debounceStartTime;
    
    public:
        Button(uint8_t buttonPin);
        
        void PressHandler(buttonEventHandler handler);
        void ReleaseHandler(buttonEventHandler handler);
        
        void Process();
        
        void GetNewState();
        void HandleStateChanges();
        bool DebounceEarlyExit();
        void CallCallbacks();
};

#endif
