#Tiny_Receiver_Signal_Info

Sample provides information about a sent IR signal. 
Using TinyIRReceiver to save space on the VERY limited available resources of the ATTiny85. 

By default expected a NEC Protocol signal however, can be programmed to expect FAST or ONKYO signal. 

## Usage
- Aim a IR transmitter at the IR receiver.  
- Monitor Serial Log for output. 

## Special Note
Requires another device monitoring serial out on TX pin. 
( TX Pin by default is Pin 7 (PB2) )

## Components Tested
- IR Receiver
- Serial Capability

## Libraries
Links on root project README.
- ATtinySerialOut
- TinyIRReceiver
