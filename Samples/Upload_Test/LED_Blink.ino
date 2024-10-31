// System Definitions
#include <Arduino.h>
#include "PinDefinitionsAndMore.h"

// Serial Communication Definitions

// Code Specific Definitions

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}