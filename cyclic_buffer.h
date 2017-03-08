#ifndef cyclic_buffer_h__
#define cyclic_buffer_h__

#include <asf.h>
#include <serial.h>

#define BUFFER_SIZE	128

typedef struct {
	uint8_t command;
	uint8_t msb;
	uint8_t lsb;
}Frame;

Frame buffer[BUFFER_SIZE];

void cb_init(void);
uint8_t cb_receive(uint8_t byte);
uint32_t cb_load(void);
uint32_t cb_insert_frame_in_buffer(void);
void cb_read_frame(Frame* f);


#endif // cyclic_buffer_h__
