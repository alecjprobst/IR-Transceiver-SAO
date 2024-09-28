#define IR_RECEIVE_PIN PIN_PB3
#define IR_SEND_PIN PIN_PB4

// In this section, disabling everything possible to save space on ATTiny85
#define NO_LED_FEEDBACK_CODE 1

#include "ATtinySerialOut.hpp"
#include <IRremote.hpp>


void setup() {
    initTXPin();
    pinMode(IR_RECEIVE_PIN, INPUT);
    delay(100); // Give Serial a moment to start
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
    Serial.println("End Setup.");
}

void loop() {
    if (IrReceiver.decode()) {
      Serial.println(IrReceiver.getProtocolString());
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // Print "old" raw data
      // IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
      IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data
      IrReceiver.resume(); // Enable receiving of the next value
  }
}