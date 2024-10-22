#include <Wire.h>
#include "Arduino.h"

void setup() 
{
    Wire.begin();                // join i2c bus as master
    Wire.onRequest(requestEvent); // register event
    pinMode(8, OUTPUT);   // Setup LED pin
    Serial.begin(9600);
    Serial.print("Starting Transmission:\n");

    Serial.print("Sending 'Hello'\n");
    Wire.beginTransmission(8);
    Wire.write("Hello");
    Wire.endTransmission();
}

void loop() 
{
}

void requestEvent() 
{
    Serial.print("Recieved: ");         // print the character
    while(Wire.available() > 0) // loop through all bytes
    {
        char c = Wire.read();    // receive byte as a character
        Serial.print(c);         // print the character
    }
}