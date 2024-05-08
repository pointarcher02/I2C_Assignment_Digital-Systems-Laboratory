#include "stm32f4xx.h"                
#include "stm32f429xx.h"
#include "stdint.h"

void I2C_pin_Init(void);
void I2C_Peripheral_Init(void);
void Set_I2C_Timings(uint8_t Clock_freq, uint8_t ccr_val, uint8_t trise_value);
void I2C_generate_start(void);
void I2C_send_address(uint8_t address);
void I2C_send_data(uint8_t data);
void I2C_generate_stop(void);
void delay(int num);