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

#include <asf.h>
#include <math.h>
#include <motors.h>
#include <cyclic_buffer.h>
#include <tracker.h>

uint8_t led = 0;
uint32_t volatile rs_edge = 0;
uint32_t volatile adc_read_tab[10];
uint8_t volatile adc_tab_prt = 0;

uint32_t volatile adc_set = 2048;
uint32_t volatile adc_read = 0;
int32_t volatile adc_error = 0;

double volatile adc_factor = 20.48;

// receive data
ISR( UART_Handler ){
	uint32_t volatile status = REG_UART_SR;
	UNUSED(status);
	uint8_t volatile data = REG_UART_RHR;
	REG_UART_THR = data;// cb_receive(data);
};

// motors speed
ISR( TC0_Handler ){
	uint32_t volatile tc0_sr0 = REG_TC0_SR0;
	UNUSED(tc0_sr0);
	lock = 0;
	REG_TC0_RC0 = step_time;
};

// io drive
ISR( TC1_Handler ){
	uint32_t volatile tc0_sr1 = REG_TC0_SR1;
	UNUSED(tc0_sr1);

	uint8_t pin = pio_get(PIOC, PIO_TYPE_PIO_INPUT, PIO_PC16);
	if( pin ){
		REG_TC0_RC1 = 21000;
	}else{
		REG_TC0_RC1 = 7000;
	};
};

// motors accelerate
ISR( TC2_Handler ){
	uint32_t volatile tc0_sr2 = REG_TC0_SR2;
	UNUSED(tc0_sr2);
	
	if( acl_phase == 1 ){
		if( target_speed > actual_speed ){
			actual_speed++;
			set_speed(actual_speed);
			REG_TC0_RC2 = accel;//accelerate;
		}else{
			acl_phase = 0;
			REG_TC0_RC2 = accel;
		};
	};
		
	if( dcl_phase == 1 ){
		if( actual_speed > target_speed ){
			actual_speed--;
			set_speed(actual_speed);
			REG_TC0_RC2 = accel;//decelerate;
		}else{
			dcl_phase = 0;
			REG_TC0_RC2 = accel;
		};
	};
};

// z axis drive
ISR( TC3_Handler ){
	uint32_t volatile tc1_sr0 = REG_TC1_SR0;
	UNUSED(tc1_sr0);
	z_step();
};

// thc 
ISR( ADC_Handler ){
	uint32_t volatile adc_isr = REG_ADC_ISR;
	UNUSED(adc_isr);
	adc_read_tab[adc_tab_prt] = adc_get_channel_value(ADC, ADC_CHANNEL_14);
	adc_tab_prt++;
	if( adc_tab_prt > 10 ){
		adc_tab_prt = 0;
		
		uint32_t sum = 0;
		for( uint8_t i=0; i<10; i++ ){
			sum += adc_read_tab[i];
		};
		sum /= 10;
		
		uint32_t act_voltage = sum;
		
		// jeœli napiêcie ³uku jest za du¿e
		if( act_voltage > adc_set ){
			z_dir_left();
		}else{
			z_dir_right();
		};
		
		adc_error = abs(act_voltage - adc_set);
		
		if(adc_error > 20){
			REG_TC1_CCR0 = 5;
			uint32_t tc_rc = 42000000/ (adc_error * 100);
			if(tc_rc < 1000 ){
				tc_rc = 1000;
			};
			REG_TC1_RC0 = tc_rc;
		}else{
			REG_TC1_CCR0 = 2;
		};
		
	};
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
	
	//pid_set_param(&pid_adc, 8, 2, 1);
	//pid_set_dt(&pid_adc, 0.001);
	//pid_set_setpoint(&pid_adc, 3000);
	//
	// steruje prêdkoœci¹ obrotow¹ silników. 
	pmc_enable_periph_clk( ID_TC0 );
	
	// w³¹cza zegar dla timera tc0 id1 - bardzo wa¿na rzecz, nie wy³¹czaæ
	pmc_enable_periph_clk( ID_TC1 );
	
	// w³¹cza timer dla przyspieszania
	pmc_enable_periph_clk( ID_TC2 );
	
	// w³¹cza timer dla thc
	pmc_enable_periph_clk( ID_TC3 );
	
	// w³¹cza zegar dla adc
	pmc_enable_periph_clk( ID_ADC );
	
	// config adc - free run mode, interrupt on adc conversion completed, chanel 14,
	REG_ADC_WPMR = 0x41444300;
	REG_ADC_MR = (ADC_MR_TRGEN_DIS)|(ADC_MR_LOWRES_BITS_12)|(ADC_MR_SLEEP_NORMAL)|(ADC_MR_FWUP_OFF)|(ADC_MR_FREERUN_ON)\
				|(ADC_MR_PRESCAL(200))|(ADC_MR_STARTUP_SUT80)|(ADC_MR_SETTLING_AST5)|(ADC_MR_ANACH_NONE)|(ADC_MR_TRACKTIM(8))\
				|(ADC_MR_TRANSFER(1))|(ADC_MR_USEQ_NUM_ORDER);
	REG_ADC_CHER = (ADC_CHER_CH14);
	REG_ADC_IER = (1<<14);
	REG_ADC_EMR = 0;
	REG_ADC_CGR = 0;
	REG_ADC_COR = 0;
	
	REG_UART_CR = (UART_CR_RXEN)|(UART_CR_TXEN)|(UART_CR_RSTSTA);
	REG_UART_MR = (UART_MR_PAR_NO)|(UART_MR_CHMODE_NORMAL);
	REG_UART_BRGR = 42;	// 125kbps
	REG_UART_IER = UART_IER_RXRDY;
	
	// licznik do sterowania prêdkoœci¹ silników
	REG_TC0_CCR0 = 5;
	REG_TC0_WPMR = 0x54494D00;
	REG_TC0_CMR0 = (2<<13)|(1<<15);
	REG_TC0_RC0 = 42000000;
	REG_TC0_CCR0 = 5;
	
	REG_TC0_IER0 = (1<<4);
	
	// timer zg³asza przerwanie co 1 ms, wtedy odczytuje stan portów od np. krañcówek. 
	REG_TC0_CCR1 = 5;
	REG_TC0_WPMR = 0x54494D00;
	REG_TC0_CMR1 = (2<<13)|(1<<15);
	REG_TC0_RC1 = 21000;
	REG_TC0_CCR1 = 5;
	
	REG_TC0_IER1 = (1<<4);
	
	// accelerate control
	REG_TC0_CCR2 = 5;
	REG_TC0_WPMR = 0x54494D00;
	REG_TC0_CMR2 = (2<<0)|(2<<13)|(1<<15);
	REG_TC0_RC2 = 42000;
	REG_TC0_CCR2 = 5;
		
	REG_TC0_IER2 = (1<<4);
	
	// torh height control (adc-step/dir for x axis)
	REG_TC1_CCR0 = 5;
	REG_TC1_WPMR = 0x54494D00;
	REG_TC1_CMR0 = (2<<13)|(1<<15);
	REG_TC1_RC0 = 42000000;
	REG_TC1_CCR0 = 5;
	
	REG_TC1_IER0 = (1<<4);
	
	irq_initialize_vectors();
	irq_register_handler( TC1_IRQn, 1 );	// I-O drive
	irq_register_handler( TC0_IRQn, 2 );	// speed control
	irq_register_handler( TC2_IRQn, 4 );	// accelerate control
	irq_register_handler( TC3_IRQn, 5 );	// torh height control (adc-step/dir for x axis)
	irq_register_handler( UART_IRQn, 3 );	// get data from uart
	irq_register_handler( ADC_IRQn, 6 );
	
	Enable_global_interrupt();
	
	set_speed_acc(100);
	line(0,0,1,1);
	line(1,1,0,0);
	
	while(1){
		set_speed_acc(20000);
		line(0,0,0,1000000);
	};

};

void delay(uint32_t volatile t){
	while(t--){};
};
