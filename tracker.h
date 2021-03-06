#ifndef tracker_h__
#define tracker_h__

#include <asf.h>
#include <motors.h>

extern int32_t volatile delay_ms_irq;

extern uint32_t volatile actual_speed;
extern uint32_t volatile target_speed;

extern uint8_t volatile acl_phase;
extern uint8_t volatile dcl_phase;


//void stop(int16_t delay_ms);
void move(int8_t dx, int8_t dy);
void set_speed(int32_t step_per_second);
void set_speed_acc(int32_t step_per_second);
void line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void homing(void);
void reset_coordinates(void);

#endif // tracker_h__
