#ifndef in_out_h__
#define in_out_h__

#include <asf.h>
#include <pinout.h>

typedef struct {
	uint32_t pio_a_state;
	uint32_t pio_b_state;
	uint32_t pio_c_state;
	uint32_t pio_d_state;
}TS_IO_State;

extern TS_IO_State io_state;

typedef void (*IO_callback)(TS_IO_State* io_state);

void io_init(IO_callback io_callback);
void io_drive(TS_IO_State* io_state);

uint32_t get_pio_a(void);
uint32_t get_pio_b(void);
uint32_t get_pio_c(void);
uint32_t get_pio_d(void);

#endif // in_out_h__