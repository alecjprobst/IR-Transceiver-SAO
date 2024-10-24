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
#define i2c_set_mode 0
#define i2c_enable_buffer 1
#define i2c_set_address 2
#define i2c_get_address 3
#define i2c_clear_receive_buffer 4
#define i2c_get_receive_buffer_avaliable 5
#define i2c_read_byte 6
#define i2c_write_byte 7

// Mode Definitions
#define mode_public 0
#define mode_address 1
#define max_mode_value mode_address