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
            
/*            
        case I2C_Logic_Mode::FLD_March_Together:
            animateFLDMarchTogether();
            break;
            
        case I2C_Logic_Mode::FLD_March_Separate:
            animateFLDMarchSeparate();
            break;
            
        case I2C_Logic_Mode::RLD_March:
            animateRLDMarch();
            break;
            
        case I2C_Logic_Mode::FLD_Spin_Clockwise_Separate:
            animateFLDSpinClockwiseSeparate();
            break;
            */
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
/*
void LogicDisplay::on() {
    isModeActive = false;
    
    for(byte x=0;x<96;x++) 
        if (isRLD)
            leds[pgm_read_byte(&rldMap[x])] = primaryColor;
        else if (x<80)
            leds[pgm_read_byte(&fldMap[x])] = primaryColor;
    FastLED.show();      
}

void LogicDisplay::off() {
    isModeActive = false;
    clear();
}
*/
void LogicDisplay::setBrightness(byte brightness) {
    FastLED.setBrightness(brightness);
}

void LogicDisplay::setMode(I2C_Logic_Display_Selection::Value display, I2C_Logic_Mode::Value mode) {
    if (display == I2C_Logic_Display_Selection::FLDTop || display == I2C_Logic_Display_Selection::FLDBoth || display == I2C_Logic_Display_Selection::RLD || display == I2C_Logic_Display_Selection::All)
        currentMode[TOP_FLD_RLD] = mode;
        
    if (display == I2C_Logic_Display_Selection::FLDBottom || display == I2C_Logic_Display_Selection::FLDBoth || display == I2C_Logic_Display_Selection::All)
        currentMode[BOTTOM_FLD] = mode;
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
/*
// ----------------------------------------------------------------------------
// MARCH
// ----------------------------------------------------------------------------
void LogicDisplay::animateFLDMarchTogether() {
    if (!IsTimeForStateChange(250))
        return;

    if (firstColor)
        clear();
    else
    {
        for(byte x=0;x<80;x++) 
            leds[pgm_read_byte(&fldMap[x])] = primaryColor;
        FastLED.show();      
    }

    firstColor = !firstColor;
}

void LogicDisplay::animateFLDMarchSeparate() {
    if (!IsTimeForStateChange(250))
        return;
        
    for (byte x=0; x<80; x++) {
        if (x < 48)
            leds[pgm_read_byte(&fldMap[x])] = firstColor ? primaryColor : CRGB::Black;
        else
            leds[pgm_read_byte(&fldMap[x])] = !firstColor ? primaryColor : CRGB::Black;
    }
    
    FastLED.show();      
    
    firstColor = !firstColor;
}

void LogicDisplay::animateRLDMarch() {
    if (!IsTimeForStateChange(250))
        return;

    if (firstColor)
        clear();
    else
    {
        for(byte x=0;x<96;x++) 
            leds[pgm_read_byte(&rldMap[x])] = primaryColor;
        FastLED.show();      
    }

    firstColor = !firstColor;
}

// ----------------------------------------------------------------------------
// RING
// ----------------------------------------------------------------------------
void LogicDisplay::animateFLDSpinClockwiseSeparate() {
}

bool LogicDisplay::IsTimeForStateChange(int delay)
{
    unsigned long timeNow = millis();
  
    // early exit if we don't need to do anything
    if (timeNow - lastTimeCheck < delay)
        return false;

    // set the time  
    lastTimeCheck = timeNow;

    // clear the device
    clear();

    return true;
}


*/


