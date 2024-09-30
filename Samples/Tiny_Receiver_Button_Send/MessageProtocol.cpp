// Intended Messure Structure
// 
// [START_BYTE] [SEQ_NUM] [DATA (each character)] [CHECKSUM] [END_BYTE]
// 
// Example: 
// [0xAA] [SEQ_NUM] ['H'] ['e'] ['l'] ['l'] ['o'] ['w'] ['o'] ['r'] ['l'] ['d'] [CHECKSUM] [0xFF]


#include "MessageProtocol.h"
#include <TinyIRSender.hpp>

const int SEND_DELAY = 10;
const byte START_BYTE = 0xAA;  // Start byte
const byte END_BYTE = 0xFF;    // End byte

// Calculate Checksum for message
static byte calculateChecksum(const char* message, byte sequenceNum) {
  // Start with the sequence number in the checksum
  byte checksum = sequenceNum;  

  while (*message) {
    // XOR each character into the checksum byte
    checksum ^= *message;  
    message++;
  }

  return checksum;
}

// Public function: sendMessage
void MessageProtocol::sendMessage(const char* message, byte sequenceNum, int irSendPin) {
  // Grab Checksum
  byte checksum = calculateChecksum(message, sequenceNum);

  // Send start byte
  sendNEC(irSendPin, START_BYTE, 32);
  delay(SEND_DELAY);

  // Send sequence number
  sendNEC(irSendPin, sequenceNum, 32);
  delay(SEND_DELAY);

  // Send the message one character at a time
  while (*message) {
    sendNEC(irSendPin, *message, 32);  // Sending ASCII of the character
    delay(SEND_DELAY);
    message++;
  }

  // Send the checksum
  sendNEC(irSendPin, checksum, 32);
  delay(SEND_DELAY);

  // Send end byte
  sendNEC(irSendPin, END_BYTE, 32);
  delay(SEND_DELAY);
}