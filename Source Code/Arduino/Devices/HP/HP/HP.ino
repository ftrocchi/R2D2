#include <Wire.h>

#define redPin 10
#define greenPin 11
#define bluePin 9

#define HP 25    // 25=Front FHP (BLUE), 26=Top THP (GREEN), 27=Rear RHP (RED)

void setup()
{
    pinMode(redPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    
    Wire.begin(HP);
    setLED( (HP == 27) * 255, (HP == 26) * 255, (HP == 25) * 255);
    delay(5000);
    setLED(0,0,0);
    
    Wire.onReceive(receiveEvent);
}

void loop()
{
}

void setLED(byte redValue, byte greenValue, byte blueValue)
{
    digitalWrite(redPin, redValue);
    digitalWrite(greenPin, greenValue);
    digitalWrite(bluePin, blueValue);
}

void receiveEvent(int eventCode)
{
    byte cmd = Wire.read();
    
    switch (cmd) 
    {
        case 1: setLED(0,0,0); break;
        case 2: setLED(255,255,255); break;
        case 3: setLED(255,0,0); break;
        case 4: setLED(0,255,0); break;
        case 5: setLED(0,0,255); break;
        case 6: setLED(255,0,255); break;
        case 7: setLED(255,255,0); break;
        case 8: setLED(0,255,255); break;
            
        default:
            break;
    }
}


