#include <hardware_inits.h>

void hardware_init(void)
{
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
	
	// uart config
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
	irq_register_handler( ADC_IRQn, 6 );	// reading analog input and set timer for z axis speed
	
	Enable_global_interrupt();
	
};
