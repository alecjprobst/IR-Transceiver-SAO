# Receiver_Signal_Info

Provides information about a received IR signal. 

Specifically the following will be printed to Serial out: 
- Protocol
- Decoded Raw Data
- Code required to send the received statement.

## Special Note

- This sample is not really practical.
	- The ATTiny85 board has very limited space and resource.  
	  This sample consumes most of the space on the ATTiny85 board.  
	  
- Requires another device monitoring serial out on TX pin. 
  ( TX Pin by default is Pin 7 (PB2) )
  
## Usage
- Aim a IR transmitter at the IR receiver.  
- Monitor Serial Log for output. 
## Components Tested
- IR Receiver
- Serial Capability
## Libraries
Links on root project README.
- ATtinySerialOut
- IRremote
