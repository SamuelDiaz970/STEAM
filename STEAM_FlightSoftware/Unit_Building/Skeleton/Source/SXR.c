// insert here
#include "SXR.h"

sxr_open(){

}

sxr_read_detector_temp(){

}

sxr_read_board_temp(){

}


int* get_x123_status(){
    //request status packet
    char status_packet_send[8] = {0xF5, 0xFA, 1, 1, 0, 0, 0xFE, 0x0F};
    UART_SEND(status_packet_send);

    char status_packet[32];
    int count = 0;
    for(int i = 0; i < 64; i++){
        if(i < 24){
            status_packet[count] = UART_RECIEVE();
            count++;
        }
        if(i >= 30 && i <= 35){
            status_packet[count] = UART_RECIEVE();
            count++;
        }
        if(i == 35){
            status_packet[30] = UART_RECIEVE();
        }
        if(i == 36){
            status_packet[31] = UART_RECIEVE();
        }
        if(i = 38){
            status_packet[29] = UART_RECIEVE();
        }
    }

    return status_packet;//need to configure to int?? or config other to char(8bits)
}

void get_x123_packet(){
    // //Spectrum
    //     0xF5 0xFA 2 1 0 0 0xFE 0x0E
    // //Spectrum and Clear
    //     0xF5 0xFA 2 2 0 0 0xFE 0x0D
    // //Spectrum + status
    //     0xF5 0xFA 2 3 0 0 0xFE 0x0C
    // //Spect + Clear + status
    //     0xF5 0xFA 2 4 0 0 0xFE 0x0B
    char status_packet_send[8] = {};
    UART_SEND(status_packet_send);

    char status_packet[32];
    int count = 0;
    
    X123_Data_First[3072];
    for(int i = 0; i < 3072; i++){
        X123_Data_First[i] = UART_RECIEVE();
    }  
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

	sxr_open
	While (MOSFET 1 current > 0.9);

	hxr_open
	While (MOSFET 2 current > 0.9);

	while(1)
	{
		If (scheduled_event & CHECK_HMS)
		{
			scheduled_event &= ~CHECK_HMS;
			check_hms();
		}

		If (scheduled_event & PARSE_COMMAND)
		{
			scheduled_event &= ~PARSE_COMMAND;
			return_packet = parse_punch_command(command);
			
		}

		If (scheduled_event & MODE_CHANGE)
// unless we want to do HMS handling differently
		{
			scheduled_event &= ~MODE_CHANGE;
			change_mode(mode);
		}

		// Spectrometer data has already been recorded in parameter table with shell
		// script, so fetch_spec_data() reads from this table
If (scheduled_event & FETCH_SPEC_DATA)
		{
			scheduled_event &= ~FETCH_SPEC_DATA;
			fetch_spec_data(spec); // specify SXR or HXR
		}

		If (scheduled_event & SPEC_CONFIG) //may be used if health interrupt received
		{
			scheduled_event &= ~SPEC_CONFIG;
			spec_config(spec); // specify SXR or HXR
		}
	}
}

// not sure if this is how we’re handling interrupts
rs422_irq_handler(void) {
	Clear interrupt flag
	command = RS-422 message;
	scheduled_event |= PARSE_COMMAND;
}

timer_irq_handler(void) {
	Clear interrupt flag
	scheduled_event |= CHECK_HMS;
}



// OLD STUFF ABOVE
// OLD STUFF ABOVE
// OLD STUFF ABOVE


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
	char uart_tx[36];
	int temp1 = 0;
	int temp2 = 0;
	int temp3 = 0;
	int temp4 = 0;
	
	
	temp1 = VER | TYPE << 3| SHDR << 4 | APID << 5;
	temp2 = APID >> 3;
	temp3 = GRP | SEQ << 2;
	temp4 = SEQ >> 6;
	uart_tx[0] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp4 = 0;
	temp1 = LEN;
	temp2 = LEN >> 8;
	temp3 = SCID;
	temp4 = FILL1 | FLASH_BLOCK << 1;
	uart_tx[1] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp4 = 0;
	temp1 = FLASH_BLOCK >> 7;
	temp2 = FILL2 | TIME_QUALITY << 2 | USEC << 4;
	temp3 = USEC >> 4;
	temp4 = USEC >> 12;
	uart_tx[2] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp4 = 0;
	uart_tx[3] = SEC;
	
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
	int uart_tx[36];
	int temp1 = 0;
	int temp2 = 0;
	int temp3 = 0;
	int temp4 = 0;
	temp1 = VER | TYPE << 3| SHDR << 4 | APID << 5;
	temp2 = APID >> 3;
	temp3 = GRP | SEQ << 2;
	temp4 = SEQ >> 6;
	uart_tx[0] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp4 = 0;
	temp1 = LEN;
	temp2 = LEN >> 8;
	temp3 = SCID;
	temp4 = FILL1 | FLASH_BLOCK << 1;
	uart_tx[1] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp4 = 0;
	temp1 = FLASH_BLOCK >> 7;
	temp2 = FILL2 | TIME_QUALITY << 2 | USEC << 4;
	temp3 = USEC >> 4;
	temp4 = USEC >> 12;
	uart_tx[2] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp4 = 0;
	uart_tx[3] = SEC;

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
	
	int checkbytes
	
	int syncword

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
	
	char uart_tx[36];
	int temp1 = 0;
	int temp2 = 0;
	int temp3 = 0;
	int temp4 = 0;
	temp1 = VER | TYPE << 3| SHDR << 4 | APID << 5;
	temp2 = APID >> 3;
	temp3 = GRP | SEQ << 2;
	temp4 = SEQ >> 6;
	uart_tx[0] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp4 = 0;
	temp1 = LEN;
	temp2 = LEN >> 8;
	temp3 = SCID;
	temp4 = FILL1 | FLASH_BLOCK << 1;
	uart_tx[1] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp4 = 0;
	temp1 = FLASH_BLOCK >> 7;
	temp2 = FILL2 | TIME_QUALITY << 2 | USEC << 4;
	temp3 = USEC >> 4;
	temp4 = USEC >> 12;
	uart_tx[2] = temp1 | temp2 << 8 | temp3 << 16 | temp4 << 24;
	
	temp1 = 0;
	temp2 = 0;
	temp3 = 0;
	temp4 = 0;
	uart_tx[3] = SEC;

	
	
	uart_tx[4] = X123_Indicator;

	


	int X123_Fast_Count = 0;
	int X123_Slow_Count = 0;
	int X123_GP_Count = 0;
	int X123_Accum_Time = 0;
	int X123_Live_Time = 0;
	int X123_Real_Time = 0;
	int X123_HV = 0;
	int X123_Det_Temp = 0;
	int X123_Brd_Temp = 0;
	int X123_Flags;

	int uart_array_temp[32] = get_x123_status();

	for(int i = 0; i < 32; i++){
		uart_tx[5+i] = uart_array_temp[i];
	}

	get_x123_packet();

	for(i = 0; i< )
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

	X123_Rd_Wr_Errors
	uint8_t X123_Data_First[3072],
	uint16_t checkbytes,
	uint16_t syncword
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
