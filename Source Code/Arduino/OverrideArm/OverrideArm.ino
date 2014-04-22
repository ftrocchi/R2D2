#include <XBee.h>
#include <Keypad.h>
#include "ButtonPad.h"

XBee xbee = XBee();

uint8_t payload[4];
Tx16Request tx = Tx16Request(0x8250, payload, sizeof(payload));
Rx16Response rx16 = Rx16Response();

bool isOn = true;
bool isTransmit = false;

ButtonPad buttonPad;

unsigned long timer;

void setup()
{
    Serial1.begin(9600);
    xbee.setSerial(Serial1);
}

void loop()
{
    buttonPad.Update();
    
    if (timer + 1000 < millis())
    {
        timer = millis();

        for (int i=0; i<4; i++)
            payload[i] = isOn;
      
        xbee.send(tx);
        isOn = !isOn;
    }

    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable())
    {
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE)

        {
            // get the data
            xbee.getResponse().getRx16Response(rx16);
            
            for (int i=0; i<4; i++)
                buttonPad.SetButtonState(i, rx16.getData(i) * 2);
        }
    }
}
