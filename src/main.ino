// System Definitions
#include "PinDefinitionsAndMore.h"

// IR Definitions
#include "TinyIRReceiver.hpp"
#include "TinyIRSender.hpp"

// I2C Definitions
#include <Wire.h>
#include <Arduino.h>

// Program Variables
uint8_t mode = 0;

bool enable_buffer = false;

bool ignore_ir_reflection = true;
volatile uint8_t last_sent_ir_command = 0;
volatile uint8_t last_sent_ir_data = 0;
volatile unsigned long last_time_ir_data_sent = 0;
const uint8_t max_ir_transmission_time = 70;

uint8_t address_of_this_sao = 0;

// Read Buffer
const uint8_t receive_buffer_size = 128;
uint8_t receive_buffer[receive_buffer_size];
volatile uint8_t number_bytes_in_receive_buffer = 0;

// Writer Buffer
//      Why is the Write Buffer so small? 
//      Because we ran out of memory of course. :D 
const uint8_t write_buffer_size = 32;
uint8_t write_buffer_address = 0;
uint8_t write_buffer[write_buffer_size];
volatile uint8_t number_bytes_in_write_buffer = 0;


uint8_t latest_i2c_received_command = 0;

const char message[] PROGMEM = "pong";  // Store "pong" in flash

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup
// 
// Sets up Wire (i2c) request and recieve events.
// Initialize IR Receiver.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
    pinMode(BUTTON_PIN_1, INPUT_PULLUP);  

    // Clear buffer to make sure we don't have bad data
    clear_buffer(receive_buffer, &number_bytes_in_receive_buffer, receive_buffer_size);
    clear_buffer(write_buffer, &number_bytes_in_write_buffer, write_buffer_size);

    // HACKADAY_Supercon_8
    write_buffer[0] = 'S';
    write_buffer[1] = 'U';
    write_buffer[2] = 'P';
    write_buffer[3] = 'E';
    write_buffer[4] = 'R';
    write_buffer[5] = 'C';
    write_buffer[6] = 'O';
    write_buffer[7] = 'N';
    write_buffer[8] = '_';
    write_buffer[9] = '8';
    number_bytes_in_write_buffer = 10;

    // Begin I2C transactions
    Wire.begin(0x08);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    // IR_RECEIVE_PIN in PinDefinitionsAndMore.h automatically overrides default reciever pin
    initPCIInterruptForTinyReceiver();    // Enables the interrupt generation on change of IR input signal
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Loop
//
// Primary IR Event Handler - Check for received IR signals
//////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
    // Loop where we check to see if we recieved any data
    if (TinyReceiverDecode()) 
    {
        uint8_t ir_address = get_received_ir_address();
        uint8_t ir_data = get_received_ir_data();

        // Check what mode we're in to check address. If checking addresses and address doesn't match ir address of this SAO, return
        if(mode == mode_address  && ir_address != address_of_this_sao)
            return;

        // Check if we're ignoring the last receieved message to prevent reflected data transmission
        if(ignore_ir_reflection == true && (millis() - last_time_ir_data_sent) < max_ir_transmission_time && (ir_address == last_sent_ir_command || mode == mode_public) && ir_data == last_sent_ir_data)
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
        }
        // If not using buffer, always overwrite the first data position with new data
        else
        {
            receive_buffer[0] = ir_data;
            number_bytes_in_receive_buffer = 1;
        }
    }

    
    if (digitalRead(BUTTON_PIN_1) == LOW) {
        if(number_bytes_in_write_buffer > 0)
        {
            for(int i = 0; i < number_bytes_in_write_buffer; i++)
            {
                sendNEC(IR_SEND_PIN, write_buffer_address, write_buffer[i]);
                delay(10);
            }
        }
        
        while (digitalRead(BUTTON_PIN_1) == LOW) {
            delay(50);  // Debouncing delay
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

// Send out data being requested based on latest i2c command
void requestEvent()
{
    switch(latest_i2c_received_command)
    {
        // Debug command to sanity check
        case ping:
        {
            char buffer[5];
            strcpy_P(buffer, message);
            Wire.write(buffer);
            break;
        }

        // Send the IR address of this ATTINY85 over I2C
        case get_ir_address:
        {
            send_i2c_data(address_of_this_sao);
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
            break;
        }

        case get_ir_write_buffer:
        {
            if(number_bytes_in_write_buffer > 0)
            {
                for(int i = 0; i < number_bytes_in_write_buffer; i++)
                {
                    send_i2c_data(write_buffer[i]);
                }
            }
            break;
        }

        case get_ir_write_buffer_avaliable:
        {
            send_i2c_data(number_bytes_in_write_buffer);
            break;
        }

        case get_ir_write_buffer_address:
        {
            send_i2c_data(write_buffer_address);
            break;
        }

        default:
        {
            // TODO: default error
            break;
        }
    }
}

// I2C switch statement to determine what the ATTINY85 should do based on commands
void receiveEvent(int howMany) 
{
    // Check for size of data coming from I2C
    uint8_t receivedDataSize = Wire.available();
    if(receivedDataSize > 0)
    {
        // Assumes that first byte is always a command byte
        latest_i2c_received_command = receive_i2c_data();

        switch(latest_i2c_received_command)
        {
            // Debug command to sanity check
            case ping:
            {
                // Do nothing here as data will be sent out in requestEvent()
                break;
            }

            // Sets the receiver if it should or shouldn't ignore messages that were reflected from
            case set_ignore_ir_reflection:
            {
                uint8_t new_set_ignore_ir_reflection = receive_i2c_data();
                if(new_set_ignore_ir_reflection == 0)
                    ignore_ir_reflection = false;
                else if(new_set_ignore_ir_reflection == 1)
                    ignore_ir_reflection = true;
                else
                    break;
            }

            // Sets the send/receive mode of the IR system
            case set_ir_mode:
            {
                uint8_t new_mode = receive_i2c_data();
                if(new_mode <= max_mode_value)
                {
                    mode = new_mode;
                }
                break;
            }

            // Enables or disables the ability to buffer IR data on the ATTINY85. Note: clears the receive buffer when this is changed
            case enable_ir_receiver_buffer:
            {
                uint8_t new_enable_buffer = receive_i2c_data();
                if(new_enable_buffer == 0)
                    enable_buffer = false;
                else if(new_enable_buffer == 1)
                    enable_buffer = true;
                else
                    break;
                clear_buffer(receive_buffer, &number_bytes_in_receive_buffer, receive_buffer_size);
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
                // Do nothing here as data will be sent out in requestEvent()
                break;
            }

            // Clears the IR Receive Buffer
            case clear_ir_receive_buffer:
            {
                clear_buffer(receive_buffer, &number_bytes_in_receive_buffer, receive_buffer_size);
                break;
            }

            // Get the number of bytes in the IR Receive Buffer
            case get_ir_receive_buffer_avaliable:
            {
                // Do nothing here as data will be sent out in requestEvent()
                break;
            }

            // Send a byte from the IR Buffer. Bytes come in FIFO ordering. Buffer is left shifted with zeros
            case read_ir_byte:
            {
                // Do nothing here as data will be sent out in requestEvent()
                break;
            }

            // Send a byte out over IR. The first received i2c byte is the address and the second is the data
            case write_ir_byte:
            {
                uint8_t address_byte = receive_i2c_data();
                uint8_t data_byte = receive_i2c_data();

                // Set the last send IR command and data and update time last IR was sent
                last_sent_ir_command = address_byte;
                last_sent_ir_data = data_byte;
                last_time_ir_data_sent = millis();

                // Send the data out over IR
                sendNEC(IR_SEND_PIN, address_byte, data_byte);

                break;
            }

            case write_to_ir_write_buffer:
            {
                uint8_t data_byte = receive_i2c_data();

                // Must check to not overflow buffer
                if(number_bytes_in_write_buffer < write_buffer_size)
                {
                    write_buffer[number_bytes_in_write_buffer] = data_byte;
                    number_bytes_in_write_buffer++;
                }
                break;
            }

            case get_ir_write_buffer:
            {
                // Do nothing here as data will be sent out in requestEvent()
                break;
            }

            case get_ir_write_buffer_avaliable:
            {
                // Do nothing here as data will be sent out in requestEvent()
                break;
            }

            // Clears the IR Receive Buffer
            case clear_ir_write_buffer:
            {
                clear_buffer(write_buffer, &number_bytes_in_write_buffer, write_buffer_size);
                break;
            }

            case set_ir_write_buffer_address:
            {
                write_buffer_address = receive_i2c_data();
                break;
            }

            case get_ir_write_buffer_address:
            {
                // Do nothing here as data will be sent out in requestEvent()
                break;
            }

            default:
            {
                // TODO: default error
                break;
            }
        }
    }
}

// Clears the entire receive buffer and resets the number of bytes variable to zero
void clear_buffer(uint8_t buffer[], volatile uint8_t *buffer_count, uint8_t buffer_max_size)
{
    for(int i=0; i < buffer_max_size; i++)
    {
        buffer[i] = 0;
    }
    *buffer_count = 0;
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

// Generic function to send 1 byte of i2c data
void send_i2c_data(uint8_t data)
{
    Wire.write(data);
}

// Generic function to receive 1 byte of i2c data
uint8_t receive_i2c_data()
{
    return Wire.read();
}
