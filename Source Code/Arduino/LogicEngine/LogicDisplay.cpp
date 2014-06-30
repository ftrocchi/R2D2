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
        primaryColor.setHSV(rldColors[1][0], rldColors[1][1], rldColors[1][2]);  // green
    } else {
        tweenPause = 7;
        keyPause = 350;
        keys = sizeof(fldColors) / 3;
        primaryColor.setHSV(fldColors[3][0], fldColors[3][1], fldColors[3][2]);  // blue
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
        else if (x < 80 && ((isTopOrBottom == TOP_FLD_RLD && x < 48) || (isTopOrBottom == BOTTOM_FLD && x >= 48)))
            leds[pgm_read_byte(&fldMap[x])] = CRGB::Black;
            
    FastLED.show();           
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
            leds[pgm_read_byte(&rldMap[x])] = primaryColor;
        else if (x < 80 && ((isTopOrBottom == TOP_FLD_RLD && x < 48) || (isTopOrBottom == BOTTOM_FLD && x >= 48)))
            leds[pgm_read_byte(&fldMap[x])] = primaryColor;
            
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
        else if (ledNum < 80 && ((isTopOrBottom == TOP_FLD_RLD && ledNum < 48) || (isTopOrBottom == BOTTOM_FLD && ledNum >= 48)))
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
                leds[pgm_read_byte(&rldMap[x])] = primaryColor;
            else if (x < 80 && ((isTopOrBottom == TOP_FLD_RLD && x < 48) || (isTopOrBottom == BOTTOM_FLD && x >= 48)))
                leds[pgm_read_byte(&fldMap[x])] = primaryColor;
                
        FastLED.show();
    }
    
    marchState[isTopOrBottom] = !marchState[isTopOrBottom];
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
    
    // shift
    if (isRLD) {
    } else {
        if (isTopOrBottom == TOP_FLD_RLD) {
            shiftRow(15,8);
            shiftRow(16,23);
            shiftRow(31,24);
            shiftRow(32,39);
            shiftRow(47,40);
        } else {
            shiftRow(88, 95);
            shiftRow(87, 80);
            shiftRow(72,79);
            shiftRow(71,64);
            shiftRow(56, 63);
        }
    }
    
    // put new in place
    if (isRLD) {
    } else {
        if (isTopOrBottom == TOP_FLD_RLD) {
            // top
            leds[8] = ((d >> 0) & 1) == 1 ? primaryColor : CRGB::Black;
            leds[23] = ((d >> 1) & 1) == 1 ? primaryColor : CRGB::Black;;
            leds[24] = ((d >> 2) & 1) == 1 ? primaryColor : CRGB::Black;;
            leds[39] = ((d >> 3) & 1) == 1 ? primaryColor : CRGB::Black;;
            leds[40] = ((d >> 4) & 1) == 1 ? primaryColor : CRGB::Black;;
        } else {
            leds[95] = ((d >> 0) & 1) == 1 ? primaryColor : CRGB::Black;
            leds[80] = ((d >> 1) & 1) == 1 ? primaryColor : CRGB::Black;;
            leds[79] = ((d >> 2) & 1) == 1 ? primaryColor : CRGB::Black;;
            leds[64] = ((d >> 3) & 1) == 1 ? primaryColor : CRGB::Black;;
            leds[63] = ((d >> 4) & 1) == 1 ? primaryColor : CRGB::Black;;
        }
    }
    
    FastLED.show();
}

void LogicDisplay::shiftRow(byte left, byte right) {
    if (left < right) {
        for (byte x=left; x<right; x++)
            leds[x] = leds[x+1];
    } else {
        for (byte x=left; x>right; x--)
            leds[x] = leds[x-1];
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


