# IR-Transceiver-SAO

Event badges get lonely too. Why not have them chat with each other?    

A Programmable Infrared Multitool SAO. Gives a basic I2C interface to control and request or send data on IR.  
Takes care of basic functions such as transmission protocol, processing incoming transmission, and address checking.

# PCB Build Instructions
- PCB Build instructions and component data can be found at the [hackaday project site](https://hackaday.io/project/197812-infrared-communication-sao) <br/>
- PCB Build video can be found here #TODO

# Installation
1. Download this Github
2. Open Visual Studio Code with PlatformIO installed
3. Open this project
4. Setup the ATTINY85 for programming
> - [This video](https://www.youtube.com/watch?v9hlY&ab_channel=Keon%27sLab) can be used as hardware connection reference from 2:42 to 8:15. Requires an Arduino Uno
5. Build and Upload using the given platformio.ini

# SAO Definitions
- Mode Definitions
  - Public: The IR SAO ignores the address of incoming IR transmissions. Will save any transmissions sent to it
  - Address: The IR SAO will check the address of incoming IR transmissions. Will only save transmission with address that match it's IR address
    - IR Address: The 0-255 value address for IR communications.
    - Note: This is not the same as the I2C Address!

# I2C Communication Format
The IR SAO default I2C Address is 0x08. <br/>
Communications are made up of two parts. The first is the command and the second is the parameter(s). The Command is always sent first and the parameter(s) are sent subsequently as needed per command. Commands do not return any acknowledgement.
* Command - uint8_t: Determines what action you would like for the IR SAO. List of commands below
* Parameter(s) - uint8_t[]: Extra data specifiec by each command. Separated in uint8_t chunks

# I2C Example Communications
- To change the address of the IR SAO to address 8, the master should send: <br/>
`0x02 0x08` <br/>
- To read a byte from the IR SAO, the master should send: <br/>
`0x06` <br/>
The master would then request a byte back and be sent back a received IR byte such as: <br/>
`0x69`
- To send a byte of value 50 to the IR address of 230 over IR from the IR SAO, the master should send: <br/>
`0x07 0xE6 0x32`

# I2C Command API
## ping
> Description: Debug command to sanity check<br/>
> Command Value: 0x00<br/>
> Parameters: None<br/>
> Output: None<br/>

## set_ignore_ir_reflection
> Description: Sets the receiver if it should or shouldn't ignore messages that were reflected from<br/>
> Command Value: 0x01<br/>
> Parameters: A uint8_t value of 0 or 1. 0 disables the check for ignoring IR reflections while 1 enables it. Other values are ignored<br/>
> Output: None<br/>

## set_ir_mode
> Description: Sets the send/receive mode of the IR system<br/>
> Command Value: 0x02<br/>
> Parameters: A uint8_t value of 0 or 1. 0 puts the SAO in public mode while 1 puts it in address mode. Other values are ignored<br/>
> Output: None<br/>

## enable_ir_buffer
> Description: Enables or disables the ability to buffer IR data on the ATTINY85. Note: clears the receive buffer when this is changed<br/>
> Command Value: 0x03<br/>
> Parameters: A uint8_t value of 0 or 1. 0 disables the receive buffer while 1 enables it. Other values are ignored<br/>
> Output: None<br/>
> Note: Clears the IR receive buffer when command is sent no matter what the new mode value is<br/>

## set_ir_address
> Description: Sets the IR address of this ATTINY85<br/>
> Command Value: 0x04<br/>
> Parameters: A uint8_t value between 0 and 255. This value represents the IR address of the SAO<br/>
> Output: None<br/>

## get_ir_address
> Description: Send the IR address of this ATTINY85 over I2C<br/>
> Command Value: 0x05<br/>
> Parameters: None<br/>
> Output: Returns a uint8_t value between 0 and 255. This value represents the IR address of the SAO<br/>

## clear_ir_receive_buffer
> Description: Clears the IR Receive Buffer<br/>
> Command Value: 0x06<br/>
> Parameters: None<br/>
> Output: None<br/>

## get_ir_receive_buffer_avaliable
> Description: Get the number of bytes in the IR Receive Buffer<br/>
> Command Value: 0x07<br/>
> Parameters: None<br/>
> Output: Returns a uint8_t value between 0 and 255. This value represents the number of bytes in the IR receive buffer<br/>
> Note: Will still return 0 or 1 if the IR receive buffer is disabled<br/>

## read_ir_byte
> Description: Send a byte from the IR Buffer. Bytes come in FIFO ordering. Buffer is left shifted with zeros<br/>
> Command Value: 0x08<br/>
> Parameters: None<br/>
> Output: Returns a uint8_t value between 0 and 255. Value is received IR data in the received IR buffer<br/>
> Note: It is not guarenteed that there will be a byte sent! Use get_ir_receive_buffer_avaliable to make sure there is a byte to read, otherwise nothing will get sent over I2C!<br/>

## write_ir_byte
> Description: Send a byte out over IR. The first received i2c byte is the address and the second is the data<br/>
> Command Value: 0x09<br/>
> Parameters: Two uint8_t values between 0 and 255. The first value represents the IR address and the second represents data that will be transmitted<br/>
> Output: None<br/>

# Samples
A few samples provided to test various functions of the SAO. 

# ATTiny85 Pinout
![alt text](attiny85_pinout.png)

# Libraries  Used
- [ATtinySerialOut](https://github.com/ArminJo/ATtinySerialOut)
- [Arduino-IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote)
