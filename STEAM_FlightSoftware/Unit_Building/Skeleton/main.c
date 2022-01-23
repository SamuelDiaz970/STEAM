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
packet_head(uint8_t VER,
  uint8_t TYPE,
  uint8_t SHDR,
  uint8_t APID,
  uint8_t GRP,
  uint8_t SEQ,
  uint8_t LEN,
  uint8_t SCID,
  uint8_t FILL1,
  uint8_t FLASH_BLOCK,
  uint8_t FILL2,
  uint8_t TIME_QUALITY,
  uint8_t USEC,
  uint8_t SEC){

	uint8_t uart_tx[36];
	uint8_t temp = 0b00000000;
	temp = VER | TYPE << 3| SHDR << 4 | APID << 5;
	uart_tx[0] = temp;
	temp = 0;

	etc.
	



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
