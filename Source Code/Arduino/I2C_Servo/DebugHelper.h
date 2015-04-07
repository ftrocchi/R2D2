// DebugHelper
//
// Use DEBUGPRINT, which takes standard printf formats, to print debug info
// 
// NOTE: This will run Serial.begin(115200) automatically

#ifndef __DEBUGHELPER__
#define __DEBUGHELPER__

// comment out this line to turn off debugging
#define USEDEBUGHELPER

#include <stdarg.h>
#include <Arduino.h>

#ifndef USEDEBUGHELPER
    #define debugPrint(fmt, ...)
#else
    #define debugPrint(fmt, ...) debugHelperPrintf(__FUNCTION__, __FILE__, fmt, __LINE__, ##__VA_ARGS__)
    #define DEBUGHELPERBUFFER 16
    
    static bool hasSerialBeginBeenCalled = false;
    
    inline const char* getFileName(const char* s) {
        // find the last index
        int lastIndex = -1;
        int index = 0;
        while (s[index] != '\0') {
            if (s[index] == '\\')
                lastIndex = index + 1;
            index++;
        }
        
        int length = strlen(s);
        
        if (lastIndex != -1) {
            char* to = (char*)malloc(length-lastIndex+1);
            strncpy(to, s + lastIndex, length-lastIndex);
            to[length-lastIndex] = '\0';
            return to;
        }
        
        return s;
    }

    inline void debugHelperPrintf(const char* function, const char* fileWithPath, const char *fmt, int line, ...) {
        // first time this is called setup the serial
        if (!hasSerialBeginBeenCalled) {
            Serial.begin(115200);
            hasSerialBeginBeenCalled = true;
        }
        
        // take the fileWithPath and get only the file
        const char* file = getFileName(fileWithPath);
        
        // print the debug information first
        Serial.print("DEBUG|");
        Serial.print(function);
        Serial.print("|");
        Serial.print(getFileName(fileWithPath));
        Serial.print("(");
        Serial.print(line);
        Serial.print(")|");
        
        // count the number of parameters in the string
        int parameterCount = 0;
        for (int i=0; fmt[i] != '\0'; i++)
            if (fmt[i] == '%')
                parameterCount++;
                
        // if the parameter count is zero, just send it out directly
        if (parameterCount == 0) {
            Serial.println(fmt);
            return;
        }
        
        char buffer[DEBUGHELPERBUFFER + 1];

        va_list argv;
        va_start(argv, parameterCount);
        
        int bufferPosition = 0;
        
        for (int i=0; fmt[i] != '\0'; i++) {
            // look for a paramter which starts with %
            if (fmt[i] == '%') {
                
                // dump what we have to the serial output
                buffer[bufferPosition] = '\0';
                Serial.print(buffer);
                
                // reset the buffer
                bufferPosition=0;
                buffer[bufferPosition] = '\0';
                
                switch (fmt[++i]) {
                    case 'd': Serial.print(va_arg(argv, int)); break;
                    case 'l': Serial.print(va_arg(argv, long)); break;
                    case 'f': Serial.print(va_arg(argv, double)); break;
                    case 'c': Serial.print((char)va_arg(argv, int)); break;
                    case 's': Serial.print(va_arg(argv, char*)); break;
                    default: break;
                }
            }
            else {
                // add to the buffer
                buffer[bufferPosition] = fmt[i];
                
                // if the buffer is full, dump the buffer to serial, then reset it
                bufferPosition = (bufferPosition+1)%DEBUGHELPERBUFFER;
                if (bufferPosition == 0) {
                    buffer[DEBUGHELPERBUFFER] = '\0';
                    Serial.print(buffer);
                    buffer[bufferPosition] = '\0';   
                }
            }
        }
        
        Serial.println();
    }

#endif

#endif

