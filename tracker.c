#include <tracker.h>

int32_t volatile abs_pos_x = 0;
int32_t volatile abs_pos_y = 0;

uint32_t volatile step_time;
int32_t volatile delay_ms_irq;

uint32_t volatile actual_speed = 1000;
uint32_t volatile target_speed = 1000;

const uint32_t volatile accel = 2500;

uint8_t volatile acl_phase = 1;
uint8_t volatile dcl_phase = 0;

void delay_tracker(uint32_t volatile t){
	while(t--){};
};

void move(int8_t dx, int8_t dy){
	line(abs_pos_x, abs_pos_y, abs_pos_x+dx, abs_pos_y+dy);
};

void set_speed(int32_t step_per_second){
	step_time = 42000000.0 / (step_per_second*2);
};

void set_speed_acc(int32_t step_per_second)
{
	target_speed = step_per_second;
	if( actual_speed < target_speed ){
		acl_phase = 1;
	};
	if( actual_speed > target_speed ){
		dcl_phase = 1;
	};
};

void step(int32_t x, int32_t y){
	int32_t dx = abs_pos_x - x;
	int32_t dy = abs_pos_y - y;
	
	if( dx > 0 ){
		x_dir_left();
		x_step();
		abs_pos_x--;
	}else{
		x_dir_right();
		x_step();
		abs_pos_x++;
	};
	
	if( dy > 0 ){
		y_dir_left();
		y_step();
		abs_pos_y--;
	}else{
		y_dir_right();
		y_step();
		abs_pos_y++;
	};
};

void line(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	if( (x1 < 0)||(y1<0)||(x2<0)||(y2<0) )return;
	
	int32_t d, dx, dy, ai, bi, xi, yi;
	int32_t x = x1;
	int32_t y = y1;
	
	if(x1 < x2){
		xi = 1;
		dx = x2 - x1;
	}else{
		xi = -1;
		dx = x1 - x2;
	};
	
	if(y1 < y2){
		yi = 1;
		dy = y2 - y1;
	}else{
		yi = -1;
		dy = y1 - y2;
	};
	
	if(dx > dy){
		ai = (dy - dx) * 2;
		bi = dy * 2;
		d = bi - dx;
		while(x != x2){
			if(d >= 0){
				x += xi;
				y += yi;
				d += ai;
			}else{
				d += bi;
				x += xi;
			};
			step(x, y);
		};
	}else{
		ai = ( dx - dy ) * 2;
		bi = dx * 2;
		d = bi - dy;
		while(y != y2){
			if(d >= 0){
				x += xi;
				y += yi;
				d += ai;
			}else{
				d += bi;
				y += yi;
			};
			step(x, y);
		};
	};
};

void homing(void)
{
	uint8_t limit_switch_x = 1;
	uint8_t limit_switch_y = 1;
	uint8_t limit_switch_z = 1;
	
	set_speed_acc(2);
	line(0,0,1,1);
	line(1,1,0,0);
	set_speed_acc(9000);
	
	uint32_t dx = 0;
	do{
		line(dx,0,dx+1,0);
		dx += 1;
		limit_switch_x = pio_get(PIOC, PIO_TYPE_PIO_INPUT, PIO_PC16);
	}while(limit_switch_x);
	
	set_speed_acc(2);
	delay_tracker(0xFFFFF);
	
	set_speed_acc(2000);
	
	uint32_t dy = 0;
	do{
		line(dx,dy,dx,dy+1);
		dy += 1;
		limit_switch_y = pio_get(PIOC, PIO_TYPE_PIO_INPUT, PIO_PC16);
	}while(limit_switch_y);
	
	delay_tracker(0xFFFFF);
	
	reset_coordinates();
}

void reset_coordinates(void)
{
	abs_pos_y = 0;
	abs_pos_x = 0;
	line(0,0,1,1);
	line(1,1,0,0);
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

// motors speed
ISR( TC0_Handler ){
	uint32_t volatile tc0_sr0 = REG_TC0_SR0;
	UNUSED(tc0_sr0);
	if(lock == 0){
		// blue screen error
		REG_TC0_RC0 = 420000;
	}else{
		lock = 0;
		REG_TC0_RC0 = step_time;
	};
	
};