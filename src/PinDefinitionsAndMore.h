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
#define ping 0
#define set_ignore_ir_reflection 1
#define set_ir_mode 2
#define enable_ir_receiver_buffer 3
#define set_ir_address 4
#define get_ir_address 5
#define clear_ir_receive_buffer 6
#define get_ir_receive_buffer_avaliable 7
#define read_ir_byte 8
#define write_ir_byte 9
#define write_to_ir_write_buffer 10
#define get_ir_write_buffer 11
#define get_ir_write_buffer_avaliable 12
#define clear_ir_write_buffer 13
#define set_ir_write_buffer_address 14
#define get_ir_write_buffer_address 15

// Mode Definitions
#define ir_public_address 0
#define mode_public 0
#define mode_address 1
#define max_mode_value mode_address