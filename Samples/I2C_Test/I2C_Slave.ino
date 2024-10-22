#include <TinyWireS.h>

void setup() 
{
  TinyWireS.begin(0x08);
  TinyWireS.onReceive(receiveEvent);
}

void loop() 
{
}

void receiveEvent(uint8_t howMany) 
{
  uint8_t receivedDataSize = TinyWireS.available();
  char receiveString[receivedDataSize];
  int i = 0;

  while (TinyWireS.available() > 0) 
  {
    // Add byte to char array
    receiveString[i] = char(TinyWireS.receive());
    i++;
  }  

  // Check if char array is equal to what is expected, send data back
  if(receivedDataSize >= 5 && receiveString[0] == 'H' && receiveString[1] == 'e' && receiveString[2] == 'l' && receiveString[3] == 'l' && receiveString[4] == 'o')
  {
    sendHello();
  }
}

void sendHello()
{
  TinyWireS.send(uint8_t('G'));
  TinyWireS.send(uint8_t('o'));
  TinyWireS.send(uint8_t('o'));
  TinyWireS.send(uint8_t('d'));
}