#include "PSI.h"

const int PSI::patternAtStage[9] = { 
    B01010101, B11010101, B10010101, B10110101, B10101010, B00101010, B01101010, B01010101, B01010101 };

PSI::PSI(I2C_DeviceAddress::Value address, LedControl *led, int first, int second, int transition) {
    i2cAddress = address;
    ledControl = led;
    
    currentMode = I2C_PSI_Mode::Normal;
    isModeActive = true;
    
    ledControl->shutdown(0, false);
    ledControl->clearDisplay(0);
    ledControl->setIntensity(0, 15);    

    resetModes(first, second, transition);
}

void PSI::processCommand() {
    I2C_PSI_Command::Value command = (I2C_PSI_Command::Value)Wire.read();
  
    switch (command) 
    {
        case I2C_PSI_Command::On:
            on();
            break;
      
        case I2C_PSI_Command::Off:
            off();
            break;
      
        case I2C_PSI_Command::Brightness:
            setBrightness(Wire.read());
            break;

        case I2C_PSI_Command::Mode:
            setMode((I2C_PSI_Mode::Value)Wire.read());
            break;
    }
}

void PSI::update() {
    if (!isModeActive)
        return;

    switch (currentMode)
    {
        case I2C_PSI_Mode::Normal:
            animateNormal();
            break;

        case I2C_PSI_Mode::March:
            animateMarch();
            break;

        case I2C_PSI_Mode::Spin:
            animateSpin();
            break;

        case I2C_PSI_Mode::Ring:
            animateRing();
            break;

        case I2C_PSI_Mode::UpDown:
            animateUpDown();
            break;

        case I2C_PSI_Mode::LeftRight:
            animateLeftRight();
            break;
    }    
}


// ----------------------------------------------------------------------------
// COMMANDS
// ----------------------------------------------------------------------------
void PSI::on() 
{
    isModeActive = false;
    for (int row = 0; row < 6; row ++)
        ledControl->setRow(0, row, 255);
}

void PSI::off() 
{
    isModeActive = false;
    ledControl->clearDisplay(0);
}

void PSI::setBrightness(int brightness) 
{
    ledControl->setIntensity(0, brightness);  
}

void PSI::setMode(I2C_PSI_Mode::Value mode)
{
    isModeActive = true;
    currentMode = mode;
}

// ----------------------------------------------------------------------------
// MODES
// ----------------------------------------------------------------------------
// NORMAL
// ----------------------------------------------------------------------------
void PSI::resetModes(int first, int second, int transition) 
{
    delayAtStage[0] = first;
    delayAtStage[1] = transition / 3; 
    delayAtStage[2] = delayAtStage[1];
    delayAtStage[3] = delayAtStage[1]; 
    delayAtStage[4] = second; 
    delayAtStage[5] = delayAtStage[1]; 
    delayAtStage[6] = delayAtStage[1]; 
    delayAtStage[7] = delayAtStage[1]; 
    delayAtStage[8] = first; 
  
    stage = 0;
    slideDirection = 1;
    maxStage = 8;
    lastTimeCheck = 0;

    // march
    firstColor = true;

    // spin
    spinState = 0;

    // ring
    ringState = 0;
    ringDirection = 1;

    // up down
    upDownState = 0;
    upDownDirection = 1;

    // left right
    leftRightState = 0;
    leftRightDirection = 1;
}

void PSI::animateNormal() 
{
    unsigned long timeNow = millis();
  
    // early exit if we don't need to do anything
    if (timeNow - lastTimeCheck < delayAtStage[stage])
        return;
  
    // set the time  
    lastTimeCheck = timeNow;
  
    // move to next stage
    stage += slideDirection;
  
    // bounds check for stage
    if (stage >= maxStage) {
        stage = maxStage;
        slideDirection = -1;
    } 
    else if (stage <= 0) 
    {
        stage = 0;
        slideDirection = 1;
    }
  
    // set the stage pattern
    ledControl->setRow(0, 0, patternAtStage[stage]);
    ledControl->setRow(0, 1, ~patternAtStage[stage]);
    ledControl->setRow(0, 2, patternAtStage[stage]);
    ledControl->setRow(0, 3, ~patternAtStage[stage]);
    ledControl->setRow(0, 4, patternAtStage[stage]);
}

// ----------------------------------------------------------------------------
// MARCH
// ----------------------------------------------------------------------------
void PSI::animateMarch()
{
    if (!IsTimeForStateChange(250))
        return;

    if (firstColor)
    {
        ledControl->setRow(0, 0, patternAtStage[0]);
        ledControl->setRow(0, 1, ~patternAtStage[0]);
        ledControl->setRow(0, 2, patternAtStage[0]);
        ledControl->setRow(0, 3, ~patternAtStage[0]);
        ledControl->setRow(0, 4, patternAtStage[0]);
    }
    else
    {
        ledControl->setRow(0, 0, patternAtStage[4]);
        ledControl->setRow(0, 1, ~patternAtStage[4]);
        ledControl->setRow(0, 2, patternAtStage[4]);
        ledControl->setRow(0, 3, ~patternAtStage[4]);
        ledControl->setRow(0, 4, patternAtStage[4]);
    }

    firstColor = !firstColor;
}


// ----------------------------------------------------------------------------
// SPIN
// ----------------------------------------------------------------------------
void PSI::animateSpin() 
{
    if (!IsTimeForStateChange(50))
        return;

    switch (spinState) 
    {
        case 0 : ledControl->setRow(0, 0, B01000000); break;
        case 1 : ledControl->setRow(0, 1, B10000000); break;
        case 2 : ledControl->setRow(0, 2, B10000000); break;
        case 3 : ledControl->setRow(0, 3, B10000000); break;
        case 4 : ledControl->setRow(0, 4, B01000000); break;
        case 5 : ledControl->setRow(0, 4, B00100000); break;
        case 6 : ledControl->setRow(0, 4, B00010000); break;
        case 7 : ledControl->setRow(0, 4, B00001000); break;
        case 8 : ledControl->setRow(0, 3, B00000100); break;
        case 9 : ledControl->setRow(0, 2, B00000100); break;
        case 10: ledControl->setRow(0, 1, B00000100); break;
        case 11: ledControl->setRow(0, 0, B00001000); break;
        case 12: ledControl->setRow(0, 0, B00010000); break;
        case 13: ledControl->setRow(0, 0, B00100000); break;
    };

    spinState++;
    if (spinState == 14)
        spinState = 0;
}

// ----------------------------------------------------------------------------
// RING
// ----------------------------------------------------------------------------
void PSI::animateRing()
{
    if (!IsTimeForStateChange(250))
        return;

    switch (ringState)
    {
        case 0: 
            ledControl->setRow(0, 0, B01111000);
            ledControl->setRow(0, 1, B10000100);
            ledControl->setRow(0, 2, B10000100);
            ledControl->setRow(0, 3, B10000100);
            ledControl->setRow(0, 4, B01111000);
            break;

        case 1: 
            ledControl->setRow(0, 0, B00000000);
            ledControl->setRow(0, 1, B01111000);
            ledControl->setRow(0, 2, B01001000);
            ledControl->setRow(0, 3, B01111000);
            ledControl->setRow(0, 4, B00000000);
            break;

        case 2: 
            ledControl->setRow(0, 0, B00000000);
            ledControl->setRow(0, 1, B00000000);
            ledControl->setRow(0, 2, B00110000);
            ledControl->setRow(0, 3, B00000000);
            ledControl->setRow(0, 4, B00000000);
            break;
    }

    ringState += ringDirection;
    if (ringState < 0)
    {
        ringState = 1;
        ringDirection = 1;
    } 
    else if (ringState > 2)
    {
        ringState = 1;
        ringDirection = -1;
    }
}

// ----------------------------------------------------------------------------
// UP / DOWN
// ----------------------------------------------------------------------------
void PSI::animateUpDown()
{
    if (!IsTimeForStateChange(250))
        return;

    ledControl->setRow(0, upDownState, B11111111);

    upDownState += upDownDirection;
    if (upDownState < 0)
    {
        upDownState = 1;
        upDownDirection = 1;
    } 
    else if (upDownState > 4)
    {
        upDownState = 3;
        upDownDirection = -1;
    }
}

// ----------------------------------------------------------------------------
// LEFT / RIGHT
// ----------------------------------------------------------------------------
void PSI::animateLeftRight()
{
    if (!IsTimeForStateChange(250))
        return;

    switch (leftRightState)
    {
        case 0: 
            ledControl->setRow(0, 0, B10000000);
            ledControl->setRow(0, 1, B10000000);
            ledControl->setRow(0, 2, B10000000);
            ledControl->setRow(0, 3, B10000000);
            ledControl->setRow(0, 4, B10000000);
            break;

        case 1: 
            ledControl->setRow(0, 0, B01000000);
            ledControl->setRow(0, 1, B01000000);
            ledControl->setRow(0, 2, B01000000);
            ledControl->setRow(0, 3, B01000000);
            ledControl->setRow(0, 4, B01000000);
            break;

        case 2: 
            ledControl->setRow(0, 0, B00100000);
            ledControl->setRow(0, 1, B00100000);
            ledControl->setRow(0, 2, B00100000);
            ledControl->setRow(0, 3, B00100000);
            ledControl->setRow(0, 4, B00100000);
            break;

        case 3: 
            ledControl->setRow(0, 0, B00010000);
            ledControl->setRow(0, 1, B00010000);
            ledControl->setRow(0, 2, B00010000);
            ledControl->setRow(0, 3, B00010000);
            ledControl->setRow(0, 4, B00010000);
            break;

        case 4: 
            ledControl->setRow(0, 0, B00001000);
            ledControl->setRow(0, 1, B00001000);
            ledControl->setRow(0, 2, B00001000);
            ledControl->setRow(0, 3, B00001000);
            ledControl->setRow(0, 4, B00001000);
            break;

        case 5: 
            ledControl->setRow(0, 0, B00000100);
            ledControl->setRow(0, 1, B00000100);
            ledControl->setRow(0, 2, B00000100);
            ledControl->setRow(0, 3, B00000100);
            ledControl->setRow(0, 4, B00000100);
            break;
    };

    leftRightState += leftRightDirection;
    if (leftRightState < 0)
    {
        leftRightState = 1;
        leftRightDirection = 1;
    }
    else if (leftRightState > 5)
    {
        leftRightState = 4;
        leftRightDirection = -1;
    }
}

bool PSI::IsTimeForStateChange(int delay)
{
    unsigned long timeNow = millis();
  
    // early exit if we don't need to do anything
    if (timeNow - lastTimeCheck < delay)
        return false;

    // set the time  
    lastTimeCheck = timeNow;

    // clear the device
    ledControl->clearDisplay(0);

    return true;
}
