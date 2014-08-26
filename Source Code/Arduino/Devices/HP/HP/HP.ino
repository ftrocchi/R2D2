#include <Wire.h>

#define PIN_RED 10
#define PIN_GREEN 11
#define PIN_BLUE 9

byte currentRed = 0;
byte currentGreen = 0;
byte currentBlue = 0;
void currentMode = 20;

#define HP 25    // 25=Front FHP (BLUE), 26=Top THP (GREEN), 27=Rear RHP (RED)

void setup()
{
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    
    Wire.begin(HP);
    setColor( (HP == 27) * 255, 	// red - Rear
	          (HP == 26) * 255, 	// green - Top
			  (HP == 25) * 255);	// blue - Front
    setLEDOn();
    delay(5000);
	setLEDOff();
    
    Wire.onReceive(receiveEvent);
}

void loop()
{
	switch (currentMode)
	{
		case 20: animateOff(); break;
		case 21: animateOn(); break;
		case 22: animateAlarm(); break;
		case 23: animateLeia(); break;
		case 24: animateDisco(); break;
		case 25: animateFaliure(); break;
		
		default:
			break;
	}
}

void setColor(byte redValue, byte greenValue, byte blueValue)
{
    currentRed = redValue;
	currentGreen = greenValue;
	currentBlue = blueValue;
}

void setLEDOn()
{
    digitalWrite(PIN_RED, currentRed);
    digitalWrite(PIN_GREEN, currentGreen);
    digitalWrite(PIN_BLUE, currentBlue);
}

void setLEDOff()
{
    digitalWrite(PIN_RED, 0);
    digitalWrite(PIN_GREEN, 0);
    digitalWrite(PIN_BLUE, 0);
}

void animateOff()
{
    setLEDOff();
}

void animateOn()
{
    setLEDOn();
}

void animateAlarm()
{
}

void animateLeia()
{
}

void animateDisco()
{
}

void animateFailyure()
{
}

void receiveEvent(int eventCode)
{
    byte cmd = Wire.read();
    
    switch (cmd) 
    {
	    // set the color
        case 1: setColor(0,0,0); break;
        case 2: setColor(255,255,255); break;
        case 3: setColor(255,0,0); break;
        case 4: setColor(0,255,0); break;
        case 5: setColor(0,0,255); break;
        case 6: setColor(255,0,255); break;
        case 7: setColor(255,255,0); break;
        case 8: setColor(0,255,255); break;
		
		// set the activity
		case 20: SetLEDOff(); break;
		case 21: SetLEDOn(); break;
		
            
        default:
            break;
    }
}


