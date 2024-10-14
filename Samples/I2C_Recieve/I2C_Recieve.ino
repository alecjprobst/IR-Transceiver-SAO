#include <TinyWireS.h>

volatile byte receivedData = 0;

void setup() {
  TinyWireS.begin(0x08);
  TinyWireS.onReceive(receiveEvent);
}

void loop() {
}

void receiveEvent(uint8_t howMany) {
  if (howMany >= 1) {
    receivedData = TinyWireS.receive();
  }
}