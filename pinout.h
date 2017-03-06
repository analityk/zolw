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


// outputs masks
#define PIO_OUT_A_PIN_MASK	((PIO_PA14)|(PIO_PA15))
#define PIO_OUT_B_PIN_MASK	((PIO_PB26)|(PIO_PB27))
#define PIO_OUT_C_PIN_MASK	((PIO_PC1)|(PIO_PC2)|(PIO_PC3))
#define PIO_OUT_D_PIN_MASK	((PIO_PD0)|(PIO_PD1)|(PIO_PD2)|(PIO_PD3)|(PIO_PD6)|PIO_PD10)

// inputs masks (all active low with pull up)
#define PIO_IN_A_PIN_MASK	((0))
#define PIO_IN_B_PIN_MASK	((0))
#define PIO_IN_C_PIN_MASK	((PIO_PC16))
#define PIO_IN_D_PIN_MASK	((0))

#endif // pinout_h__
