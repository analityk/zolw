#include <in_out.h>

TS_IO_State io_state;

void io_init(IO_callback io_callback)
{	
	io_callback = io_drive;
	
	REG_PIOA_WPMR = 0x50494F00;
	REG_PIOB_WPMR = 0x50494F00;
	REG_PIOC_WPMR = 0x50494F00;
	REG_PIOD_WPMR = 0x50494F00;
	
	REG_PIOA_IDR = (PIO_IN_A_PIN_MASK)|(PIO_OUT_A_PIN_MASK);
	REG_PIOB_IDR = (PIO_IN_B_PIN_MASK)|(PIO_OUT_B_PIN_MASK);
	REG_PIOC_IDR = (PIO_IN_C_PIN_MASK)|(PIO_OUT_C_PIN_MASK);
	REG_PIOD_IDR = (PIO_IN_D_PIN_MASK)|(PIO_OUT_D_PIN_MASK);
	
	REG_PIOA_PER = (PIO_IN_A_PIN_MASK)|(PIO_OUT_A_PIN_MASK);
	REG_PIOB_PER = (PIO_IN_B_PIN_MASK)|(PIO_OUT_B_PIN_MASK);
	REG_PIOC_PER = (PIO_IN_C_PIN_MASK)|(PIO_OUT_C_PIN_MASK);
	REG_PIOD_PER = (PIO_IN_D_PIN_MASK)|(PIO_OUT_D_PIN_MASK);
	
	REG_PIOA_OER = PIO_OUT_A_PIN_MASK;
	REG_PIOB_OER = PIO_OUT_B_PIN_MASK;
	REG_PIOC_OER = PIO_OUT_C_PIN_MASK;
	REG_PIOD_OER = PIO_OUT_D_PIN_MASK;
	
	REG_PIOA_ODR = PIO_IN_A_PIN_MASK;
	REG_PIOB_ODR = PIO_IN_B_PIN_MASK;
	REG_PIOC_ODR = PIO_IN_C_PIN_MASK;
	REG_PIOD_ODR = PIO_IN_D_PIN_MASK;
	
	//pio_configure(PIOA, PIO_TYPE_PIO_OUTPUT_0, PIO_OUT_A_PIN_MASK, 0);
	//pio_configure(PIOB, PIO_TYPE_PIO_OUTPUT_0, PIO_OUT_B_PIN_MASK, 0);
	//pio_configure(PIOC, PIO_TYPE_PIO_OUTPUT_0, PIO_OUT_C_PIN_MASK, 0);
	//pio_configure(PIOD, PIO_TYPE_PIO_OUTPUT_0, PIO_OUT_D_PIN_MASK, 0);
	//
	//pio_configure(PIOA, PIO_TYPE_PIO_INPUT, PIO_IN_A_PIN_MASK, 0);
	//pio_configure(PIOB, PIO_TYPE_PIO_INPUT, PIO_IN_B_PIN_MASK, 0);
	//pio_configure(PIOC, PIO_TYPE_PIO_INPUT, PIO_IN_C_PIN_MASK, 0);
	//pio_configure(PIOD, PIO_TYPE_PIO_INPUT, PIO_IN_D_PIN_MASK, 0);
	
	pio_set(PIOA, PIO_OUT_A_PIN_MASK);
	pio_set(PIOB, PIO_OUT_B_PIN_MASK);
	pio_set(PIOC, PIO_OUT_C_PIN_MASK);
	pio_set(PIOD, PIO_OUT_D_PIN_MASK);
	
	pio_pull_up(PIOA, PIO_IN_A_PIN_MASK, PIO_PULLUP);
	pio_pull_up(PIOB, PIO_IN_B_PIN_MASK, PIO_PULLUP);
	pio_pull_up(PIOC, PIO_IN_C_PIN_MASK, PIO_PULLUP);
	pio_pull_up(PIOD, PIO_IN_D_PIN_MASK, PIO_PULLUP);
	
};

uint32_t get_pio_a(void)
{
	return REG_PIOA_PDSR;
};

uint32_t get_pio_b(void)
{
	return REG_PIOB_PDSR;
};

uint32_t get_pio_c(void)
{
	return REG_PIOC_PDSR;
};

uint32_t get_pio_d(void)
{
	return REG_PIOD_PDSR;
};

void io_drive(TS_IO_State* ios)
{
	if( get_pio_c() & PIO_PC16 ){
		pio_clear(PIOB, PIO_PB27);
	}else{
		pio_set(PIOB, PIO_PB27);
	};
};
