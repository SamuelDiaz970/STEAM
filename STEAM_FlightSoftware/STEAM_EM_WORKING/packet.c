/*
 *  packet.c
 *
 *  Created on: Mar 30, 2022
 *      Author: Kanin
 */


// #include "FIFO_accessor.h"
// #include "buffer.h"
#include "packet.h"

#define VER				0
#define TYPE			0
// #define SHDR			
// #define APID			
#define GRP				0
// #define SEQ				
// #define LEN				
#define SCID			0xE
#define FILL1			0
#define FLASH_BLOCK		0
#define FILL2			0
#define TIME_QUALITY	0
#define science_packet_apid				0x2C
#define housekeeping_packet_apid		0x76

int hk_packet_count = 0;
int hr_packet_count = 0;
int sr_packet_count = 0;
// struct tm time = {
// 		.tm_year = 2000,
// 		.tm_mon = 0,
// 		.tm_mday = 0,
// 		.tm_hour = 0,
// 		.tm_min = 0,
// 		.tm_sec = 0
// 	};

// void rtc_start(){
// 	set_rtc_time(time);
// }
// #define USEC,
// #define SEC
/***************************************************************************//**
 * @brief
 *   sets up the packet header to be sent. conforms data into bytes
 *
 * @details
 * 	 
 *
 * @note
 *	 
 * @param[in] variable_name
 *  
 *
 ******************************************************************************/
void packet_head(int buff,
  int SHDR,
  int APID,
  int SEQ,
  int LEN){
	int err = 1;
    err = gettimeofday(&tvv,NULL);
    if(err == -1) return;
	int SEC = tvv.tv_sec;//get_rtc_time(&time);
	int USEC = tvv.tv_usec;
	int count = 0;
	LEN = LEN - 7;
    if(buff == 0){
        count = buffer_write_hk((VER << 5 | TYPE << 4| SHDR << 3 | (APID >> 8)), count);
        count = buffer_write_hk(APID, count);
		count = buffer_write_hk((GRP << 6| (SEQ >> 8)), count);
        count = buffer_write_hk(SEQ, count);
        count = buffer_write_hk(LEN >> 8, count);
        count = buffer_write_hk(LEN, count);
        count = buffer_write_hk(SCID, count);
        count = buffer_write_hk(FILL1 | FLASH_BLOCK << 1, count);
        count = buffer_write_hk(FLASH_BLOCK >> 7, count);
        count = buffer_write_hk(FILL2 << 6 | TIME_QUALITY << 4 | USEC, count);
        count = buffer_write_hk(USEC >> 4, count);
        count = buffer_write_hk(USEC >> 12, count);
        count = buffer_write_hk(SEC, count);
        count = buffer_write_hk(SEC >> 8, count);
        count = buffer_write_hk(SEC >> 16, count);
        count = buffer_write_hk(SEC >> 24, count);
    }
    if(buff == 1){
        count = buffer_write_hr((VER << 5 | TYPE << 4| SHDR << 3 | (APID >> 8)), count);
        count = buffer_write_hr(APID, count);
        count = buffer_write_hr((GRP << 6| (SEQ >> 8)), count);
        count = buffer_write_hr(SEQ, count);
        count = buffer_write_hr(LEN >> 8, count);
        count = buffer_write_hr(LEN, count);
        count = buffer_write_hr(SCID, count);
        count = buffer_write_hr(FILL1 | FLASH_BLOCK << 1, count);
        count = buffer_write_hr(FLASH_BLOCK >> 7, count);
        count = buffer_write_hr(FILL2 << 6 | TIME_QUALITY << 4 | USEC, count);
        count = buffer_write_hr(USEC >> 4, count);
        count = buffer_write_hr(USEC >> 12, count);
        count = buffer_write_hr(SEC, count);
        count = buffer_write_hr(SEC >> 8, count);
        count = buffer_write_hr(SEC >> 16, count);
        count = buffer_write_hr(SEC >> 24, count);
    }
	if(buff == 2){
        count = buffer_write_sr((VER << 5 | TYPE << 4| SHDR << 3 | (APID >> 8)), count);
        count = buffer_write_sr(APID, count);
        count = buffer_write_sr((GRP << 6| (SEQ >> 8)), count);
        count = buffer_write_sr(SEQ, count);
        count = buffer_write_sr(LEN >> 8, count);
        count = buffer_write_sr(LEN, count);
        count = buffer_write_sr(SCID, count);
        count = buffer_write_sr(FILL1 | FLASH_BLOCK << 1, count);
        count = buffer_write_sr(FLASH_BLOCK >> 7, count);
        count = buffer_write_sr(FILL2 << 6 | TIME_QUALITY << 4 | USEC, count);
        count = buffer_write_sr(USEC >> 4, count);
        count = buffer_write_sr(USEC >> 12, count);
        count = buffer_write_sr(SEC, count);
        count = buffer_write_sr(SEC >> 8, count);
        count = buffer_write_sr(SEC >> 16, count);
        count = buffer_write_sr(SEC >> 24, count);
    }
}


/***************************************************************************//**
 * @brief Add a 32 bit value to an 8 bit aligned FIFO (Little Endian)
 *   
 * @param[in] array
 * 	this is a pointer to an array so that you can edit the array in the function
 * 	Most likely passed in array will likely be a packet
 * 
 * @param[in] length
 * 	This is the max length of the packet 
 * 
 * @param[in] location
 * 	This is the current location inside the array specifying where to add the data
 * 
 * @param[in] var
 *  This is the data that will be added to the array/packet at the specified location
 * 
 ******************************************************************************/

int IntToChar(char buff, int length, int var){
    if(buff == 0){
        for(int k = 0; k < 4; k++){
			length = buffer_write_hk(var >> (k*8),length);
			if(length == -1){
				printf("Buffer Full");
				return -1;
			}
		}
    }
    if(buff == 1){
        for(int k = 0; k < 4; k++){
            length = buffer_write_hr(var >> (k*8),length);
			if(length == -1){
				printf("Buffer Full");
				return -1;
			}
        }
    }
	if(buff == 2){
        for(int k = 0; k < 4; k++){
            length = buffer_write_sr(var >> (k*8),length);
			if(length == -1){
				printf("Buffer Full");
				return -1;
			}
        }
    }
	return length;
}


/***************************************************************************//**
 * @brief Add a 16 bit value to an 8 bit aligned buff (Little Endian)
 *   
 * @param[in] array
 * 	this is a pointer to an array so that you can edit the array in the function
 * 	Most likely passed in array will likely be a packet
 * 
 * @param[in] length
 * 	This is the max length of the packet 
 * 
 * @param[in] location
 * 	This is the current location inside the array specifying where to add the data
 * 
 * @param[in] var
 *  This is the data that will be added to the array/packet at the specified location
 * 
 ******************************************************************************/
int ShortToChar(char buff, int length, short var){
	if(buff == 0){
        for(int k = 0; k < 2; k++){
			length = buffer_write_hk(var >> (k*8),length);
			if(length == -1){
				printf("Buffer Full");
				return -1;
			}
		}
    }
    if(buff == 1){
        for(int k = 0; k < 2; k++){
            length = buffer_write_hr(var >> (k*8),length);
			if(length == -1){
				printf("Buffer Full");
				return -1;
			}
        }
    }
	if(buff == 2){
        for(int k = 0; k < 2; k++){
            length = buffer_write_sr(var >> (k*8),length);
			if(length == -1){
				printf("Buffer Full");
				return -1;
			}
        }
    }
	return length;
}

void payload_science_dummy_packet_sr(){
	int buff = 2;
	packet_head(buff, 1, science_packet_apid, sr_packet_count, sr_buff_length -4);
    sr_packet_count++;

    int packet_tail_location = 16;
	packet_tail_location = buffer_write_sr(0x02,packet_tail_location);//X123_Indicator;//HXR(0x1) or SXR (0x2)
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
    for(int i = 0; i < 3105; i++){
		packet_tail_location = buffer_write_sr(0,packet_tail_location);
        if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
		}
	}

    // int uart_array_temp[32] = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
	// for(int i = 0; i < 32; i++){
	// 	buffer_write_sr(uart_array_temp[i];
    //     packet_tail_location++;
	// }
	// char X123_Rd_Wr_Errors = 0;
	// uart_tx[49] = X123_Rd_Wr_Errors;
	// for(int i = 0; i < 3072; i++){
	// 	uart_tx[i+50] = 0;
    //     packet_tail_location++;
	// }

    unsigned short check = checksum_sr();
	packet_tail_location = buffer_write_sr(check ,packet_tail_location);
	packet_tail_location = buffer_write_sr(check>>8,packet_tail_location);
    int syncword = 0x1DFCCF1A;//0x1ACFFC1D;

    packet_tail_location = IntToChar(buff, packet_tail_location, syncword);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
}

/**
 * It takes a spectrometer number, formats data to be sent to punch, and then writes the response to
 * a buffer
 * 
 * @param spectrometer the spectrometer number (1 or 2)
 * 
 * @return The return value is the error code.
 */
int payload_science_packet_sr(int spectrometer){
	int buff = 2;
	packet_head(buff, 1, science_packet_apid, sr_packet_count, sr_buff_length - 4);
    sr_packet_count++;

    int packet_tail_location = 16;
	packet_tail_location = buffer_write_sr(0x02,packet_tail_location);//X123_Indicator;//HXR(0x1) or SXR (0x2)
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return -1;
	}
	struct h_packet pckt;
	unsigned char buffer[rx_buff_length];
	pckt.buf = buffer;
	pckt.size = rx_buff_length;
	int err = tx_rx_spectrum(spectrometer,3, &pckt);
	if(err == -1) return -1;
	for(int i = 3078; i < 3078+24; i++){
		packet_tail_location = buffer_write_sr(pckt.buf[i],packet_tail_location);
	}
	for(int i = (3078+30); i < (3078+35); i++){
		packet_tail_location = buffer_write_sr(pckt.buf[i],packet_tail_location);
	}
	unsigned int k = (pckt.buf[3078+32] & 0b00001111)<< 8 | pckt.buf[3078+33];

	printf("\n\nDetector Temp: %u\n\n\n", k);
	packet_tail_location = buffer_write_sr(pckt.buf[3116],packet_tail_location);
	packet_tail_location = buffer_write_sr(pckt.buf[3113],packet_tail_location);
	packet_tail_location = buffer_write_sr(pckt.buf[3114],packet_tail_location);
	
	
	//UART ERRORS
	packet_tail_location = buffer_write_sr(0,packet_tail_location);
	// packet_tail_location = buffer_write_sr(0,packet_tail_location);

	for(int i = 6; i < 3078; i++){
		packet_tail_location = buffer_write_sr(pckt.buf[i],packet_tail_location);
	}


    // for(int i = 0; i < 3105; i++){
	// 	packet_tail_location = buffer_write_sr(0,packet_tail_location);
    //     if(packet_tail_location == -1){
	// 	printf("Buffer Full");
	// 	return;
	// 	}
	// }

    // int uart_array_temp[32] = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
	// for(int i = 0; i < 32; i++){
	// 	buffer_write_sr(uart_array_temp[i];
    //     packet_tail_location++;
	// }
	// char X123_Rd_Wr_Errors = 0;
	// uart_tx[49] = X123_Rd_Wr_Errors;
	// for(int i = 0; i < 3072; i++){
	// 	uart_tx[i+50] = 0;
    //     packet_tail_location++;
	// }

    unsigned short check = checksum_sr();
	packet_tail_location = buffer_write_sr(check ,packet_tail_location);
	packet_tail_location = buffer_write_sr(check>>8,packet_tail_location);
    int syncword = 0x1DFCCF1A;//0x1ACFFC1D;
    packet_tail_location = IntToChar(buff, packet_tail_location, syncword);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return -1;
	}
	return 0;
}

/**
 * It takes a spectrometer number, formats data to be sent to punch, and then writes the response to
 * a buffer
 * 
 * @param spectrometer the spectrometer number (1 or 2)
 * 
 * @return The return value is the error code.
 */
int payload_science_packet_hr(int spectrometer){
	int buff = 1;
	packet_head(buff,1, science_packet_apid, hr_packet_count, hr_buff_length-4);
    hr_packet_count++;

    int packet_tail_location = 16;
	packet_tail_location = buffer_write_hr(0x01,packet_tail_location);//X123_Indicator;//HXR(0x1) or SXR (0x2)
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return -1;
	}
	
	struct h_packet pckt;
	unsigned char buffer[rx_buff_length];
	pckt.buf = buffer;
	pckt.size = rx_buff_length;
	int err = tx_rx_spectrum(spectrometer,3, &pckt);
	if(err == -1) return -1;
	for(int i = 3078; i < 3078+24; i++){
		packet_tail_location = buffer_write_hr(pckt.buf[i],packet_tail_location);
	}
	for(int i = (3078+30); i < (3078+35); i++){
		packet_tail_location = buffer_write_hr(pckt.buf[i],packet_tail_location);
	}
	int k = (pckt.buf[3078+32] & 0b00001111)<< 8 | pckt.buf[3078+33];
	printf("\n\nDetector Temp: %d\n\n\n", k);
	packet_tail_location = buffer_write_hr(pckt.buf[3116],packet_tail_location);
	packet_tail_location = buffer_write_hr(pckt.buf[3113],packet_tail_location);
	packet_tail_location = buffer_write_hr(pckt.buf[3114],packet_tail_location);
	
	
	//UART ERRORS
	packet_tail_location = buffer_write_hr(0,packet_tail_location);
	// packet_tail_location = buffer_write_sr(0,packet_tail_location);

	for(int i = 6; i < 3078; i++){
		packet_tail_location = buffer_write_hr(pckt.buf[i],packet_tail_location);
	}

    // for(int i = 0; i < 3105; i++){
	// 	packet_tail_location = buffer_write_hr(0,packet_tail_location);
    //     if(packet_tail_location == -1){
	// 	printf("Buffer Full");
	// 	return;
	// 	}
	// }

    // int uart_array_temp[32] = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
	// for(int i = 0; i < 32; i++){
	// 	buffer_write_hr(uart_array_temp[i];
    //     packet_tail_location++;
	// }
	// char X123_Rd_Wr_Errors = 0;
	// uart_tx[49] = X123_Rd_Wr_Errors;
	// for(int i = 0; i < 3072; i++){
	// 	uart_tx[i+50] = 0;
    //     packet_tail_location++;
	// }

    unsigned short check = checksum_hr();
	packet_tail_location = buffer_write_hr(check ,packet_tail_location);
	packet_tail_location = buffer_write_hr(check>>8,packet_tail_location);
    int syncword = 0x1DFCCF1A;//0x1ACFFC1D;
    packet_tail_location = IntToChar(buff, packet_tail_location, syncword);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return -1;
	}
	return 0; 
}


void payload_science_dummy_packet_hr(){
	int buff = 1;
	packet_head(buff,1, science_packet_apid, hr_packet_count, hr_buff_length-4);
    hr_packet_count++;

    int packet_tail_location = 16;
	packet_tail_location = buffer_write_hr(0x01,packet_tail_location);//X123_Indicator;//HXR(0x1) or SXR (0x2)
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
    for(int i = 0; i < 3105; i++){
		packet_tail_location = buffer_write_hr(0,packet_tail_location);
        if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
		}
	}

    // int uart_array_temp[32] = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
	// for(int i = 0; i < 32; i++){
	// 	buffer_write_hr(uart_array_temp[i];
    //     packet_tail_location++;
	// }
	// char X123_Rd_Wr_Errors = 0;
	// uart_tx[49] = X123_Rd_Wr_Errors;
	// for(int i = 0; i < 3072; i++){
	// 	uart_tx[i+50] = 0;
    //     packet_tail_location++;
	// }

    // checksum_hr();
	unsigned short check = checksum_hr();
	packet_tail_location = buffer_write_hr(check ,packet_tail_location);
	packet_tail_location = buffer_write_hr(check>>8,packet_tail_location);
	// packet_tail_location = packet_tail_location + 2;
    int syncword = 0x1DFCCF1A;//0x1ACFFC1D;
    packet_tail_location = IntToChar(buff, packet_tail_location, syncword);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
}


/**
 *  Formats housekeeping dummy data to be sent to punch, and then writes the response to
 * a buffer
 *  
 * @return The return value is the error code.
 */
void payload_HK_dummy_packet(){
	int buff = 0;
	packet_head(buff,1, housekeeping_packet_apid, hk_packet_count, hk_buff_length - 4);
    hk_packet_count++;

	int packet_tail_location = 16;
	/*
	MSN: CDH State Bitfield
	0x0- Nominal Science
	0x1- Reduced Science
	0x2- Safe Mode
	*/
	char CDH_Info = 0;
	packet_tail_location = buffer_write_hk(CDH_Info,packet_tail_location);
    // uart_tx[packet_tail_location] = CDH_Info;
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}

	//setup as global variable to remeber last opcode
	char CMD_Last_OpCode = get_last_opcode();
    packet_tail_location = buffer_write_hk(CMD_Last_OpCode,packet_tail_location);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}

	/*
	0=CMD_SUCCESS
	1=CMD_FAIL_LENGTH
	2=CMD_FAIL_APID
	3=CMD_FAIL_OPCODE
	4=CMD_FAIL_CHECKSUM
	5=CMD_FAIL_SAFE_MODE
	6=CMD_FAIL_DUAL_CMD
	7=CMD_FAIL_PARAMETER
	8=CMD_FAIL_STATE
	9=CMD_FAIL
	*/
	char CMD_Last_Status = 0;
    packet_tail_location = buffer_write_hk(CMD_Last_Status,packet_tail_location);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}

	//make global command counters
	short CMD_Accept_Count = get_cmd_accept();
	packet_tail_location = ShortToChar(buff, packet_tail_location, CMD_Accept_Count);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	//make global command counters
	short CMD_Reject_Count = get_cmd_reject();
	packet_tail_location = ShortToChar(buff, packet_tail_location, CMD_Reject_Count);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}

	/*
	MSB - little-endian

	Major, Minor Masks:
	0xF0 - Major
	0x0F - Minor
	*/
	char FSW_Major_Minor = 0;	
	packet_tail_location = buffer_write_hk(FSW_Major_Minor,packet_tail_location);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}

	/*
	MSB - little-endian

	0x-F: PATCH Version
	0x3-: Flight Model
	0x4-: ADCS Gain Detuned
	0x8-: ADCS Using SPS
	*/
	char FSW_Patch_Version = 0;
	packet_tail_location = buffer_write_hk(FSW_Patch_Version,packet_tail_location);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short ContactTx_TimeoutCounter = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, ContactTx_TimeoutCounter);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}

	int TIME_Offset = 0;
	packet_tail_location = IntToChar(buff, packet_tail_location, TIME_Offset);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Xiphos_V = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Xiphos_V);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short HXR_5V = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, HXR_5V);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short SXR_5V = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, SXR_5V);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Xiphos_Power = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Xiphos_Power);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Xiphos_Die_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Xiphos_Die_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Xiphos_Board_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Xiphos_Board_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Xiphos_Current = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Xiphos_Current);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short STEAM_Current = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, STEAM_Current);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short HXR_Current = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, HXR_Current);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short SXR_Current = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, SXR_Current);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Thermistor1_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Thermistor1_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Thermistor2_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Thermistor2_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Thermistor3_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Thermistor3_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Thermistor4_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Thermistor4_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Thermistor5_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Thermistor5_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Thermistor6_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Thermistor6_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short Thermistor7_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, Thermistor7_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short CDH_Enables = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, CDH_Enables);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short CDH_I2C_Err = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, CDH_I2C_Err);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short CDH_UART0_Err = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, CDH_UART0_Err);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short CDH_UART1_Err = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, CDH_UART1_Err);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short CDH_UART2_Err = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, CDH_UART2_Err);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	char SXR_Brd_Temp = 0;
	packet_tail_location = buffer_write_hk(SXR_Brd_Temp,packet_tail_location);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short SXR_Fast_Count_Norm = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, SXR_Fast_Count_Norm);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short SXR_Slow_Count_Norm = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, SXR_Slow_Count_Norm);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short SXR_Det_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, SXR_Det_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	char HXR_Brd_Temp = 0;
	packet_tail_location = buffer_write_hk(HXR_Brd_Temp,packet_tail_location);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}

	short HXR_Fast_Count_Norm = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, HXR_Fast_Count_Norm);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short HXR_Slow_Count_Norm = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, HXR_Slow_Count_Norm);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	short HXR_Det_Temp = 0;
	packet_tail_location = ShortToChar(buff, packet_tail_location, HXR_Det_Temp);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	//1288 bits long (161 bytes long zero filled to hey to 254 byte packet length(not including syncword))
	//Spare
	for(int i = 0; i < 161; i++){
		packet_tail_location = buffer_write_hk(0,packet_tail_location);//double check start byte is correct
        if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	}	

    unsigned short check = checksum_hk();
	packet_tail_location = buffer_write_hk(check ,packet_tail_location);
	packet_tail_location = buffer_write_hk(check>>8,packet_tail_location);

    // packet_tail_location = packet_tail_location + 2;
    int syncword = 0x1DFCCF1A;//0x1ACFFC1D;
    packet_tail_location = IntToChar(buff, packet_tail_location, syncword);
	if(packet_tail_location == -1){
		printf("Buffer Full");
		return;
	}
	
	// short checkbytes = checksum(uart_tx, 256, 250);
	// uart_tx[250] = checkbytes;
	// uart_tx[251] = checkbytes >> 8;//verify MSB or LSB first or Checksum output TBD
	// int syncword = 0x1ACFFC1D;
	// uart_tx[252] = 0x1A;// Verify MSB/LSB first
	// uart_tx[253] = 0xCF;
	// uart_tx[254] = 0xFC;
	// uart_tx[255] = 0x1D;

}
