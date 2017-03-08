#include <cyclic_buffer.h>

ISR( UART_Handler ){
	uint32_t volatile status = REG_UART_SR;
	UNUSED(status);
	uint8_t volatile data = REG_UART_RHR;
	REG_UART_THR = cb_receive(data);
};

typedef struct {
	uint8_t volatile rec_cnt;
	uint8_t rec_tab[3];
	Frame frame;
	uint8_t write_prt;
	uint8_t read_prt;
	uint8_t cnts;
}TS_cb_ctrl;

TS_cb_ctrl _cbs;

void cb_init(void)
{
	_cbs.cnts = 0;
	_cbs.frame.command = 0;
	_cbs.frame.lsb = 0;
	_cbs.frame.msb = 0;
	_cbs.read_prt = 0;
	_cbs.rec_cnt = 0;
	_cbs.write_prt = 0;
};

uint32_t cb_load(void)
{
	return _cbs.cnts;
};

uint8_t cb_receive(uint8_t byte){
	uint8_t result = 255;
	if( _cbs.rec_cnt < 2 ){
		_cbs.rec_tab[_cbs.rec_cnt] = byte;
		_cbs.rec_cnt++;
	}else{
		_cbs.rec_cnt = 0;
		_cbs.frame.command = _cbs.rec_tab[0];
		_cbs.frame.msb = _cbs.rec_tab[1];
		_cbs.frame.lsb = byte;

		result = cb_insert_frame_in_buffer();
	};

	return result;
};

uint32_t cb_insert_frame_in_buffer(void)
{
	if( _cbs.cnts >= BUFFER_SIZE ){
		return 0;
	}else{
		_cbs.cnts++;
		_cbs.write_prt++;
		buffer[ _cbs.write_prt % BUFFER_SIZE ] = _cbs.frame;
	};
	return _cbs.cnts;
};

void cb_read_frame(Frame* f){
	if( _cbs.cnts > 0 ){
		_cbs.read_prt++;
		*f = buffer[ _cbs.read_prt % BUFFER_SIZE ];
		_cbs.cnts--;
	};
};
