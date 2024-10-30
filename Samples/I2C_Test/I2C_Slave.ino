#include <Wire.h>
#include <Arduino.h>

char command = 0;
char data = 0;

void setup() 
{
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop() 
{
  delay(100);
}

void requestEvent()
{
  if(command == 'H')
  {
    sendChar('X');
  }
  else if(command == 'J' && data == 'e')
  {
    sendChar('U');
  }
  else{
    sendChar('a');
  }
}

void receiveEvent(int howMany) 
{
  uint8_t receivedDataSize = Wire.available();

  if (receivedDataSize > 0) 
  {
    command = Wire.read();
  }
  if(receivedDataSize > 1)
  {
    data = Wire.read();
  }
  
}

void sendChar(char character)
{
  Wire.write(character);
}