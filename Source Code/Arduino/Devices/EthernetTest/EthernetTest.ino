#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Ethernet.h>

#define MAX_FRAME_LENGTH 64

#include <WebSocket.h>

#define SERVOMIN  140 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  550 // this is the 'maximum' pulse length count (out of 4096)

// websocket declarations
byte mac[6];
byte ip[4];
WebSocket webSocket;

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);

void setup()
{
    Serial.begin(9600);
    Serial.println("BEGIN");
    
    // websocket setup
    byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x5F, 0xFB };
    byte ip[] = { 192, 168, 1, 82 };
    
    Ethernet.begin(mac, ip);
    
    webSocket.registerConnectCallback(&OnConnect);
    webSocket.registerDataCallback(&OnData);
    webSocket.registerDisconnectCallback(&OnDisconnect);
    
    webSocket.begin();
    
    delay(100);  
    
    pwm1.begin();
    pwm1.setPWMFreq(50);

    Wire.begin();
    
    delay(2000);
}

void loop()
{
    webSocket.listen();
    
    delay(20);
}

void OnConnect(WebSocket &socket)
{
}

void OnDisconnect(WebSocket &socket)
{
}

void OnData(WebSocket &socket, char* dataString, byte frameLength)
{
    Serial.println("ON DATA");
    
    // split the data
    byte command[12];
    int index = 0;
    char *savePointer;
    char *token;
    
    for (token = strtok_r(dataString, "/", &savePointer); token; token = strtok_r(NULL, "/", &savePointer))
    {
        command[index] = atoi(token);
        index++;
    }
    
    Serial.print("I2C MESSAGE:");
    Serial.print(command[0]);
    for (int i=1; i < index; i++)
    {
        Serial.print("-");
        Serial.print(command[i]);
    }
    Serial.println("");
    
    // if it is less than 128 it is an i2c command so send it over i2c
    if (command[0] < 128)
    {
        Serial.print("COMMAND RECEIVED: ");
        Serial.println(command[0]);
        
        if (command[0] == 64)
        {
            int value = SERVOMIN;

            command[2] == 1 ? value = 240 : value = 440;
            
            pwm1.setPWM(command[1], 0, value);
        }
        else 
        {
        
        
        Wire.beginTransmission(command[0]);
        for (int i=1; i < index; i++)
        {
            Serial.println(command[i]);
            Wire.write(command[i]);
        }
            
        Serial.println("ENDING");
        Wire.endTransmission();
        Serial.println("ENDED");
        return;
        }
    }    
}
