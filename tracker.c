#include <tracker.h>

int32_t volatile abs_pos_x = 0;
int32_t volatile abs_pos_y = 0;

uint32_t volatile step_time;
int32_t volatile delay_ms_irq;

uint32_t volatile accelerate = 300;
uint32_t volatile decelerate = 300;
uint32_t volatile actual_speed = 1111;
uint32_t volatile target_speed = 1111;

uint32_t volatile accel = 100;

uint8_t volatile acl_phase = 1;
uint8_t volatile dcl_phase = 0;

void move(int8_t dx, int8_t dy){
	line(abs_pos_x, abs_pos_y, abs_pos_x+dx, abs_pos_y+dy);
};

void set_speed(int16_t step_per_second){
	step_time = 42000000.0 / (step_per_second*2);
};

void set_speed_acc(int16_t step_per_second)
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