#ifndef pinout_h__
#define pinout_h__

#include <asf.h>

#define Z_DIR_H		PIO_PD10
#define Z_DIR_L		PIO_PC1
#define Z_STEP_H	PIO_PC3
#define Z_STEP_L	PIO_PC2

#define X_DIR_H		PIO_PB26
#define X_DIR_L		PIO_PA14
#define X_STEP_H	PIO_PA15
#define X_STEP_L	PIO_PD0

#define Y_DIR_H		PIO_PD1
#define Y_DIR_L		PIO_PD2
#define Y_STEP_H	PIO_PD3
#define Y_STEP_L	PIO_PD6

#endif // pinout_h__
