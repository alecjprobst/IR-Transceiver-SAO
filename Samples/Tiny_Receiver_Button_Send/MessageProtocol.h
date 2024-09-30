#ifndef MESSAGE_PROTOCOL_H
#define MESSAGE_PROTOCOL_H

#include <Arduino.h>

class MessageProtocol {
public:
    static void sendMessage(const char* message, byte sequenceNum, int irSendPin); 
};

#endif