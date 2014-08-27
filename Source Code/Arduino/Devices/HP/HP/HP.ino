#include <Wire.h>
#include "HPLed.h"
#include "I2C_Common.h"

#define PIN_RED 10
#define PIN_GREEN 11
#define PIN_BLUE 9

byte currentRed = 0;
byte currentGreen = 0;
byte currentBlue = 0;
void currentMode = 20;

#define HP 25    // 25=Front FHP (BLUE), 26=Top THP (GREEN), 27=Rear RHP (RED)

HPLed hpLed = HPLed(redPin, greenPin, bluePin);

void setup()
{
<<<<<<< HEAD
    Wire.begin(HP);
    
    hpLed.SetCurrentColor(  HP == 27 ? I2C_HP_Command::Red : HP == 26 ? I2C_HP_Command::Green : I2C_HP_Command::Blue);
    hpLed.SetMode(I2C_HP_Command::On);
    hpLed.Update();
    delay(5000);
    hpLed.SetMode(I2C_HP_Command::Off);
    hpLed.Update();
=======
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
>>>>>>> origin/master
    
    Wire.onReceive(receiveEvent);
}

void loop()
{
<<<<<<< HEAD
    hpLed.Update();
=======
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
>>>>>>> origin/master
}

void receiveEvent(int eventCode)
{
    I2C_HP_Command::Value command = (I2C_HP_Command::Value)Wire.read();
    
    switch (command)
    {
<<<<<<< HEAD
        case I2C_HP_Command::Black:
        case I2C_HP_Command::White:
        case I2C_HP_Command::Red:
        case I2C_HP_Command::Green:
        case I2C_HP_Command::Blue:
        case I2C_HP_Command::Magenta:
        case I2C_HP_Command::Yellow:
        case I2C_HP_Command::Cyan:
            hpLed.SetCurrentColor(command);
            break;
=======
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
		
>>>>>>> origin/master
            
        case I2C_HP_Command::Off:
        case I2C_HP_Command::On:
        case I2C_HP_Command::Alarm:
            hpLed.SetMode(command);
            break;
           
        default:
            break;
    }
}

