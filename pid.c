#include <pid.h>

void pid_set_mesure(TS_PID* pid, double mes_val)
{
	pid->measure_val = mes_val;
};

void pid_compute(TS_PID* pid)
{
	pid->error = pid->setpoint - pid->measure_val;
	pid->integral = pid->integral + (pid->error * pid->dt);
	pid->derivative = (pid->error - pid->prev_error) / pid->dt;
	pid->output = (pid->kp * pid->error) + (pid->ki * pid->integral) + (pid->kd * pid->derivative);
	pid->prev_error = pid->error;
};

void pid_set_setpoint(TS_PID* pid, double setpoint)
{
	pid->setpoint = setpoint;
};

void pid_set_param(TS_PID* pid, double kp, double ki, double kd)
{
	pid->kp = kp;
	pid->ki	= ki;
	pid->kd = kd;
};

double pid_get_output(TS_PID* pid)
{
	return pid->output;
};

void pid_set_kp(TS_PID* pid, double kp)
{
	pid->kp = kp;
;}

void pid_set_ki(TS_PID* pid, double ki)
{
	pid->ki = ki;
};

void pid_set_kd(TS_PID* pid, double kd)
{
	pid->kd = kd;
};

void pid_set_dt(TS_PID* pid, double dt)
{
	pid->dt = dt;
};


