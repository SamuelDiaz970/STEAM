#include "spectrometer_handler.h"
#include "rs422.h"


/*************************************************************
 *                  Packet Structs
 *  These are packets that are sent from STEAM -to-> Spacecraft
 *************************************************************/

// Echo back last command APID and status
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

/*Analog and Temperature Monitors fromXihpos, Spectrometers, 
thermistors, and additionaly current/voltage sensors CDH Flags and Status*/
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

//Dump of Parameter Set data 
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

//Log message (text) for noting error messages and autonous mode transitions
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

//"X123 1024-Channel Spectrum and Status 
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
