#include <libpa3.h>//unsure if this is correct
#include "SXR.h"


struct payload_echo{
	VER // 3 bit
	TYPE // 1 bit
	SHDR 
	APID
	GRP
	SEQ
	LEN
	SCID
	FILL1
	FLASH_BLOCK
	FILL2
	TIME_QUALITY
	USEC
	SEC
	CDH_Info
	CMD_Last_OpCode
	CMD_Last_Status
	CMD_Accept_Count
	CMD_Reject_Count
	CMD_Params
	FILL3
	uint16_t checkbytes,
	uint16_t syncword
}
struct payload_HK{
	VER
	TYPE
	SHDR
	APID
	GRP
	SEQ
	LEN
	SCID
	FILL1
	FLASH_BLOCK
	FILL2
	TIME_QUALITY
	USEC
	SEC
	CDH_Info
	CMD_Last_OpCode
	CMD_Last_Status
	CMD_Accept_Count
	CMD_Reject_Count
	FSW_Major_Minor
	FSW_Patch_Version
	ContactTx_TimeoutCounter
	TIME_Offset
	Xiphos_V
	HXR_5V
	SXR_5V
	Xiphos_Power
	Xiphos_Die_Temp
	Xiphos_Board_Temp
	Xiphos_Current
	STEAM_Current
	HXR_Current
	SXR_Current
	Thermistor1_Temp
	Thermistor2_Temp
	Thermistor3_Temp
	Thermistor4_Temp
	Thermistor5_Temp
	Thermistor6_Temp
	Thermistor7_Temp
	CDH_Enables
	CDH_I2C_Err
	CDH_UART0_Err
	CDH_UART1_Err
	CDH_UART2_Err
	SXR_Brd_Temp
	SXR_Fast_Count_Norm
	SXR_Slow_Count_Norm
	SXR_Det_Temp
	HXR_Brd_Temp
	HXR_Fast_Count_Norm
	HXR_Slow_Count_Norm
	HXR_Det_Temp
	Spare
	uint16_t checkbytes,
	uint16_t syncword
}
struct payload_dump{
	VER
	TYPE
	SHDR
	APID
	GRP
	SEQ
	LEN
	SCID
	FILL1
	FLASH_BLOCK
	FILL2
	TIME_QUALITY
	USEC
	SEC
	CDH_Info
	Dump_Data
	Fill3
	uint16_t checkbytes,
	uint16_t syncword
}
struct payload_log_msg{
	int VER = 0x2;
	TYPE = 1;
	SHDR
	APID
	GRP
	SEQ
	LEN
	SCID
	FILL1
	FLASH_BLOCK
	FILL2
	TIME_QUALITY
	USEC
	SEC
	CDH_Info
	Message
	uint16_t checkbytes,
	uint16_t syncword
}
struct payload_science{
	VER[3] = 0b1 >> 3
	TYPE
	SHDR
	APID
	GRP
	SEQ
	LEN
	SCID
	FILL1
	FLASH_BLOCK
	FILL2
	TIME_QUALITY
	USEC
	SEC
	X123_Indicator
	X123_Fast_Count
	X123_Slow_Count
	X123_GP_Count
	X123_Accum_Time
	X123_Live_Time
	X123_Real_Time
	X123_HV
	X123_Det_Temp
	X123_Brd_Temp
	X123_Flags
	X123_Rd_Wr_Errors
	uint8_t X123_Data_First[3072],
	uint16_t checkbytes,
	uint16_t syncword
}





Static int scheduled_event;
Volatile int mode; // initialize to safe mode
/*
 * 0 for Nominal Power Mode
 * 1 for Reduced Power Mode
 * 2 for Safe Mode
 */

main()
{
	mode = 2;
	Initialize HMS
	Initialize timer for HMS check (set period to 3 sec)
	Enable interrupts for RS-422, timer

	mode = 0; // nominal power mode

	sxr_open();
	While (MOSFET 1 current > 0.9);

	hxr_open();
	While (MOSFET 2 current > 0.9);


	int counter = 0;
	while(1){
		HMS_collect();
		if(counter == 3){
			payload_HK_send();
			counter = 0;
		}
		counter++;
	}

}
/*************************
 * Example scheduler loop
 *************************/
// while (1) {
// //	  EMU_EnterEM1();
// 	  //EMU_EnterEM2(true);

// 	  CORE_DECLARE_IRQ_STATE;
// 	  CORE_ENTER_CRITICAL();

// 	  if (!get_scheduled_events()) enter_sleep();

// 	  CORE_EXIT_CRITICAL();

// 	  if(get_scheduled_events() & LETIMER0_UF_CB){
// 		  scheduled_letimer0_uf_cb();
// 	  }
// 	  if(get_scheduled_events() & LETIMER0_COMP0_CB){
// 		  scheduled_letimer0_comp0_cb();
// 	  }
// 	  if(get_scheduled_events() & LETIMER0_COMP1_CB){
// 		  scheduled_letimer0_comp1_cb();
// 	  }
// 	  if(get_scheduled_events() & SI7021_READ_CB){
// 		  scheduled_si7021tempdone();
// 	  }
// 	  if(get_scheduled_events() & SI7021_RH_READ_CB){
// 		  scheduled_si7021RHdone();
// 	  }
// 	  if(get_scheduled_events() & BOOT_UP_CB){
// 		  scheduled_boot_up_cb();
// 	  }
// 	  if(get_scheduled_events() & BLE_TX_DONE_CB){
// 		  scheduled_ble_tx_done_cb();
//   	  }
// 	  if(get_scheduled_events() & VEML6030_READ_CB){
// 		  scheduled_veml6030done();
// 	  }
//   }




/*************************
 * Interrupt handlers
 *************************/
/***************************************************************************//**
 * @brief
 *   
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
timer_irq_handler(){
	Measurments(10 second Interrupt)
}
/***************************************************************************//**
 * @brief
 *   Interrupt to indicate a communication from the Spacecraft
 *
 * @details
 * 	 This would present itself in the form of a packet with a command to be done
 * 		adds the command to be processed in the scheduler
 *
 * @note
 *	 
 * @param[in] variable_name
 *  
 *
 ******************************************************************************/
rs422_irq_handler(){
	recieve message
	Decode message
	process command

	do either
		Command then do
		payload_echo_send();async (after a command)(more confirmation)

		payload_parameter_dump_send();async (on request)(data)
}
/***************************************************************************//**
 * @brief
 *   Interrupt to indicate a communication with the spectrometers
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
rs232_irq_handler(){
	talk to spectrometers
}
/***************************************************************************//**
 * @brief
 *   Any error that occurs would prompt this interrecpt to send an
 *   error packet to the spacecraft
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
error_irq_handler(){

}


/*************************
 * Packet handlers
 *************************/
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
packet_head(unsigned int VER,
  unsigned int TYPE,
  unsigned int SHDR,
  unsigned int APID,
  unsigned int GRP,
  unsigned int SEQ,
  unsigned int LEN,
  unsigned int SCID,
  unsigned int FILL1,
  unsigned int FLASH_BLOCK,
  unsigned int FILL2,
  unsigned int TIME_QUALITY,
  unsigned int USEC,
  unsigned int SEC){
	// char uart_tx[36];
	// int temp1 = 0;
	// int temp2 = 0;
	// int temp3 = 0;
	// int temp4 = 0;
	
	
	// temp1 = VER | TYPE << 3| SHDR << 4 | APID << 5;
	// temp2 = APID >> 3;
	// temp3 = GRP | SEQ << 2;
	// temp4 = SEQ >> 6;
	// uart_tx[0] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	// temp1 = 0;
	// temp2 = 0;
	// temp3 = 0;
	// temp4 = 0;
	// temp1 = LEN;
	// temp2 = LEN >> 8;
	// temp3 = SCID;
	// temp4 = FILL1 | FLASH_BLOCK << 1;
	// uart_tx[1] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	// temp1 = 0;
	// temp2 = 0;
	// temp3 = 0;
	// temp4 = 0;
	// temp1 = FLASH_BLOCK >> 7;
	// temp2 = FILL2 | TIME_QUALITY << 2 | USEC << 4;
	// temp3 = USEC >> 4;
	// temp4 = USEC >> 12;
	// uart_tx[2] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	// temp1 = 0;
	// temp2 = 0;
	// temp3 = 0;
	// temp4 = 0;
	// uart_tx[3] = SEC;

	
	uint8_t uart_tx[36];
	uint8_t temp = 0b00000000;
	temp = VER | TYPE << 3| SHDR << 4 | APID << 5;
	uart_tx[0] = temp;
	temp = 0;
	temp = APID >> 3;
	uart_tx[1] = temp; 
	temp = 0;
	temp = GRP | SEQ << 2;
	uart_tx[2] = temp;
	temp = 0;
	temp = SEQ >> 6;
	uart_tx[3] = temp;
	uart_tx[4] = LEN;
	uart_tx[5] = LEN >> 8;
	uart_tx[6] = SCID;
	temp = 0;
	temp = FILL1 | FLASH_BLOCK << 1;
	uart_tx[7] = temp;
	temp = 0;
	temp = FLASH_BLOCK >> 7;
	uart_tx[8] = temp;
	temp = 0;
	temp = FILL2 | TIME_QUALITY << 2 | USEC << 4;
	uart_tx[9] = temp;
	temp = 0;
	temp = USEC >> 4;
	uart_tx[10] = temp;
	temp = 0;
	temp = USEC >> 12;
	uart_tx[11] = temp;
	temp = 0;
	temp = SEC;
	uart_tx[12] = temp;
	temp = 0;
	temp = SEC >> 8;
	uart_tx[13] = temp;
	temp = 0;
	temp = SEC >> 16;
	uart_tx[14] = temp;
	temp = 0;
	temp = SEC >> 24;
	uart_tx[15] = temp;
	
}

/***************************************************************************//**
 * @brief
 *   
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
payload_echo_send(){
	uint8_t temp = 0b00000000;
	temp = VER | TYPE << 3| SHDR << 4 | APID << 5;
	uart_tx[0] = temp;
	temp = 0;

}

/***************************************************************************//**
 * @brief
 *   
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
payload_HK_send(){
	// int uart_tx[36];
	// int temp1 = 0;
	// int temp2 = 0;
	// int temp3 = 0;
	// int temp4 = 0;
	// temp1 = VER | TYPE << 3| SHDR << 4 | APID << 5;
	// temp2 = APID >> 3;
	// temp3 = GRP | SEQ << 2;
	// temp4 = SEQ >> 6;
	// uart_tx[0] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	// temp1 = 0;
	// temp2 = 0;
	// temp3 = 0;
	// temp4 = 0;
	// temp1 = LEN;
	// temp2 = LEN >> 8;
	// temp3 = SCID;
	// temp4 = FILL1 | FLASH_BLOCK << 1;
	// uart_tx[1] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	// temp1 = 0;
	// temp2 = 0;
	// temp3 = 0;
	// temp4 = 0;
	// temp1 = FLASH_BLOCK >> 7;
	// temp2 = FILL2 | TIME_QUALITY << 2 | USEC << 4;
	// temp3 = USEC >> 4;
	// temp4 = USEC >> 12;
	// uart_tx[2] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	// temp1 = 0;
	// temp2 = 0;
	// temp3 = 0;
	// temp4 = 0;
	// uart_tx[3] = SEC;


	char uart_tx[256];
	//How pass?
	//1
	packet_head(uart_tx);
	//2
	head[16] = packet_head();

	int packet_tail_location = 12;
	/*
	MSN: CDH State Bitfield
	0x0- Nominal Science
	0x1- Reduced Science
	0x2- Safe Mode
	*/
	char CDH_Info;
	uart_tx[packet_tail_location] = CDH_Info;
	packet_tail_location++;

	//setup as global variable to remeber last opcode
	char CMD_Last_OpCode;
	uart_tx[packet_tail_location] = CMD_Last_OpCode;
	packet_tail_location++;

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
	char CMD_Last_Status;
	uart_tx[packet_tail_location] = CMD_Last_Status;
	packet_tail_location++;

	//make global command counters
	short CMD_Accept_Count;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CMD_Accept_Count);
	
	//make global command counters
	short CMD_Reject_Count;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CMD_Reject_Count);
	
	/*
	MSB - little-endian

	Major, Minor Masks:
	0xF0 - Major
	0x0F - Minor
	*/
	char FSW_Major_Minor;	
	uart_tx[packet_tail_location] = FSW_Major_Minor;
	packet_tail_location++;

	/*
	MSB - little-endian

	0x-F: PATCH Version
	0x3-: Flight Model
	0x4-: ADCS Gain Detuned
	0x8-: ADCS Using SPS
	*/
	char FSW_Patch_Version;
	uart_tx[packet_tail_location] = FSW_Patch_Version;
	packet_tail_location++;
	
	short ContactTx_TimeoutCounter;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, ContactTx_TimeoutCounter);
	
	int TIME_Offset;
	packet_tail_location = IntToChar(uart_tx, 254, packet_tail_location, TIME_Offset);
	
	short Xiphos_V;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Xiphos_V);
	
	short HXR_5V;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, HXR_5V);
	
	short SXR_5V;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, SXR_5V);
	
	short Xiphos_Power;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Xiphos_Power);
	
	short Xiphos_Die_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Xiphos_Die_Temp);
	
	short Xiphos_Board_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Xiphos_Board_Temp);
	
	short Xiphos_Current;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Xiphos_Current);
	
	short STEAM_Current;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, STEAM_Current);
	
	short HXR_Current;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, HXR_Current);
	
	short SXR_Current;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, SXR_Current);
	
	short Thermistor1_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor1_Temp);
	
	short Thermistor2_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor2_Temp);
	
	short Thermistor3_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor3_Temp);
	
	short Thermistor4_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor4_Temp);
	
	short Thermistor5_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor5_Temp);
	
	short Thermistor6_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor6_Temp);
	
	short Thermistor7_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor7_Temp);
	
	short CDH_Enables;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CDH_Enables);
	
	short CDH_I2C_Err;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CDH_I2C_Err);
	
	short CDH_UART0_Err;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CDH_UART0_Err);
	
	short CDH_UART1_Err;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CDH_UART1_Err);
	
	short CDH_UART2_Err;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CDH_UART2_Err);
	
	char SXR_Brd_Temp;
	uart_tx[packet_tail_location] = SXR_Brd_Temp;
	packet_tail_location++;
	
	short SXR_Fast_Count_Norm;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, SXR_Fast_Count_Norm);
	
	short SXR_Slow_Count_Norm;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, SXR_Slow_Count_Norm);
	
	short SXR_Det_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, SXR_Det_Temp);
	
	char HXR_Brd_Temp;
	uart_tx[packet_tail_location] = HXR_Brd_Temp;
	packet_tail_location++;

	short HXR_Fast_Count_Norm;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, HXR_Fast_Count_Norm);
	
	short HXR_Slow_Count_Norm;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, HXR_Slow_Count_Norm);
	
	short HXR_Det_Temp;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, HXR_Det_Temp);
	
	//1288 bits long (161 bytes long zero filled to hey to 254 byte packet length(not including syncword))
	//Spare
	for(int i = 0; i < 161; i++){
		uart_tx[i + 89] = 0;//double check start byte is correct
	}
	
	short checkbytes = checksum(uart_tx, 250);
	uart_tx[250] = checkbytes;
	uart_tx[251] = checkbytes >> 8;//verify MSB or LSB first or Checksum output TBD
	int syncword = 0x1ACFFC1D;
	uart_tx[252] = 0x1A;// Verify MSB/LSB first
	uart_tx[253] = 0xCF;
	uart_tx[254] = 0xFC;
	uart_tx[255] = 0x1D;

}
/***************************************************************************//**
 * @brief
 *   
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
payload_science_send(){
	char uart_tx[3128];
	//How pass?
	//1
	packet_head(uart_tx);
	//2
	head[16] = packet_head();


	

	uart_tx[16] = X123_Indicator;//HXR(0x1) or SXR (0x2)

	


	// int X123_Fast_Count = 0;
	// int X123_Slow_Count = 0;
	// int X123_GP_Count = 0;
	// int X123_Accum_Time = 0;
	// int X123_Live_Time = 0;
	// int X123_Real_Time = 0;
	// int X123_HV = 0;
	// int X123_Det_Temp = 0;
	// int X123_Brd_Temp = 0;
	// int X123_Flags;

	int uart_array_temp[32] = get_x123_status();

	for(int i = 0; i < 32; i++){
		uart_tx[i+17] = uart_array_temp[i];
	}

	//STILL NEED BELOW UNSURE WHERE ERRORS ARE COMING FROM
	//X123_Rd_Wr_Errors where from..
	char X123_Rd_Wr_Errors;
	uart_tx[49] = X123_Rd_Wr_Errors;

	get_x123_packet();

	for(int i = 0; i < 3072; i++){
		uart_tx[i+50] = X123_Data_First[i];
	}
	//done with Request Status packet DP5
	// X123_Fast_Count
	// X123_Slow_Count
	// X123_GP_Count
	// X123_Accum_Time
	// X123_Live_Time
	// X123_Real_Time
	// X123_HV
	// X123_Det_Temp
	// X123_Brd_Temp
	// X123_Flags

	
	// uint8_t X123_Data_First[3072],
	short checkbytes = checksum(uart_tx, 3122);
	uart_tx[3122] = checkbytes;
	uart_tx[3123] = checkbytes >> 8;//verify MSB or LSB first or Checksum output TBD
	int syncword = 0x1ACFFC1D;
	uart_tx[3124] = 0x1A;// Verify MSB/LSB first
	uart_tx[3125] = 0xCF;
	uart_tx[3126] = 0xFC;
	uart_tx[3127] = 0x1D;


	// short checkbytes = checksum(uart_tx, 3122);
	// uart_tx[3122] = checkbytes;
	// int syncword = 0x1ACFFC1D;
}

/***************************************************************************//**
 * @brief
 *   
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
payload_parameter_dump_send(){
	
}
/***************************************************************************//**
 * @brief
 *   
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
payload_log_msg_send(){
	
}


//NOT COMPLETED - IMPLEMENTATION INQUIRY IN PROGRESS
int checksum(char* array, int length, int location){https://www.tutorialspoint.com/fletcher-s-checksum
	int checkbytes = 0;
	for(int i = 0; i < length; i++){
		checkbytes += array[i];
	}
	return checkbytes;
}


/***************************************************************************//**
 * @brief Add a 32 bit value to an 8 bit aligned array (Little Endian)
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

int IntToChar(char* array, int length, int location, int var){
	if(location + 4 < length){
		for(int k = 0; k < 4; k++){
			array[location + k] = var >> (k*8);
		}
	}
	return location + 4;
}

/***************************************************************************//**
 * @brief Add a 16 bit value to an 8 bit aligned array (Little Endian)
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
int ShortToChar(char*array, int length, int location, short var){
	if(location + 2 < length){
		for(int k = 0; k < 2; k++){
			array[location + k] = var >> (k*8);
		}
	}
	return location + 2;
}


payload_HK_dummy_packet(){



	char uart_tx[256];

	packet_head(uart_tx);

	int packet_tail_location = 12;
	/*
	MSN: CDH State Bitfield
	0x0- Nominal Science
	0x1- Reduced Science
	0x2- Safe Mode
	*/
	char CDH_Info = 0;
	uart_tx[packet_tail_location] = CDH_Info;
	packet_tail_location++;

	//setup as global variable to remeber last opcode
	char CMD_Last_OpCode = 0;
	uart_tx[packet_tail_location] = CMD_Last_OpCode;
	packet_tail_location++;

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
	uart_tx[packet_tail_location] = CMD_Last_Status;
	packet_tail_location++;

	//make global command counters
	short CMD_Accept_Count = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CMD_Accept_Count);
	
	//make global command counters
	short CMD_Reject_Count = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CMD_Reject_Count);
	
	/*
	MSB - little-endian

	Major, Minor Masks:
	0xF0 - Major
	0x0F - Minor
	*/
	char FSW_Major_Minor = 0;	
	uart_tx[packet_tail_location] = FSW_Major_Minor;
	packet_tail_location++;

	/*
	MSB - little-endian

	0x-F: PATCH Version
	0x3-: Flight Model
	0x4-: ADCS Gain Detuned
	0x8-: ADCS Using SPS
	*/
	char FSW_Patch_Version = 0;
	uart_tx[packet_tail_location] = FSW_Patch_Version;
	packet_tail_location++;
	
	short ContactTx_TimeoutCounter = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, ContactTx_TimeoutCounter);
	
	int TIME_Offset = 0;
	packet_tail_location = IntToChar(uart_tx, 254, packet_tail_location, TIME_Offset);
	
	short Xiphos_V = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Xiphos_V);
	
	short HXR_5V = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, HXR_5V);
	
	short SXR_5V = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, SXR_5V);
	
	short Xiphos_Power = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Xiphos_Power);
	
	short Xiphos_Die_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Xiphos_Die_Temp);
	
	short Xiphos_Board_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Xiphos_Board_Temp);
	
	short Xiphos_Current = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Xiphos_Current);
	
	short STEAM_Current = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, STEAM_Current);
	
	short HXR_Current = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, HXR_Current);
	
	short SXR_Current = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, SXR_Current);
	
	short Thermistor1_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor1_Temp);
	
	short Thermistor2_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor2_Temp);
	
	short Thermistor3_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor3_Temp);
	
	short Thermistor4_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor4_Temp);
	
	short Thermistor5_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor5_Temp);
	
	short Thermistor6_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor6_Temp);
	
	short Thermistor7_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, Thermistor7_Temp);
	
	short CDH_Enables = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CDH_Enables);
	
	short CDH_I2C_Err = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CDH_I2C_Err);
	
	short CDH_UART0_Err = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CDH_UART0_Err);
	
	short CDH_UART1_Err = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CDH_UART1_Err);
	
	short CDH_UART2_Err = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, CDH_UART2_Err);
	
	char SXR_Brd_Temp = 0;
	uart_tx[packet_tail_location] = SXR_Brd_Temp;
	packet_tail_location++;
	
	short SXR_Fast_Count_Norm = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, SXR_Fast_Count_Norm);
	
	short SXR_Slow_Count_Norm = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, SXR_Slow_Count_Norm);
	
	short SXR_Det_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, SXR_Det_Temp);
	
	char HXR_Brd_Temp = 0;
	uart_tx[packet_tail_location] = HXR_Brd_Temp;
	packet_tail_location++;

	short HXR_Fast_Count_Norm = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, HXR_Fast_Count_Norm);
	
	short HXR_Slow_Count_Norm = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, HXR_Slow_Count_Norm);
	
	short HXR_Det_Temp = 0;
	packet_tail_location = ShortToChar(uart_tx, 254, packet_tail_location, HXR_Det_Temp);
	
	//1288 bits long (161 bytes long zero filled to hey to 254 byte packet length(not including syncword))
	//Spare
	for(int i = 0; i < 161; i++){
		uart_tx[i + 89] = 0;//double check start byte is correct
	}
	
	short checkbytes = checksum(uart_tx, 256, 250);
	uart_tx[250] = checkbytes;
	uart_tx[251] = checkbytes >> 8;//verify MSB or LSB first or Checksum output TBD
	int syncword = 0x1ACFFC1D;
	uart_tx[252] = 0x1A;// Verify MSB/LSB first
	uart_tx[253] = 0xCF;
	uart_tx[254] = 0xFC;
	uart_tx[255] = 0x1D;

}


payload_science_dummy_packet(){
	char uart_tx[3128];
	packet_head(uart_tx);//needs more parameters figure out later

	uart_tx[16] = 0x01;//X123_Indicator;//HXR(0x1) or SXR (0x2)
	int uart_array_temp[32] = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
	for(int i = 0; i < 32; i++){
		uart_tx[i+17] = uart_array_temp[i];
	}
	char X123_Rd_Wr_Errors = 0;
	uart_tx[49] = X123_Rd_Wr_Errors;
	for(int i = 0; i < 3072; i++){
		uart_tx[i+50] = 0;
	}
	short checkbytes = checksum(uart_tx, 3122);
	uart_tx[3122] = checkbytes;
	uart_tx[3123] = checkbytes >> 8;//verify MSB or LSB first or Checksum output TBD
	int syncword = 0x1ACFFC1D;
	uart_tx[3124] = 0x1A;// Verify MSB/LSB first
	uart_tx[3125] = 0xCF;
	uart_tx[3126] = 0xFC;
	uart_tx[3127] = 0x1D;
}























/***************************************************************************//**
 * 
 *   
 *			Extra not ready to delete stuff
 * 
 *
 ******************************************************************************




Measurements(interrupt)


initialize

loop{
HMS
three loops SEND
}
Measurments(10 second Interrupt)
payload_parameter_dump_send();async (on request)(data)
payload_log_msg_send();async (on error we send)
payload_echo_send();async (after a command)(more confirmation)


Uart/232
anyhting to spectrometers
i2c
HMS to ADCs
422
Anything to spacecraft

}








*/
// Static int scheduled_event;
// Volatile int mode; // initialize to safe mode
// /*
//  * 0 for Nominal Power Mode
//  * 1 for Reduced Power Mode
//  * 2 for Safe Mode
//  */

// main()
// {
// 	mode = 2;
// 	Initialize HMS
// 	Initialize timer for HMS check (set period to 3 sec)
// 	Enable interrupts for RS-422, timer

// 	mode = 0; // nominal power mode

// 	sxr_open
// 	While (MOSFET 1 current > 0.9);

// 	hxr_open
// 	While (MOSFET 2 current > 0.9);

// 	while(1)
// 	{
// 		If (scheduled_event & CHECK_HMS)
// 		{
// 			scheduled_event &= ~CHECK_HMS;
// 			check_hms();
// 		}

// 		If (scheduled_event & PARSE_COMMAND)
// 		{
// 			scheduled_event &= ~PARSE_COMMAND;
// 			return_packet = parse_punch_command(command);
			
// 		}

// 		If (scheduled_event & MODE_CHANGE)
// // unless we want to do HMS handling differently
// 		{
// 			scheduled_event &= ~MODE_CHANGE;
// 			change_mode(mode);
// 		}

// 		// Spectrometer data has already been recorded in parameter table with shell
// 		// script, so fetch_spec_data() reads from this table
// If (scheduled_event & FETCH_SPEC_DATA)
// 		{
// 			scheduled_event &= ~FETCH_SPEC_DATA;
// 			fetch_spec_data(spec); // specify SXR or HXR
// 		}

// 		If (scheduled_event & SPEC_CONFIG) //may be used if health interrupt received
// 		{
// 			scheduled_event &= ~SPEC_CONFIG;
// 			spec_config(spec); // specify SXR or HXR
// 		}
// 	}
// }

// // not sure if this is how we’re handling interrupts
// rs422_irq_handler(void) {
// 	Clear interrupt flag
// 	command = RS-422 message;
// 	scheduled_event |= PARSE_COMMAND;
// }

// timer_irq_handler(void) {
// 	Clear interrupt flag
// 	scheduled_event |= CHECK_HMS;
// }



// // OLD STUFF ABOVE
// // OLD STUFF ABOVE
// OLD STUFF ABOVE