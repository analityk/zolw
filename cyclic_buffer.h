#ifndef cyclic_buffer_h__
#define cyclic_buffer_h__

#include <asf.h>

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


//class Bufor{
	//private:
	//uint8_t volatile rec_cnt;
	//uint8_t rec_tab[3];
	//Frame frame;
	//uint8_t write_prt;
	//uint8_t read_prt;
	//uint8_t cnts;
	//
	//public:
//
	
//
	//Bufor():
	//rec_cnt(0),
	//write_prt(0),
	//read_prt(0),
	//cnts(0)
	//{};
	//
	//bool full(void);
	//uint8_t receive(uint8_t byte);
	//uint8_t insert_frame_in_buffer(void);
	//void read_frame_from_buffer(Frame* frame);
//};
//
//bool Bufor::full(void)
//{
	//if( cnts >= 8 )return true;
	//return false;
//};
//
//
//uint8_t Bufor::receive(uint8_t byte)
//{
	//uint8_t result = 255;
	//
	//if( rec_cnt < 2 ){
		//rec_tab[rec_cnt] = byte;
		//rec_cnt++;
		//}else{
		//rec_cnt = 0;
		//frame.command = rec_tab[0];
		//frame.msb = rec_tab[1];
		//frame.lsb = byte;
		//
		//result = insert_frame_in_buffer();
	//};
	//
	//return result;
//};
//
//uint8_t Bufor::insert_frame_in_buffer(void){
	//if( cnts >= BUFFER_SIZE ){
		//return 0;
	//}else{
		//cnts++;
		//write_prt++;
		//buffer[ write_prt % BUFFER_SIZE ] = frame;
	//};
	//return cnts;
//};
//
//void Bufor::read_frame_from_buffer(Frame* frame)
//{
	//if( cnts > 0 ){
		//read_prt++;
		//*frame = buffer[ read_prt % BUFFER_SIZE ];
		//cnts--;
	//};
//};
//
//Bufor bufor;


#endif // cyclic_buffer_h__
