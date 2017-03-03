#include <thc.h>

uint8_t volatile thcIsEnable = 0;

uint32_t volatile adc_read_tab[10];
uint8_t volatile adc_tab_prt = 0;

uint32_t volatile adc_set = 0;
uint32_t volatile adc_read = 0;
int32_t volatile adc_error = 0;

#define MAX_ADC_READ	4096.0
#define MAX_MESURED_VAL	200.0

// 100V/mm, 2mm max. 
const double heightToAdc_factor = 2048.0;
const double voltageToAdc_factor = MAX_ADC_READ / MAX_MESURED_VAL;

TS_PID pid_adc;

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
		
		if( thcIsEnable ){
			
			uint32_t sum = 0;
			for( uint8_t i=0; i<10; i++ ){
				sum += adc_read_tab[i];
			};
			sum /= 10;
			
			uint32_t act_voltage = sum;
			
			if( act_voltage > adc_set ){
				z_dir_left();
				}else{
				z_dir_right();
			};
			
			pid_set_mesure(&pid_adc, act_voltage);
			pid_compute(&pid_adc);
			
			adc_error = abs( pid_get_output(&pid_adc) );
			
			if(adc_error > 2){
				REG_TC1_CCR0 = 5;
				uint32_t tc_rc = 42000000/ (adc_error * 10);
				if(tc_rc <= 3000 ){
					tc_rc = 3000;
				};
				REG_TC1_RC0 = tc_rc;
			}else{
				REG_TC1_CCR0 = 2;
			};
		};
	};
};

void thc_enable(void)
{
	thcIsEnable = 1;
};

void thc_disable(void)
{
	thcIsEnable = 0;
};

void thc_set_height_mm(float height)
{
	adc_set = height * heightToAdc_factor;
	pid_adc.setpoint = adc_set;
};

void thc_set_height_v(float voltage)
{
	adc_set = voltage * voltageToAdc_factor;
	pid_adc.setpoint = adc_set;
};

void thc_set_pid(double dt, double kp, double ki, double kd)
{
	pid_adc.dt = dt;
	pid_adc.kp	= kp;
	pid_adc.ki = ki;
	pid_adc.kd = kd;
};
