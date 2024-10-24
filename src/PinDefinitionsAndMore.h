#define F_CPU 8000000

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
#define set_ir_mode 0
#define enable_ir_buffer 1
#define set_ir_address 2
#define get_ir_address 3
#define clear_ir_receive_buffer 4
#define get_ir_receive_buffer_avaliable 5
#define read_ir_byte 6
#define write_ir_byte 7

// Mode Definitions
#define ir_public_address 0
#define mode_public 0
#define mode_address 1
#define max_mode_value mode_address