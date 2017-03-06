#ifndef motors_h__
#define motors_h__

#include <asf.h>

extern uint8_t volatile lock;

void z_dir_left(void);
void z_dir_right(void);
void z_step(void);

//void x_enable(void);
//void x_disable(void);
void x_dir_left(void);
void x_dir_right(void);
//void x_stop(void);
void x_step(void);

//void y_enable(void);
//void y_disable(void);
void y_dir_left(void);
void y_dir_right(void);
//void y_stop(void);
void y_step(void);

#endif // motors_h__
