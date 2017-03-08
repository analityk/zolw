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


// max speed 8000 steps/s
#include <stdlib.h>

#include <asf.h>
#include <in_out.h>
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
	
	io_state.pio_a_state = get_pio_a();
	io_state.pio_b_state = get_pio_b();
	io_state.pio_c_state = get_pio_c();
	io_state.pio_d_state = get_pio_d();
	
	io_drive(&io_state);
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
	
	hardware_init();
	
	io_init(io_drive);
	
	// pid config
	thc_set_pid(0.001, 250, 1, 0.010);
	thc_set_height_v(100.0);
	thc_enable();
		
	//homing();
	
	//set_speed_acc(1600);
	
	while(1){
		line(0,0,1000,0);
		line(1000,0,1000,1000);
		line(1000,1000, 1000, 0 );
		line(1000,0, 0, 0);
	};

};

void delay(uint32_t volatile t){
	while(t--){};
};
