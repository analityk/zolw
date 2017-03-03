#ifndef tracker_h__
#define tracker_h__

#include <asf.h>
#include <motors.h>

extern uint32_t volatile step_time; 
extern int32_t volatile delay_ms_irq;

extern uint32_t volatile accelerate;
extern uint32_t volatile decelerate;
extern uint32_t volatile actual_speed;
extern uint32_t volatile target_speed;

extern uint32_t volatile accel;

extern uint8_t volatile acl_phase;
extern uint8_t volatile dcl_phase;


void stop(int16_t delay_ms);
void move(int8_t dx, int8_t dy);
void set_speed(int16_t step_per_second);
void set_speed_acc(int16_t step_per_second);
void line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
//uint8_t bazowanie(void);

#endif // tracker_h__
