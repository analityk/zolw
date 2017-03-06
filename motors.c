#include <motors.h>

uint8_t volatile lock;

void delay_1us(void){
	uint32_t volatile t = 30;
	while(t--){};
};

void z_dir_left(void){
	pio_set(PIOD, PIO_PD10);
	pio_clear(PIOC, PIO_PC1);
};

void z_dir_right(void){
	pio_clear(PIOD, PIO_PD10);
	pio_set(PIOC, PIO_PC1);
};

void z_step(void){
	pio_set(PIOC, PIO_PC3);
	pio_clear(PIOC, PIO_PC2);
	delay_1us();
	pio_clear(PIOC, PIO_PC3);
	pio_set(PIOC, PIO_PC2);
};

void x_dir_left(void){
	pio_clear(PIOB, PIO_PB26);
	pio_set(PIOA, PIO_PA14);
};

void x_dir_right(void){
	pio_set(PIOB, PIO_PB26);
	pio_clear(PIOA, PIO_PA14);
};

void x_step(void){
	while(lock){};
	lock = 1;
	pio_clear(PIOA, PIO_PA15);
	pio_set(PIOD, PIO_PD0);
	delay_1us();
	pio_set(PIOA, PIO_PA15);
	pio_clear(PIOD, PIO_PD0);
};

void y_dir_left(void){
	pio_set(PIOD, PIO_PD1);
	pio_clear(PIOD, PIO_PD2);
};


void y_dir_right(void){
	pio_clear(PIOD, PIO_PD1);
	pio_set(PIOD, PIO_PD2);
};

void y_step(void){
	while(lock){};
	lock = 1;
	pio_clear(PIOD, PIO_PD3);
	pio_set(PIOD, PIO_PD6);
	delay_1us();
	pio_set(PIOD, PIO_PD3);
	pio_clear(PIOD, PIO_PD6);
};

