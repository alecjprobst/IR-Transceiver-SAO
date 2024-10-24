// System Definitions
#include "PinDefinitionsAndMore.h"

// IR Definitions
#include "TinyIRReceiver.hpp"
#include "TinyIRSender.hpp"

// I2C Definitions
#include <TinyWireS.h>

// Program Variables
uint8_t mode = 0;
bool enable_buffer = false;
uint8_t address_of_this_sao = 0;
const uint8_t receive_buffer_size = 256;
uint8_t receive_buffer[receive_buffer_size];
uint8_t number_bytes_in_receive_buffer = 0;

void setup()
{
    TinyWireS.begin(0x08);
    TinyWireS.onReceive(receiveEvent);
    // IR_RECEIVE_PIN in PinDefinitionsAndMore.h automatically overrides default reciever pin
    initPCIInterruptForTinyReceiver();    // Enables the interrupt generation on change of IR input signal
}

void loop() 
{
}

// I2C switch statement to determine what the ATTINY85 should do based on commands
void receiveEvent(uint8_t howMany) 
{
    // Check for size of data coming from I2C
    uint8_t receivedDataSize = TinyWireS.available();
    if(receivedDataSize > 1)
    {
        // Assumes that first byte is always a command byte
        uint8_t command = receive_i2c_data();
        switch(command)
        {
            // Sets the send/receive mode of the IR system
            case set_ir_mode:
                uint8_t new_mode = receive_i2c_data();
                if(new_mode <= max_mode_value)
                {
                    mode = new_mode;
                }
                //TODO: Maybe send an error code or something?
                break;
            // Enables or disables the ability to buffer IR data on the ATTINY85. Note: clears the receive buffer when this is changed
            case enable_ir_buffer:
                uint8_t new_enable_buffer = receive_i2c_data();
                if(new_enable_buffer == 0)
                    enable_buffer = false;
                else if(new_enable_buffer == 1)
                    enable_buffer = true;
                else
                    break;
                clear_receive_buffer();
                break;
            // Sets the IR address of this ATTINY85
            case set_ir_address:
                address_of_this_sao = receive_i2c_data();
                break;
            // Send the IR address of this ATTINY85 over I2C
            case get_ir_address:
                send_i2c_data(address_of_this_sao);
                break;
            // Clear the IR Receive Buffer
            case clear_ir_receive_buffer:
                clear_receive_buffer();
                break;
            // Get the number of bytes in the IR Receive Buffer
            case get_ir_receive_buffer_avaliable:
                send_i2c_data(number_bytes_in_receive_buffer);
                break;
            // Send a byte from the IR Buffer. Bytes come in FIFO ordering. Buffer is left shifted with zeros
            case read_ir_byte:
                if(number_bytes_in_receive_buffer > 0)
                {
                    send_i2c_data(receive_buffer[0]);
                    left_shift_receive_buffer();
                }
                //TODO: Maybe send an error code or something?
                break;
            // Send a byte out over IR. The specific address sent over is determined by the mode
            case write_ir_byte:
                uint8_t data_byte = receive_i2c_data();
                if(mode == mode_public)
                {
                    sendNEC(IR_SEND_PIN, ir_public_address, data_byte);
                }
                else if(mode == mode_address)
                {
                    sendNEC(IR_SEND_PIN, address_of_this_sao, data_byte);
                }
                break;
            default:
                //TODO: Maybe send an error code or something?
                break;
        }
    }
}

// Clears the entire receive buffer and resets the number of bytes variable to zero
void clear_receive_buffer()
{
    for(int i=0; i < receive_buffer_size; i++)
    {
        receive_buffer[i] = 0;
    }
    number_bytes_in_receive_buffer = 0;
}

// Shifts all values in the receive buffer to the left by one position. Zeros out the last position moved in the buffer
void left_shift_receive_buffer()
{
    if(number_bytes_in_receive_buffer > 0)
    {
        for(int i = 0; i < number_bytes_in_receive_buffer; i++)
        {
            receive_buffer[i] = receive_buffer[i+1];
        }
        number_bytes_in_receive_buffer--;
        receive_buffer[number_bytes_in_receive_buffer] = 0;
    }
}

// Generic function to send i2c data
void send_i2c_data(uint8_t data)
{
    TinyWireS.send(data);
}

// Generic function to receive i2c data
uint8_t receive_i2c_data()
{
    return TinyWireS.receive();
}