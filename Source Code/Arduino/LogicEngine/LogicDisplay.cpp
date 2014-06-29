#include "LogicDisplay.h"

void LogicDisplay::setup(I2C_Device_Address::Value address, bool isRLDLogic) {
    i2cAddress = address;
    isRLD = isRLDLogic;
    
    setMode(I2C_Logic_Mode::Normal);
    setBrightness(255);
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, 96);
      
    delay(50);
    randomSeed(analogRead(0));
    
    if (isRLD) {
        tweenPause = 40;
        keyPause = 1200;
        keys = sizeof(rldColors) / 3;
        primaryColor.setHSV(rldColors[2][0], rldColors[2][1], rldColors[2][2]);  // green
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
    
    clear();

    // RLD TEST PATTERN HERE
    
    setInitialColors();
    
    showStartupAnimation();
}


void LogicDisplay::processCommand() {
}

void LogicDisplay::update() {
    // LOOPCHECK HERE
    
    if (!isModeActive)
        return;

    switch (currentMode)
    {
        case I2C_Logic_Mode::Normal:
            animateNormal();
            break;
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

void LogicDisplay::clear() {
    for(byte x=0;x<96;x++) 
        leds[x] = CRGB::Black;
    FastLED.show();      
}

// ----------------------------------------------------------------------------
// COMMANDS
// ----------------------------------------------------------------------------
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

void LogicDisplay::setBrightness(byte brightness) {
    FastLED.setBrightness(brightness);
}

void LogicDisplay::setMode(I2C_Logic_Mode::Value mode) {
    isModeActive = true;
    currentMode = mode;
}

// ----------------------------------------------------------------------------
// MODES
// ----------------------------------------------------------------------------
// NORMAL
// ----------------------------------------------------------------------------
void LogicDisplay::animateNormal() {
    hueVal = round(analogRead(A3)/4);
    for (byte ledNum=0; ledNum<96; ledNum++)
        if (isRLD)
            updateLed(pgm_read_byte(&rldMap[ledNum]), hueVal);
        else if (ledNum < 80) 
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




