// System Definitions
#define F_CPU 8000000
#include "PinDefinitionsAndMore.h"

// IR Definitions
#include "TinyIRReceiver.hpp"
#include "TinyIRSender.hpp"

// Serial Communication Definitions
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(SERIAL_RX_PIN, SERIAL_TX_PIN); //rx,tx

// Code Specific Definitions

void setup()
{
  // IR_RECEIVE_PIN in PinDefinitionsAndMore.h automatically overrides default reciever pin
  initPCIInterruptForTinyReceiver(); // Enables the interrupt generation on change of IR input signal
  // Setup Normal LED Pin for testing (Note, cannot do this and Serial output at the same time)
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  //mySerial.begin(9600);
  //mySerial.println("Attiny85 IR");
}

void loop() {
  // Loop where we check to see if we recieved any data
  if (TinyReceiverDecode()) 
  {
    // NEC protocol has 16 bit address, 16 bit Command. Command is made up of 8 bit data, 8 bit inverted data (for polarity check)
    uint16_t Recieved_Address = TinyIRReceiverData.Address;
    uint16_t Recieved_Command = TinyIRReceiverData.Command;
    uint8_t Recieved_Data = Recieved_Command >> 8; // Bit Shift last 8 bits to get non-inversed data from Command

    // Check if recieved data is correct. If True, will turn pin 6 (PB1) high or send Serial Data througn pin
    if(Recieved_Address == 0 and Recieved_Data == 69)
    {
      digitalWrite(LED_PIN, HIGH);
      //printTinyReceiverResultMinimal(&Serial);
    }
  }

  // Send Data out
  uint16_t Sending_Address = 0;
  uint8_t Sending_Data = 69;
  uint8_t Sending_Repeats = 0;
  uint16_t Sending_Command = Generate_Command(Sending_Data);
  sendNEC(IR_SEND_PIN, Sending_Address, Sending_Command, Sending_Repeats);

  delay(100);
}

uint16_t Generate_Command(uint8_t Data)
{
  uint16_t New_Command = Data;
  New_Command = (~(New_Command << 8)) & Data;
  return New_Command;
}