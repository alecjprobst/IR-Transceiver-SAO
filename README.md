# IR-Transceiver-SAO

Event badges get lonely too. Why not have them chat with each other?  
A Programmable Infrared Multitool SAO.   

PCB Build instructions and component data can be found at the hackaday project <br/>
HACKADAY Project: https://hackaday.io/project/197812-infrared-communication-sao

# Installation
1. Download this Github
2. Open Visual Studio Code with PlatformIO installed
3. Open this project 

# Samples
A few samples provided to test various functions of the SAO. 

# SAO Definitions
TODO

# I2C Communication Format
Communications are made up of two parts. The first is the command and the second is the parameter(s)
* Command - uint8_t: Determines what action you would like for the IR SAO. List of commands below
* Parameter(s) - uint8_t: Extra data specifiec by each command. Separated in uint8_t chunks

# I2C Command API
## set_ir_mode
> Description: Sets the send/receive mode of the IR system<br/>
> Parameters: A uint8_t value of 0 or 1. 0 puts the SAO in public mode while 1 puts it in address mode. Other values are ignored<br/>
> Output: None<br/>
## enable_ir_buffer
> Description: Enables or disables the ability to buffer IR data on the ATTINY85. Note: clears the receive buffer when this is changed<br/>
> Parameters: A uint8_t value of 0 or 1. 0 disables the receive buffer while 1 enables it. Other values are ignored. Clears the IR receive buffer when command is sent<br/>
> Output: None<br/>
## set_ir_address
> Description: Sets the IR address of this ATTINY85<br/>
> Parameters: A uint8_t value between 0 and 255. This value represents the IR address of the SAO<br/>
> Output: None<br/>
## get_ir_address
> Description: Send the IR address of this ATTINY85 over I2C<br/>
> Parameters: None<br/>
> Output: Returns a uint8_t value between 0 and 255. This value represents the IR address of the SAO<br/>
## clear_ir_receive_buffer
> Description: Clears the IR Receive Buffer<br/>
> Parameters: None<br/>
> Output: None<br/>
## get_ir_receive_buffer_avaliable
> Description: Get the number of bytes in the IR Receive Buffer<br/>
> Parameters: None<br/>
> Output: Returns a uint8_t value between 0 and 255. This value represents the number of bytes in the IR receive buffer<br/>
## read_ir_byte
> Description: Send a byte from the IR Buffer. Bytes come in FIFO ordering. Buffer is left shifted with zeros<br/>
> Parameters: None<br/>
> Output: Returns a uint8_t value between 0 and 255. Value is received IR data in the received IR buffer<br/>
## write_ir_byte
> Description: Send a byte out over IR. The first received i2c byte is the address and the second is the data<br/>
> Parameters: Two uint8_t values between 0 and 255. The first value represents the IR address and the second represents data that will be transmitted<br/>
> Output: None<br/>

# ATTiny85 Pinout
![alt text](attiny85_pinout.png)

# Libraries  Used
- [ATtinySerialOut](https://github.com/ArminJo/ATtinySerialOut)
- [Arduino-IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote)
