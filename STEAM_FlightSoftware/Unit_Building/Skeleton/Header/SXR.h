// insert here


typedef struct{
	LEUART_TypeDef *leuart;
	leuart_defined_states state;
	uint32_t count;
	//uint32_t DEVADDR;
	//uint32_t REG_ADDR;
	char output[30];
	//uint32_t * data;
	//bool read_write;
	volatile bool sm_busy;
	uint32_t length;
	uint32_t event_CB;
}X123_status;

char X123_Data_First[3072];//char is 1 byte length in memory


int* get_x123_status();

void get_x123_packet();

int byte_to_int();

