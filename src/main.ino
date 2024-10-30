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
const uint8_t receive_buffer_size = 255;
uint8_t receive_buffer[receive_buffer_size];
uint8_t number_bytes_in_receive_buffer = 0;

void setup()
{
    // Clear buffer to make sure we don't have bad data
    clear_receive_buffer();
    // Begin I2C transactions
    TinyWireS.begin(0x08);
    TinyWireS.onReceive(receiveEvent);
    // IR_RECEIVE_PIN in PinDefinitionsAndMore.h automatically overrides default reciever pin
    initPCIInterruptForTinyReceiver();    // Enables the interrupt generation on change of IR input signal
}

// Check for received IR signals
void loop() 
{
    // Loop where we check to see if we recieved any data
    if (TinyReceiverDecode()) 
    {
        uint8_t ir_data = get_received_ir_data();
        uint8_t ir_address = get_received_ir_address();

        // Check what mode we're in to check address. If checking addresses and address doesn't match ir address of this SAO, return
        if(mode == mode_address  && ir_address != address_of_this_sao)
            return;
        
        // Check if using buffer or not
        if(enable_buffer)
        {
            // Must check to not overflow buffer
            if(number_bytes_in_receive_buffer < receive_buffer_size)
            {
                receive_buffer[number_bytes_in_receive_buffer] = ir_data;
                number_bytes_in_receive_buffer++;
            }
            //TODO: Maybe send an warning code or something about buffer being full?
        }
        else
        {
            // Always overwrite the first data position with new data
            receive_buffer[0] = ir_data;
            number_bytes_in_receive_buffer = 1;
        }
    }
}

// Get polarized data from IR and return non-inverted value
uint8_t get_received_ir_data()
{
    // NEC protocol has 16 bit Command. Command is made up of 8 bit data, 8 bit inverted data (for polarity check)
    uint16_t recieved_data_polarity = TinyIRReceiverData.Command;
    uint8_t recieved_data = recieved_data_polarity & 0xFF; // Removed upper bits to get non-inverted value
    return recieved_data;
}

// Get polarized address from IR and return non-inverted value
uint8_t get_received_ir_address()
{
    // NEC protocol has 16 bit address. Address is made up of 8 bit data, 8 bit inverted data (for polarity check)
    uint16_t recieved_address_polarity = TinyIRReceiverData.Address;
    uint16_t recieved_address = recieved_address_polarity & 0xFF; // Removed upper bits to get non-inverted value
    return recieved_address;
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
            {
                uint8_t new_mode = receive_i2c_data();
                if(new_mode <= max_mode_value)
                {
                    mode = new_mode;
                }
                //TODO: Maybe send an error code or something?
                break;
            }
            // Enables or disables the ability to buffer IR data on the ATTINY85. Note: clears the receive buffer when this is changed
            case enable_ir_buffer:
            {
                uint8_t new_enable_buffer = receive_i2c_data();
                if(new_enable_buffer == 0)
                    enable_buffer = false;
                else if(new_enable_buffer == 1)
                    enable_buffer = true;
                else
                    break;
                clear_receive_buffer();
                break;
            }
            // Sets the IR address of this ATTINY85
            case set_ir_address:
            {
                address_of_this_sao = receive_i2c_data();
                break;
            }
            // Send the IR address of this ATTINY85 over I2C
            case get_ir_address:
            {
                send_i2c_data(address_of_this_sao);
                break;
            }
            // Clears the IR Receive Buffer
            case clear_ir_receive_buffer:
            {
                clear_receive_buffer();
                break;
            }
            // Get the number of bytes in the IR Receive Buffer
            case get_ir_receive_buffer_avaliable:
            {
                send_i2c_data(number_bytes_in_receive_buffer);
                break;
            }
            // Send a byte from the IR Buffer. Bytes come in FIFO ordering. Buffer is left shifted with zeros
            case read_ir_byte:
            {
                if(number_bytes_in_receive_buffer > 0)
                {
                    send_i2c_data(receive_buffer[0]);
                    left_shift_receive_buffer();
                }
                //TODO: Maybe send an error code or something?
                break;
            }
            // Send a byte out over IR. The first received i2c byte is the address and the second is the data
            case write_ir_byte:
            {
                uint8_t address_byte = receive_i2c_data();
                uint8_t data_byte = receive_i2c_data();
                sendNEC(IR_SEND_PIN, address_byte, data_byte);
                break;
            }
            default:
            {
                //TODO: Maybe send an error code or something?
                break;
            }
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