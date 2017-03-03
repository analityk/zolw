#ifndef thc_h__
#define thc_h__

#include <asf.h>
#include <pid.h>
#include <pinout.h>
#include <tracker.h>
#include <motors.h>

void thc_enable(void);
void thc_disable(void);

void thc_set_height_mm(float height);
void thc_set_height_v(float voltage);

void thc_homing(void);

void thc_set_pid(double dt, double kp, double ki, double kd);

#endif // thc_h__
