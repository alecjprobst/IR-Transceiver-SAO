#include <Wire.h>
#include "Arduino.h"

void setup() 
{
    Wire.begin();                // join i2c bus as master
    //Wire.onReceive(recieveEvent); // register event
    pinMode(8, OUTPUT);   // Setup LED pin
    Serial.begin(9600);
    Serial.print("Starting Host:\n");
    //Serial.print("Sending 'Hello'\n");
    //Wire.beginTransmission(0x08);
    //Wire.write("Hello");
    //Wire.endTransmission();
}

void loop() 
{
    // Send character as command to slave
    Serial.print("\nSending Letter to Slave\n");
    Wire.beginTransmission(0x08);
    Wire.write("J");
    Wire.endTransmission();

    // Request 1 byte based on command
    Serial.print("\nRequesting 1 byte from Slave\n");
    Wire.requestFrom(8, 1);    // request 4 bytes from peripheral device #8

    while (Wire.available()) 
    {   // peripheral may send less than requested
        char c = Wire.read(); // receive a byte as character
        Serial.print(c);         // print the character
    }

    delay(500);
}

/*
void recieveEvent(int count) 
{
    Serial.print("Recieved: ");         // print the character
    while(Wire.available() > 0) // loop through all bytes
    {
        char c = Wire.read();    // receive byte as a character
        Serial.print(c);         // print the character
    }
}
*/