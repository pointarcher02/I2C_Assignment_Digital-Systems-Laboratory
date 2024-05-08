#include "I2C_DS_Assignment_B21CS093_B21CS074.h"


void delay(int num){
	int k = 0;
	for(int i = 0 ; i < num ; i++){
			for(int j = 0 ; j < 1000 ; j++){
				k++;
			}
	}
}


void Set_I2C_Timings(uint8_t Clock_freq, uint8_t ccr_val, uint8_t trise_value){
	I2C1->CR2 |= (Clock_freq); 							// Setting the PCLK2 frequency
	I2C1->CCR = ccr_val;  									// Clock Configuration Register
	I2C1->TRISE = trise_value;							// Rise time Calculated from Manual
}


void I2C_pin_Init(void){
	RCC->APB1ENR |= (1<<21);  							// Enable Clock for I2C1
	RCC->AHB1ENR |= (1<<1);									// Enable Port B clock
	GPIOB->MODER |= (2<<16) | (2<<18); 			// Set AF Mode
	GPIOB->PUPDR |= (1<<16) | (1<<18); 			// Pull Up mode Because I2C follows default HIGH
	GPIOB->AFR[1] |= (4<<0) | (4<<4); 			// AFR -->  4 for I2C1
}
	

void I2C_Peripheral_Init(void){
	I2C1->CR1 |= (1<<15); 									// Software Reset
	I2C1->CR1 &= ~(1<<15);									// Software Reset
	Set_I2C_Timings(45,225,46);							// Setting I2C clock timings and edge time
	I2C1->CR1 |= (1<<0); 										// Enabling the I2C on
	I2C1->CR1 |= (1<<10);										// Enabling the Acknowledgement bit
}


void I2C_generate_start(){
	I2C1->CR1 |= (1<<8);  									// Generate the start Condition
	while (!(I2C1->SR1 & (1<<0))); 					// Wait till the SB (flag) is set
}


void I2C_send_address(uint8_t address){
	I2C1->DR = (address);  									// Put Address in Data Register (DR) to send
	while (!(I2C1->SR1 & (1<<1))); 					// Wait ADDR bit is set
	uint8_t temp = I2C1->SR1 | I2C1->SR2;		// Clear ADDR by reading SR1 and SR2 sequentially
}


void I2C_send_data(uint8_t data){
	while (!(I2C1->SR1 & (1<<7)));  				// Wait till TXE not set
	I2C1->DR = data;												// Put Data in DR
	while (!(I2C1->SR1 & (1<<2)));					// Wait till BTF set
}


void I2C_generate_stop(){
	I2C1->CR1 |= (1<<9);										// Set the stop bit
}


int main ()
{
	I2C_pin_Init();
	I2C_Peripheral_Init();
	//------------------Communication starts--------------------
	I2C_generate_start();
	I2C_send_address(0x40);
	I2C_send_data(0x5);
	I2C_generate_stop();
	
	I2C_generate_start();
	I2C_send_address(0x44);
	I2C_send_data(0x2);
	I2C_generate_stop();
	//------------------Communication ends ---------------------
	while(1){}
}
