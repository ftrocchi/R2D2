#include "LogicDisplay.h"

void LogicDisplay::setup(I2C_Device_Address::Value address, bool isRLDLogic) {
    i2cAddress = address;
    isRLD = isRLDLogic;
    
    setMode(I2C_Logic_Display_Selection::All, I2C_Logic_Mode::Normal);
    setBrightness(255);
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, 96);
      
    delay(50);
    randomSeed(analogRead(0));
    
    if (isRLD) {
        tweenPause = 40;
        keyPause = 1200;
        keys = sizeof(rldColors) / 3;
        setPrimaryColor(I2C_Logic_Display_Selection::RLD);
    } else {
        tweenPause = 7;
        keyPause = 350;
        keys = sizeof(fldColors) / 3;
        setPrimaryColor(I2C_Logic_Display_Selection::FLDBoth);
    }
    
    totalColors = keys * tweens;
    
    pinMode(SPEEDPIN, INPUT);
    speeds = digitalRead(SPEEDPIN) == HIGH;
    
    generateAllColors();
    
    clear(TOP_FLD_RLD);
    clear(BOTTOM_FLD);

    // RLD TEST PATTERN HERE
    
    setInitialColors();
    
    showStartupAnimation();
}


void LogicDisplay::processCommand() {
}

void LogicDisplay::update() {
    // LOOPCHECK HERE
    
    switch (currentMode[TOP_FLD_RLD])
    {
        case I2C_Logic_Mode::On:
            animateOn(TOP_FLD_RLD);
            break;
            
        case I2C_Logic_Mode::Off:
            animateOff(TOP_FLD_RLD);
            break;            
            
        case I2C_Logic_Mode::Normal:
            animateNormal(TOP_FLD_RLD);
            break;
            
        case I2C_Logic_Mode::March:
        case I2C_Logic_Mode::March_Separate:
            animateMarch(TOP_FLD_RLD);
            break;
            
        case I2C_Logic_Mode::Color_Shift:
            animateColorShift(TOP_FLD_RLD);
            break;
            
        case I2C_Logic_Mode::Left_Shift:
            animateLeftShift(TOP_FLD_RLD);
            break;
            
        case I2C_Logic_Mode::Right_Shift:
            animateRightShift(TOP_FLD_RLD);
            break;
            
        case I2C_Logic_Mode::Left_Right_Shift:
            animateLeftRightShift(TOP_FLD_RLD);
            break;
            
        case I2C_Logic_Mode::Random_Pixel:
            animateRandomPixel(TOP_FLD_RLD);
            break;
            
        case I2C_Logic_Mode::Text:
            animateText(TOP_FLD_RLD);
            break;
    }        
    
    // if this is FLD, then also a mode for bottom
    if (!isRLD) {
        switch (currentMode[BOTTOM_FLD])
        {
            case I2C_Logic_Mode::On:
                animateOn(BOTTOM_FLD);
                break;
    
            case I2C_Logic_Mode::Off:
                animateOff(BOTTOM_FLD);
                break;            
    
            case I2C_Logic_Mode::Normal:
                animateNormal(BOTTOM_FLD);
                break;
                
            case I2C_Logic_Mode::March:
            case I2C_Logic_Mode::March_Separate:
                animateMarch(BOTTOM_FLD);
                break;
                
            case I2C_Logic_Mode::Color_Shift:
                animateColorShift(BOTTOM_FLD);
                break;
                
            case I2C_Logic_Mode::Left_Shift:
                animateLeftShift(BOTTOM_FLD);
                break;
                
            case I2C_Logic_Mode::Right_Shift:
                animateRightShift(BOTTOM_FLD);
                break;
                
            case I2C_Logic_Mode::Left_Right_Shift:
                animateLeftRightShift(BOTTOM_FLD);
                break;
                
            case I2C_Logic_Mode::Random_Pixel:
                animateRandomPixel(BOTTOM_FLD);
                break;

            case I2C_Logic_Mode::Text:
                animateText(BOTTOM_FLD);
                break;
        }
    }
}

void LogicDisplay::generateAllColors() {
    byte val;
    byte perStep;
    byte totalColorCount;
    
    for (byte kcol=0; kcol<keys;kcol++)
        for (byte el=0; el<3; el++) {
            if (isRLD) {
                perStep=int(pgm_read_byte(&rldColors[kcol+1][el])-pgm_read_byte(&rldColors[kcol][el]))/(tweens+1);
                val=pgm_read_byte(&rldColors[kcol][el]);
            } else {
                perStep=int(pgm_read_byte(&fldColors[kcol+1][el])-pgm_read_byte(&fldColors[kcol][el]))/(tweens+1);
                val=pgm_read_byte(&fldColors[kcol][el]);
            }
                
            byte tweenCount = 0;
            
            while (tweenCount <= tweens) {
                if (tweenCount == 0)
                    totalColorCount = kcol * (tweens + 1);
                allColors[totalColorCount][el] = val;
                tweenCount++;
                totalColorCount++;
                val = byte(val + perStep);
            }
        }
}

void LogicDisplay::setInitialColors() {
    for (byte x=0; x<96; x++) {
        LEDstat[x][0] = byte(random(totalColors));
        LEDstat[x][1] = random(2);
        if (LEDstat[x][0] % (tweens + 1) == 0)
            LEDstat[x][2] = random(keyPause);
        else
            LEDstat[x][2] = random(tweenPause);
    }
    
    for (byte x=0; x<96; x++) {
        if (isRLD)
            leds[pgm_read_byte(&rldMap[x])].setHSV(allColors[LEDstat[x][0]][0],allColors[LEDstat[x][0]][1],allColors[LEDstat[x][0]][2]);
        else if (x<80) leds[pgm_read_byte(&fldMap[x])].setHSV(allColors[LEDstat[x][0]][0],allColors[LEDstat[x][0]][1],allColors[LEDstat[x][0]][2]) ; 
    }
}

void LogicDisplay::showStartupAnimation() {
    for(byte x=0;x<96;x++) {
        if (isRLD) {
            leds[pgm_read_byte(&rldMap[x])] = CRGB::Green;
            if ((x+1)%24==0) { delay(200); FastLED.show(); }
        }  else {
            if (x<80) {
                leds[pgm_read_byte(&fldMap[x])] = CRGB::Blue;
                if ((x+1)%8==0) { delay(100); FastLED.show(); } 
            }         
        } 
    }
}

void LogicDisplay::clear(byte isTopOrBottom) {
    for(byte x=0;x<96;x++) 
        if (isRLD) 
            leds[pgm_read_byte(&rldMap[x])] = CRGB::Black;
        else if (x < 80 && ((isTopOrBottom == TOP_FLD_RLD && x < 40) || (isTopOrBottom == BOTTOM_FLD && x >= 40)))
            leds[pgm_read_byte(&fldMap[x])] = CRGB::Black;
            
    FastLED.show();           
}

void LogicDisplay::setPrimaryColor(I2C_Logic_Display_Selection::Value display, byte hue, byte saturation, byte value) {
    if (display == I2C_Logic_Display_Selection::FLDTop || display == I2C_Logic_Display_Selection::FLDBoth || display == I2C_Logic_Display_Selection::RLD || display == I2C_Logic_Display_Selection::All)
        primaryColor[TOP_FLD_RLD].setHSV(hue, saturation, value);
        
    if (display == I2C_Logic_Display_Selection::FLDBottom || display == I2C_Logic_Display_Selection::FLDBoth || display == I2C_Logic_Display_Selection::All)
        primaryColor[BOTTOM_FLD].setHSV(hue, saturation, value);
}

void LogicDisplay::setPrimaryColor(I2C_Logic_Display_Selection::Value display, CRGB value) {
    if (display == I2C_Logic_Display_Selection::FLDTop || display == I2C_Logic_Display_Selection::FLDBoth || display == I2C_Logic_Display_Selection::RLD || display == I2C_Logic_Display_Selection::All)
        primaryColor[TOP_FLD_RLD] = value;
        
    if (display == I2C_Logic_Display_Selection::FLDBottom || display == I2C_Logic_Display_Selection::FLDBoth || display == I2C_Logic_Display_Selection::All)
        primaryColor[BOTTOM_FLD] = value;
}

void LogicDisplay::setPrimaryColor(I2C_Logic_Display_Selection::Value display) {
    if (isRLD)
        setPrimaryColor(display, rldColors[1][0], rldColors[1][1], rldColors[1][2]);
    else
        setPrimaryColor(display, fldColors[3][0], fldColors[3][1], fldColors[3][2]);
}

// ----------------------------------------------------------------------------
// COMMANDS
// ----------------------------------------------------------------------------
void LogicDisplay::setBrightness(byte brightness) {
    FastLED.setBrightness(brightness);
}

void LogicDisplay::setMode(I2C_Logic_Display_Selection::Value display, I2C_Logic_Mode::Value mode) {
    if (display == I2C_Logic_Display_Selection::FLDTop || display == I2C_Logic_Display_Selection::FLDBoth || display == I2C_Logic_Display_Selection::RLD || display == I2C_Logic_Display_Selection::All)
        currentMode[TOP_FLD_RLD] = mode;
        
    if (display == I2C_Logic_Display_Selection::FLDBottom || display == I2C_Logic_Display_Selection::FLDBoth || display == I2C_Logic_Display_Selection::All)
        currentMode[BOTTOM_FLD] = mode;
        
    // special handling for March
    if (currentMode[TOP_FLD_RLD] == I2C_Logic_Mode::March || currentMode[TOP_FLD_RLD] == I2C_Logic_Mode::March_Separate)
        marchState[TOP_FLD_RLD] = true;
        
    if (currentMode[BOTTOM_FLD] == I2C_Logic_Mode::March)
        marchState[BOTTOM_FLD] = true;
        
    if (currentMode[BOTTOM_FLD] == I2C_Logic_Mode::March_Separate)
        marchState[BOTTOM_FLD] = false;
        
    // special handling for randome pixel
    if (currentMode[TOP_FLD_RLD] == I2C_Logic_Mode::Random_Pixel)
        clear(TOP_FLD_RLD);
    
    if (currentMode[BOTTOM_FLD] == I2C_Logic_Mode::Random_Pixel)
        clear(BOTTOM_FLD);
    
    // special handling for text
    if (currentMode[TOP_FLD_RLD] == I2C_Logic_Mode::Text)
        clear(TOP_FLD_RLD);
    
    if (currentMode[BOTTOM_FLD] == I2C_Logic_Mode::Text)
        clear(BOTTOM_FLD);
}

void LogicDisplay::setText(I2C_Logic_Display_Selection::Value display, String text) {
    if (display == I2C_Logic_Display_Selection::FLDTop || display == I2C_Logic_Display_Selection::FLDBoth || display == I2C_Logic_Display_Selection::RLD || display == I2C_Logic_Display_Selection::All) {
        textString[TOP_FLD_RLD] = text + "      ";
        textPosition[TOP_FLD_RLD] = 0;
        letterPosition[TOP_FLD_RLD] = 0;
    }
        
    if (display == I2C_Logic_Display_Selection::FLDBottom || display == I2C_Logic_Display_Selection::FLDBoth || display == I2C_Logic_Display_Selection::All) {
        textString[BOTTOM_FLD] = text + "      ";
        textPosition[BOTTOM_FLD] = 0;
        letterPosition[BOTTOM_FLD] = 0;
    }
}

// ----------------------------------------------------------------------------
// MODES
// ----------------------------------------------------------------------------
// ON
// ----------------------------------------------------------------------------
void LogicDisplay::animateOn(byte isTopOrBottom) {
    for(byte x=0;x<96;x++) 
        if (isRLD) 
            leds[pgm_read_byte(&rldMap[x])] = primaryColor[TOP_FLD_RLD];
        else if (x < 80) {
            if (isTopOrBottom == TOP_FLD_RLD && x < 40)
                leds[pgm_read_byte(&fldMap[x])] = primaryColor[TOP_FLD_RLD];
            else if (isTopOrBottom == BOTTOM_FLD && x >= 40)
                leds[pgm_read_byte(&fldMap[x])] = primaryColor[BOTTOM_FLD];
        }
       
    FastLED.show();         
}

// ----------------------------------------------------------------------------
// OFF
// ----------------------------------------------------------------------------
void LogicDisplay::animateOff(byte isTopOrBottom) {
    clear(isTopOrBottom);
}

// ----------------------------------------------------------------------------
// NORMAL
// ----------------------------------------------------------------------------
void LogicDisplay::animateNormal(byte isTopOrBottom) {
    hueVal = round(analogRead(A3)/4);
    
    for (byte ledNum = 0; ledNum<96; ledNum++) 
        if (isRLD) 
            updateLed(pgm_read_byte(&rldMap[ledNum]), hueVal);
        else if (ledNum < 80 && ((isTopOrBottom == TOP_FLD_RLD && ledNum < 40) || (isTopOrBottom == BOTTOM_FLD && ledNum >= 40)))
            updateLed(pgm_read_byte(&fldMap[ledNum]), hueVal);
    
    FastLED.show();         
}

void LogicDisplay::updateLed(byte ledNum, byte hueVal) {
    //this will take an LED number and adjust its status in the LEDstat array
    //check the current color this LED is set to...
    //unsigned int currentColor=LEDstat[LEDnum];  
    if (LEDstat[ledNum][2]!=0) {
      //LED is paused
      LEDstat[ledNum][2]=LEDstat[ledNum][2]-1; //reduce the LEDs pause number and check back next loop
    }
    else {
        //LED had 0 pause time, let's change things around...
        if (LEDstat[ledNum][1]==0 && LEDstat[ledNum][0]<(totalColors-1)) {
            LEDstat[ledNum][0]=LEDstat[ledNum][0]+1; //change it to next color
            leds[ledNum].setHSV(allColors[LEDstat[ledNum][0]][0]+hueVal,allColors[LEDstat[ledNum][0]][1],allColors[LEDstat[ledNum][0]][2]);
            if (LEDstat[ledNum][0]%(keys+1)==0) LEDstat[ledNum][2]=random(keyPause); //color is a key, set its pause time for longer than tweens
            else LEDstat[ledNum][2]=random(tweenPause);
        }
        else if (LEDstat[ledNum][1]==0 && LEDstat[ledNum][0]==(totalColors-1)) {
            LEDstat[ledNum][1]=1; //LED is at the final color, leave color but change direction to down
        }
        else if (LEDstat[ledNum][1]==1 && LEDstat[ledNum][0]>0) {
            LEDstat[ledNum][0]=LEDstat[ledNum][0]-1; //change it to previous color
            leds[ledNum].setHSV(allColors[LEDstat[ledNum][0]][0]+hueVal,allColors[LEDstat[ledNum][0]][1],allColors[LEDstat[ledNum][0]][2]);
            if (LEDstat[ledNum][0]%(keys+1)==0) {
              LEDstat[ledNum][2]=random(keyPause); //new color is a key, set LED's pause time for longer than tweens
            }
            else LEDstat[ledNum][2]=tweenPause;
        }
        else if (LEDstat[ledNum][1]==1 && LEDstat[ledNum][0]==0) {
            LEDstat[ledNum][1]=0; //LED is at the first color (0), leave color but change direction to up
        }
    }
}

// ----------------------------------------------------------------------------
// MARCH
// ----------------------------------------------------------------------------
void LogicDisplay::animateMarch(byte isTopOrBottom) {
    if (!IsTimeForStateChange(isTopOrBottom, 250, true))
        return;
        
    if (marchState[isTopOrBottom])
        clear(isTopOrBottom);
    else {
        for(byte x=0;x<96;x++) 
        if (isRLD) 
            leds[pgm_read_byte(&rldMap[x])] = primaryColor[TOP_FLD_RLD];
        else if (x < 80) {
            if (isTopOrBottom == TOP_FLD_RLD && x < 40)
                leds[pgm_read_byte(&fldMap[x])] = primaryColor[TOP_FLD_RLD];
            else if (isTopOrBottom == BOTTOM_FLD && x >= 40)
                leds[pgm_read_byte(&fldMap[x])] = primaryColor[BOTTOM_FLD];
        }
                
        FastLED.show();
    }
    
    marchState[isTopOrBottom] = !marchState[isTopOrBottom];
}

// ----------------------------------------------------------------------------
// COLR SHIFT
// ----------------------------------------------------------------------------
void LogicDisplay::animateColorShift(byte isTopOrBottom) {
    if (!IsTimeForStateChange(isTopOrBottom, 25, false))
        return;
        
    for(byte x=0;x<96;x++) 
        if (isRLD) 
            leds[pgm_read_byte(&rldMap[x])].setHSV(colorShiftHue[isTopOrBottom], 255, 255);
        else if (x < 80 && ((isTopOrBottom == TOP_FLD_RLD && x < 40) || (isTopOrBottom == BOTTOM_FLD && x >= 40)))
            leds[pgm_read_byte(&fldMap[x])].setHSV(colorShiftHue[isTopOrBottom], 255, 255);
            
    colorShiftHue[isTopOrBottom]++;
            
    FastLED.show();         
}

// ----------------------------------------------------------------------------
// LEFT SHIFT
// ----------------------------------------------------------------------------
void LogicDisplay::animateLeftShift(byte isTopOrBottom) {
    if (!IsTimeForStateChange(isTopOrBottom,  150,  false))
        return;
        
    if (leftShiftPosition[isTopOrBottom] == 0) {
        clear(isTopOrBottom);
        if (isRLD) {
            for (byte x=0;x<4;x++)
                leds[pgm_read_byte(&rldRows[x][23])] = primaryColor[TOP_FLD_RLD];
        } else {
            if (isTopOrBottom  == TOP_FLD_RLD) {
                leds[8] = primaryColor[TOP_FLD_RLD];
                leds[23] = primaryColor[TOP_FLD_RLD];
                leds[24] = primaryColor[TOP_FLD_RLD];
                leds[39] = primaryColor[TOP_FLD_RLD];
                leds[40] = primaryColor[TOP_FLD_RLD];
            } else {
                leds[95] = primaryColor[BOTTOM_FLD];
                leds[80] = primaryColor[BOTTOM_FLD];
                leds[79] = primaryColor[BOTTOM_FLD];
                leds[64] = primaryColor[BOTTOM_FLD];
                leds[63] = primaryColor[BOTTOM_FLD];
            }
        }
    } else {
        shiftDisplay(isTopOrBottom, true);
    }
    
    leftShiftPosition[isTopOrBottom]++;

    if (isRLD) {
        if (leftShiftPosition[isTopOrBottom] == 24)
            leftShiftPosition[isTopOrBottom] = 0;
    } else {
        if (leftShiftPosition[isTopOrBottom] == 8)
            leftShiftPosition[isTopOrBottom] = 0;
    }
    
    FastLED.show();
}

// ----------------------------------------------------------------------------
// RIGHT SHIFT
// ----------------------------------------------------------------------------
void LogicDisplay::animateRightShift(byte isTopOrBottom) {
    if (!IsTimeForStateChange(isTopOrBottom,  150,  false))
        return;
        
    if (rightShiftPosition[isTopOrBottom] == 0) {
        clear(isTopOrBottom);
        if (isRLD) {
            for (byte x=0;x<4;x++)
                leds[pgm_read_byte(&rldRows[x][0])] = primaryColor[TOP_FLD_RLD];
        } else {
            if (isTopOrBottom  == TOP_FLD_RLD) {
                leds[15] = primaryColor[TOP_FLD_RLD];
                leds[16] = primaryColor[TOP_FLD_RLD];
                leds[31] = primaryColor[TOP_FLD_RLD];
                leds[32] = primaryColor[TOP_FLD_RLD];
                leds[47] = primaryColor[TOP_FLD_RLD];
            } else {
                leds[88] = primaryColor[BOTTOM_FLD];
                leds[87] = primaryColor[BOTTOM_FLD];
                leds[72] = primaryColor[BOTTOM_FLD];
                leds[71] = primaryColor[BOTTOM_FLD];
                leds[56] = primaryColor[BOTTOM_FLD];
            }
        }
    } else {
        shiftDisplay(isTopOrBottom, false);
    }
    
    rightShiftPosition[isTopOrBottom]++;
    
    if (isRLD) {
        if (rightShiftPosition[isTopOrBottom] == 24)
            rightShiftPosition[isTopOrBottom] = 0;
    } else {
        if (rightShiftPosition[isTopOrBottom] == 8)
            rightShiftPosition[isTopOrBottom] = 0;
    }
    
    FastLED.show();
}

// ----------------------------------------------------------------------------
// LEFT RIGHT SHIFT
// ----------------------------------------------------------------------------
void LogicDisplay::animateLeftRightShift(byte isTopOrBottom) {
    if (isMovingLeft[isTopOrBottom])
    {
        byte preCheck = leftShiftPosition[isTopOrBottom];
        animateLeftShift(isTopOrBottom);
        if (leftShiftPosition[isTopOrBottom] == 0 && leftShiftPosition[isTopOrBottom] != preCheck)
            isMovingLeft[isTopOrBottom] = !isMovingLeft[isTopOrBottom];
    } else {
        byte preCheck = rightShiftPosition[isTopOrBottom];
        animateRightShift(isTopOrBottom);
        if (rightShiftPosition[isTopOrBottom] == 0 && rightShiftPosition[isTopOrBottom] != preCheck)
            isMovingLeft[isTopOrBottom] = !isMovingLeft[isTopOrBottom];
    }
}

// ----------------------------------------------------------------------------
// RANDOM PIXEL
// ----------------------------------------------------------------------------
void LogicDisplay::animateRandomPixel(byte isTopOrBottom) {
    if (!IsTimeForStateChange(isTopOrBottom, 50, false))
        return;    
        
    leds[currentPixel[isTopOrBottom]] = CRGB::Black;
    
    byte ledNum = random( isRLD ? 96 : 80);
    leds[ledNum] = primaryColor[isTopOrBottom];
    
    currentPixel[isTopOrBottom] = ledNum;
    
    FastLED.show();
}

// ----------------------------------------------------------------------------
// TEXT
// ----------------------------------------------------------------------------
void LogicDisplay::animateText(byte isTopOrBottom) {
    if (!IsTimeForStateChange(isTopOrBottom, 150, false))
        return;

    // exit if we don't have a letter to display        
    if (textString[isTopOrBottom].length() <= textPosition[isTopOrBottom])
    {
        textPosition[isTopOrBottom] = 0;
        letterPosition[isTopOrBottom] = 0;
    }
        
    // display the letter
    char letterToDisplay = textString[isTopOrBottom].charAt(textPosition[isTopOrBottom]);
    
    byte letterIndex = getLetterIndex(letterToDisplay);
    
    // display the position
    byte d = pgm_read_byte(&letters[letterIndex][letterPosition[isTopOrBottom]]);
    
    // increment
    if (d == B00000)
    {
        // increment letter
        textPosition[isTopOrBottom]++;
        letterPosition[isTopOrBottom] = 0;
    }
    else
        letterPosition[isTopOrBottom]++;
        
    // DISPLAY IT HERE
    shiftDisplay(isTopOrBottom, true);
    
    // put new in place
    if (isRLD) {
    } else {
        if (isTopOrBottom == TOP_FLD_RLD) {
            // top
            leds[8] = ((d >> 0) & 1) == 1 ? primaryColor[TOP_FLD_RLD] : CRGB::Black;
            leds[23] = ((d >> 1) & 1) == 1 ? primaryColor[TOP_FLD_RLD] : CRGB::Black;;
            leds[24] = ((d >> 2) & 1) == 1 ? primaryColor[TOP_FLD_RLD] : CRGB::Black;;
            leds[39] = ((d >> 3) & 1) == 1 ? primaryColor[TOP_FLD_RLD] : CRGB::Black;;
            leds[40] = ((d >> 4) & 1) == 1 ? primaryColor[TOP_FLD_RLD] : CRGB::Black;;
        } else {
            leds[95] = ((d >> 0) & 1) == 1 ? primaryColor[BOTTOM_FLD] : CRGB::Black;
            leds[80] = ((d >> 1) & 1) == 1 ? primaryColor[BOTTOM_FLD] : CRGB::Black;;
            leds[79] = ((d >> 2) & 1) == 1 ? primaryColor[BOTTOM_FLD] : CRGB::Black;;
            leds[64] = ((d >> 3) & 1) == 1 ? primaryColor[BOTTOM_FLD] : CRGB::Black;;
            leds[63] = ((d >> 4) & 1) == 1 ? primaryColor[BOTTOM_FLD] : CRGB::Black;;
        }
    }
    
    FastLED.show();
}

void LogicDisplay::shiftDisplay(byte isTopOrBottom, bool moveLeft) {
    if (isRLD) {
        for (byte x=0; x<4; x++)
            shiftRLDRow(x, moveLeft);
    } else {
        if (isTopOrBottom == TOP_FLD_RLD) {
            shiftFLDRow(15,8, moveLeft);
            shiftFLDRow(16,23, moveLeft);
            shiftFLDRow(31,24, moveLeft);
            shiftFLDRow(32,39, moveLeft);
            shiftFLDRow(47,40, moveLeft);
        } else {
            shiftFLDRow(88, 95, moveLeft);
            shiftFLDRow(87, 80, moveLeft);
            shiftFLDRow(72,79, moveLeft);
            shiftFLDRow(71,64, moveLeft);
            shiftFLDRow(56, 63, moveLeft);
        }
    }
    
    // now put black where it should
    if (moveLeft) {
        if (isRLD) {
            for (byte x=0;x<4;x++)
                leds[pgm_read_byte(&rldRows[x][23])] = CRGB::Black;
        } else {
            if (isTopOrBottom == TOP_FLD_RLD) {
                leds[8] = CRGB::Black;
                leds[23] = CRGB::Black;
                leds[24] = CRGB::Black;
                leds[39] = CRGB::Black;
                leds[40] = CRGB::Black;
            } else {
                leds[95] = CRGB::Black;
                leds[80] = CRGB::Black;
                leds[79] = CRGB::Black;
                leds[64] = CRGB::Black;
                leds[63] = CRGB::Black;
            }
        }
    } else {
        if (isRLD) {
            for (byte x=0;x<4;x++)
                leds[pgm_read_byte(&rldRows[x][0])] = CRGB::Black;
        } else {
            if (isTopOrBottom == TOP_FLD_RLD) {
                leds[15] = CRGB::Black;
                leds[16] = CRGB::Black;
                leds[31] = CRGB::Black;
                leds[32] = CRGB::Black;
                leds[47] = CRGB::Black;
            } else {
                leds[88] = CRGB::Black;
                leds[87] = CRGB::Black;
                leds[72] = CRGB::Black;
                leds[71] = CRGB::Black;
                leds[56] = CRGB::Black;
            }
        }
    }
}

void LogicDisplay::shiftFLDRow(byte left, byte right, bool moveLeft) {
    if (moveLeft) {
        if (left < right) {
            for (byte x=left; x<right; x++)
                leds[x] = leds[x+1];
        } else {
            for (byte x=left; x>right; x--)
                leds[x] = leds[x-1];
        }
    } else {
        if (left < right) {
            for (byte x=right; x>left; x--)
                leds[x] = leds[x-1];
        } else {
            for (byte x=right; x<left; x++)
                leds[x] = leds[x+1];
        }
    }
}

void LogicDisplay::shiftRLDRow(byte row, bool moveLeft) {
    if (moveLeft) {
        for (byte x=0; x<23; x++)
            leds[pgm_read_byte(&rldRows[row][x])] = leds[pgm_read_byte(&rldRows[row][x + 1])];
    } else {
        for (byte x=23; x > 0; x--)
            leds[pgm_read_byte(&rldRows[row][x])] = leds[pgm_read_byte(&rldRows[row][x - 1])];
    }
}

byte LogicDisplay::getLetterIndex(char letter) {
    if (letter >= 'A' and letter <= 'Z')
        return letter - 'A';
        
    if (letter >= '0' and letter <='9')
        return letter - '0' + 26;
        
    if (letter == '-') return 36;
    if (letter == ' ') return 37;
    if (letter == '!') return 38;
    if (letter == '.') return 39;
    if (letter == '+') return 40;
    
    return 37; // space
}

bool LogicDisplay::IsTimeForStateChange(byte isTopOrBottom, int delay, bool shouldClear)
{
    unsigned long timeNow = millis();
  
    // early exit if we don't need to do anything
    if (timeNow - lastTimeCheck[isTopOrBottom] < delay)
        return false;

    // set the time  
    lastTimeCheck[isTopOrBottom] = timeNow;

    // clear the device
    if (shouldClear)
        clear(isTopOrBottom);

    return true;
}


