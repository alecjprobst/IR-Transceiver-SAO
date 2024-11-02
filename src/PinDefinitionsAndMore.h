// IR Definitions
#define IR_RECEIVE_PIN PIN_PB3
#define IR_SEND_PIN PIN_PB4

// Button Definitions
#define BUTTON_PIN_1 PIN_PB1 

// Serial Out Definitions
#define SERIAL_TX_PIN PIN_PB2 

// LED Definitions
#define LED_PIN PIN_PB2

// I2C Command Definitions
#define ping 0 // Validated - dsp
#define set_ignore_ir_reflection 1
#define set_ir_mode 2
#define enable_ir_receiver_buffer 3 // Validated - dsp
#define set_ir_address 4 // Validated - dsp
#define get_ir_address 5 // Validated - dsp
#define clear_ir_receive_buffer 6 // Validated - dsp
#define get_ir_receive_buffer_avaliable 7 // Validated - dsp
#define read_ir_byte 8 
#define write_ir_byte 9 // Validated - dsp
#define write_to_ir_write_buffer 10 // Validated - dsp
#define get_ir_write_buffer 11 // Validated - dsp
#define get_ir_write_buffer_avaliable 12 // Validated - dsp
#define clear_ir_write_buffer 13 // Validated - dsp
#define set_ir_write_buffer_address 14 // Validated - dsp
#define get_ir_write_buffer_address 15 // Validated - dsp
#define get_version 16

// Mode Definitions
#define ir_public_address 0
#define mode_public 0
#define mode_address 1
#define max_mode_value mode_address