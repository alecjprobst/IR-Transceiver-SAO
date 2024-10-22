#include "PinDefinitionsAndMore.h"
#include "MessageProtocol.h"
#include <ATtinySerialOut.hpp>

const byte START_BYTE = 0xAA;
const byte END_BYTE = 0xFF;

byte sequenceNum = 0;
const char* message = "Hello World!";

void setup() {
    // Set the button pin as input with pull-up resistor
    pinMode(BUTTON_PIN_1, INPUT_PULLUP);  
    
    // Initialize Serial
    initTXPin();
    Serial.begin(115200);
    delay(100);

    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
}

void loop() {

    if (digitalRead(BUTTON_PIN_1) == LOW) {
        MessageProtocol::sendMessage(message, sequenceNum, IR_SEND_PIN); 
        
        while (digitalRead(BUTTON_PIN_1) == LOW) {
            delay(50);  // Debouncing delay
        }
    }

        delay(500);
}