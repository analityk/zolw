#ifndef pid_h__
#define pid_h__

#include <asf.h>

typedef struct {
	double error;
	double derivative;
	double prev_error;
	double integral;
	double setpoint;
	double measure_val;
	double output;
	double dt;
	double kp;
	double ki;
	double kd;
}TS_PID;

void pid_set_mesure(TS_PID* pid, double mes_val);
void pid_compute(TS_PID* pid);
void pid_set_setpoint(TS_PID* pid, double setpoint);
void pid_set_param(TS_PID* pid, double kp, double ki, double kd);
double pid_get_output(TS_PID* pid);
void pid_set_kp(TS_PID* pid, double kp);
void pid_set_ki(TS_PID* pid, double ki);
void pid_set_kd(TS_PID* pid, double kd);
void pid_set_dt(TS_PID* pid, double dt);

#endif // pid_h__
