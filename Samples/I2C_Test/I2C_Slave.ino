#include <Wire.h>
#include <Arduino.h>

char command = 0;

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
  else if(command == 'J')
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
  char receiveString[receivedDataSize];
  int i = 0;

  while (Wire.available() > 0) 
  {
    // Add byte to char array
    receiveString[i] = char(Wire.read());
    i++;
  }  

  // Check if char array is equal to what is expected, send data back
  if(receivedDataSize >= 1)
  {
    command = receiveString[0];
  }
  
}

void sendChar(char character)
{
  Wire.write(character);
}