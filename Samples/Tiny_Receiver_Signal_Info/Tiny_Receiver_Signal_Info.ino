#include "PinDefinitionsAndMore.h"

#include "ATtinySerialOut.hpp"
#include "TinyIRReceiver.hpp"

void setup() {
    initTXPin();
    pinMode(IR_RECEIVE_PIN, INPUT);

    delay(100); // Give Serial a moment to start

    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_ATTINY_SERIAL_OUT));

    // Enables the interrupt generation on change of IR input signal
    if (!initPCIInterruptForTinyReceiver()) {
         Serial.println("No interrupt available for pin " + IR_RECEIVE_PIN ); // optimized out by the compiler, if not required :-)
    }

    // Verify IR Reciever is working.
    Serial.print("Is Reciever Init: ");
    Serial.println(isIRReceiverAttachedForTinyReceiver());

#if defined(USE_FAST_PROTOCOL)
    Serial.println( "Ready to receive Fast IR signals at pin " + IR_RECEIVE_PIN );
#else
    Serial.println( "Ready to receive NEC IR signals at pin " + IR_RECEIVE_PIN );
#endif

    Serial.println("End Setup.");
}

void loop() {
    if (TinyReceiverDecode()) {

#if !defined(USE_FAST_PROTOCOL)
        // We have no address at FAST protocol
        Serial.print(F("Address=0x"));
        Serial.print(TinyIRReceiverData.Address, HEX);
        Serial.print(' ');
#endif

        Serial.print(F("Command=0x"));
        Serial.print(TinyIRReceiverData.Command, HEX);
        if (TinyIRReceiverData.Flags == IRDATA_FLAGS_IS_REPEAT) {
            Serial.print(F(" Repeat"));
        }
        
        if (TinyIRReceiverData.Flags == IRDATA_FLAGS_PARITY_FAILED) 
        {
            Serial.print(F(" Parity failed"));

#if !defined(USE_EXTENDED_NEC_PROTOCOL) && !defined(USE_ONKYO_PROTOCOL)
            Serial.print(F(", try USE_EXTENDED_NEC_PROTOCOL or USE_ONKYO_PROTOCOL"));
#endif
        }

        Serial.println();
    }
}