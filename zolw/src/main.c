/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <stdlib.h>

#include <asf.h>
#include <hardware_inits.h>
#include <math.h>
#include <motors.h>
#include <cyclic_buffer.h>
#include <tracker.h>
#include <pid.h>
#include <thc.h>

uint8_t led = 0;
uint32_t volatile rs_edge = 0;

// receive data
ISR( UART_Handler ){
	uint32_t volatile status = REG_UART_SR;
	UNUSED(status);
	uint8_t volatile data = REG_UART_RHR;
	REG_UART_THR = data;// cb_receive(data);
};

// io drive
ISR( TC1_Handler ){
	uint32_t volatile tc0_sr1 = REG_TC0_SR1;
	UNUSED(tc0_sr1);

	uint8_t pin = pio_get(PIOC, PIO_TYPE_PIO_INPUT, PIO_PC16);
	
};

void delay(uint32_t volatile t);

void uart_send(uint8_t data){
	while( ! ( REG_UART_SR &(UART_SR_TXRDY))){};
	REG_UART_THR = data;
};

uint8_t uart_receive(void){
	while( ! ( REG_UART_SR &(UART_SR_RXRDY))){};
	uint8_t volatile t = REG_UART_RHR;
	return t;
};

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	
	board_init();
	
	pio_cfg();
	
	hardware_init();
	
	// pid config
	thc_set_pid(0.001, 15, 0.01, 0.005);
	thc_set_height_v(120.0);
	thc_enable();
		
	//homing();
	//reset_coordinates();
	
	uint32_t speed = 1000;
	
	while(1){
		//set_speed_acc(speed);
		//move(126, 126);
		//speed += 5;
	};

};

void delay(uint32_t volatile t){
	while(t--){};
};
