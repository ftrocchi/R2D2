#include "HPLed.h"

HPLed::HPLed(int red, int green, int blue)
{
    redPin = red;
    greenPin = green;
    bluePin = blue;
    
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    currentColorRed = LOW;
    currentColorGreen = LOW;
    currentColorBlue = LOW;
    
    currentMode = I2C_HP_Command::Off;
}

void HPLed::Update()
{
    switch (currentMode)
    {
        case I2C_HP_Command::Off:
            SetLedOff();
            break;
            
        case I2C_HP_Command::On:
            SetLedOn();
            break;
            
        case I2C_HP_Command::Alarm:
            AnimateAlarm();
            break;
			
		case I2C_HP_Command::Leia:
			AnimateLeia();
			break;
			
		case I2C_HP_Command::Disco:
			AnimateDisco();
			break;
			
		case I2C_HP_Command::Failure:
			AnimateFailure();
			break;
            
        default:
            break;
    }
}

void HPLed::SetColor(bool red, bool green, bool blue)
{
    digitalWrite(redPin, red);
    digitalWrite(greenPin, green);
    digitalWrite(bluePin, blue);
}

void HPLed::SetCurrentColor(I2C_HP_Command::Value command)
{
    switch (command)
    {
        case I2C_HP_Command::Black:
            currentColorRed = false;
            currentColorGreen = false;
            currentColorBlue = false;
            break;
            
        case I2C_HP_Command::White:
            currentColorRed = true;
            currentColorGreen = true;
            currentColorBlue = true;
            break;
            
        case I2C_HP_Command::Red:
            currentColorRed = true;
            currentColorGreen = false;
            currentColorBlue = false;
            break;
            
        case I2C_HP_Command::Green:
            currentColorRed = false;
            currentColorGreen = true;
            currentColorBlue = false;
            break;
            
        case I2C_HP_Command::Blue:
            currentColorRed = false;
            currentColorGreen = false;
            currentColorBlue = true;
            break;
            
        case I2C_HP_Command::Magenta:
            currentColorRed = true;
            currentColorGreen = false;
            currentColorBlue = true;
            break;
            
        case I2C_HP_Command::Yellow:
            currentColorRed = true;
            currentColorGreen = true;
            currentColorBlue = false;
            break;
            
        case I2C_HP_Command::Cyan:
            currentColorRed = false;
            currentColorGreen = true;
            currentColorBlue = true;
            break;
    }
}

void HPLed::SetMode(I2C_HP_Command::Value command)
{
    switch (command)
    {
        case I2C_HP_Command::Off:
        case I2C_HP_Command::On:
            currentMode = command;
            break;
            
        case I2C_HP_Command::Alarm:
	case I2C_HP_Command::Disco:
            currentMode = command;
            currentModeState = 1;
            break;
			
	case I2C_HP_Command::Failure:
		currentMode = command;
		currentModeState = 0;
		break;
            
	case I2C_HP_Command::Leia:
		currentMode = command;
		currentModeState = 0;
		leiaDelay = 0;
		break;

        default:
            break;
    }
}

void HPLed::SetLedOff()
{
    SetColor(LOW, LOW, LOW);
}

void HPLed::SetLedOn()
{
    SetColor(currentColorRed, currentColorGreen, currentColorBlue);
}

void HPLed::AnimateAlarm()
{
    if (!IsTimeForStateChange(250))
        return;
        
    if (currentModeState == 1)
        SetLedOn();
    else
        SetLedOff();
        
    currentModeState *= -1;
}

void HPLed::AnimateLeia()
{
	if (!IsTimeForStateChange(leiaDelay))
        return;
		
	switch (currentModeState)
	{
		case 0: SetColor(LOW, HIGH, HIGH); break;
		case 1: SetColor(HIGH, HIGH, HIGH); break;
		case 2: SetColor(LOW, LOW, HIGH); break;
	}
	
	leiaDelay = random(1,10) * 10;
	
	currentModeState++;
	
	if (currentModeState == 2)
		currentModeState = 0;
}

void HPLed::AnimateDisco()
{
    if (!IsTimeForStateChange(500))
        return;
        
    if (currentModeState == 1)
        SetColor(HIGH, HIGH, HIGH);
    else
        SetLedOff();
        
    currentModeState *= -1;
}

void HPLed::AnimateFailure()
{
	if (!IsTimeForStateChange(250))
	    return;
		
	switch (currentModeState)
	{
		case 0: SetColor(HIGH, LOW, LOW); break;
		case 1: SetColor(LOW, HIGH, LOW); break;
		case 2: SetColor(LOW, LOW, HIGH); break;
		case 3: SetColor(HIGH, LOW, HIGH); break;
		case 4: SetColor(HIGH, HIGH, LOW); break;
		case 5: SetColor(LOW, HIGH, HIGH); break;
		case 6: SetMode(I2C_HP_Command::Off); break;
		
		default:
			break;
	}
	
	currentModeState++;
}

bool HPLed::IsTimeForStateChange(int delay)
{
    unsigned long timeNow = millis();
  
    if (timeNow - lastTimeCheck < delay)
        return false;

    lastTimeCheck = timeNow;

    return true;
}

