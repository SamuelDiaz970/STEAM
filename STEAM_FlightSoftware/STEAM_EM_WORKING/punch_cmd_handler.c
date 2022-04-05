// rx_buff_length[]
#include "punch_cmd_handler.h"
#include <errno.h>

int science_pckt_rate;
int housekeeping_pckt_rate;

int set_hk = 0;
int get_hk_rate(){
    // int i = 0;
    if(set_hk == 0){
        housekeeping_pckt_rate = 3;
        set_hk = 1;
    }
    return housekeeping_pckt_rate;
}
int set_sci = 0;
int get_sci_rate(){
    // int i = 0;
    if(set_sci == 0){
        science_pckt_rate = 10;
        set_sci = 1;
    }
    return science_pckt_rate;
}
/*
Punch commands
NOOP
Reset Counters
Set STEAM Time
Copy Parameter Set
Dump Parameter Set (sends special Memory Dump Packet)
Set Full Power Mode
Set Reduced Power Mode
Set Safe Mode
Set Mode Trigger Persistence
Set Max Tries Full Power-2-Safe
Set CMD Echo Packet Enable
Issue Real-Time HK Packet
Set HK Packet Rate
Set SCI Packet Rate
Route LOG Packet
Set Contact Tx Timeout
Switch Power X123
Set I2C Timeout
Write to  NOR Flash (PROTECTED command - dual cmd)
Read from NOR Flash 
Send X123 Command
Query X123 Status (send Query Info)
Enable Dual Command
Autoset X123 Input Offset (PROTECTED command - dual cmd)
Autoset X123 Fast Threshold (PROTECTED command - dual cmd)
X123: Request Spectrum plus Status
X123: Request Diagnostic Data
Trigger Watchdog Reset
Set Last CMD Watchdog Timeout
Set detector power delay
Power Cycle STEAM (PROTECTED command - dual cmd)
*/


// typedef enum{
// 	NOOP,
//     reset_counters,
//     set_STEAM_Time,
//     Copy_Parameter_Set,
//     Dump_Parameter_Set,
//     Set_Full_Power_Mode,
//     Set_Reduced_Power_Mode,
//     Set_Safe_Mode,
//     Set_Mode_Trigger_Persistence,
//     Set_Max_Tries_Full_Power2Safe,
//     Set_CMD_Echo_Packet_Enable,
//     Issue_RealTime_HK_Packet,
//     Set_HK_Packet_Rate,
//     Set_SCI_Packet_Rate,
//     Route_LOG_Packet,
//     Set_Contact_Tx_Timeout,
//     Switch_Power_X123,
//     Set_I2C_Timeout,
//     Write_to_NOR_Flash,
//     Read_from_NOR_Flash,
//     Send_X123_Command,
//     Query_X123_Status,
//     Enable_Dual_Command,
//     Autoset_X123_Input_Offset,
//     Autoset_X123_Fast_Threshold,
//     X123_Request_Spectrum_plus_Status,
//     X123_Request_Diagnostic_Data,
//     Trigger_Watchdog_Reset,
//     Set_Last_CMD_Watchdog_Timeout,
//     Set_detector_power_delay,
//     Power_Cycle_STEAM,
// }punch_commands;

#define NOOP                        0x00
#define NOOP_LENGTH                 8
#define SET_STEAM_TIME              0x06
#define SET_STEAM_TIME_LENGTH       15
#define CMD_ECHO_PACKET             0x75
#define SET_HK_RATE                 0x39
#define SET_SCIENCE_RATE            0x48
#define SWITCH_POWER_X123           0x96

int Command_Accept_Count = 0;
int Command_Reject_Count = 0;
int Cmd_Echo_Count = 0;
char Last_OpCode = 0;

int get_cmd_accept(){
    // int i = 0;
    // if(set_hk == 0){
    //     housekeeping_pckt_rate = 3;
    //     set_hk = 1;
    // }
    return Command_Accept_Count;
}

int get_cmd_reject(){
    // int i = 0;
    // if(set_hk == 0){
    //     housekeeping_pckt_rate = 3;
    //     set_hk = 1;
    // }
    return Command_Reject_Count;
}

int get_last_opcode(){
    // int i = 0;
    // if(set_hk == 0){
    //     housekeeping_pckt_rate = 3;
    //     set_hk = 1;
    // }
    return Last_OpCode;
}

int command_handle(struct punch_packet command_buf){
	buffer_print(command_buf.buf,command_buf.size);
    int command = 0;
    command = (command_buf.buf[0] & 0b00000111) <<8 | command_buf.buf[1];
    Last_OpCode = command;
    printf("    cmd: %d   \n", command);
    unsigned short checksum = 0;
    printf("    size: %d   \n", command_buf.size);
    for(int i = 0; i < command_buf.size-2; i++){
        printf("CMD BUFF    %x\n", command_buf.buf[i]);
        checksum += command_buf.buf[i];
    }
    printf("    check1: %x   \n", checksum);
    unsigned short com_check = command_buf.buf[command_buf.size-2] | command_buf.buf[command_buf.size-1] << 8;
    printf("    check2: %x   \n", com_check);
    if (com_check != checksum) {
        echo_packet(command_buf, 5);
        return 5;
    }
    int status = 1;
    switch(command){
        case NOOP:
            status = noop();
            break;
        // case reset_counters:
        //     reset_counters();
        //     break;
        case SET_STEAM_TIME:
            status = set_steam_time(command_buf);
            break;
        // case Copy_Parameter_Set:
        //     Copy_Parameter_Set();
        //     break;
        // case Copy_Parameter_Set:
        //     Copy_Parameter_Set();
        //     break;
        // case Set_Full_Power_Mode:
        //     Set_Full_Power_Mode();
        //     break;
        // case Set_Reduced_Power_Mode:
        //     Set_Reduced_Power_Mode();
        //     break;
        // case Set_Safe_Mode:
        //     Set_Safe_Mode();
        //     break;
        // case Set_Mode_Trigger_Persistence:
        //     Set_Mode_Trigger_Persistence();
        //     break;
        // case Set_Max_Tries_Full_Power2Safe:
        //     Set_Max_Tries_Full_Power2Safe();
        //     break;
        // case Set_CMD_Echo_Packet_Enable:
        //     Set_CMD_Echo_Packet_Enable();
        //     break;
        // case Issue_RealTime_HK_Packet:
        //     Issue_RealTime_HK_Packet();
        //     break;
        case SET_HK_RATE:
            status = Set_HK_Packet_Rate(command_buf);
            break;
        case SET_SCIENCE_RATE:
            status = Set_SCI_Packet_Rate(command_buf);
            break;
        // case Route_LOG_Packet:
        //     Route_LOG_Packet();
        //     break;
        // case Set_Contact_Tx_Timeout:
        //     Set_Contact_Tx_Timeout();
        //     break;
        case SWITCH_POWER_X123:
            status = Switch_Power_X123(command_buf);
            break;
        // case Set_I2C_Timeout:
        //     Set_I2C_Timeout();
        //     break;
        // case Write_to_NOR_Flash:
        //     Write_to_NOR_Flash();
        //     break;
        // case Read_from_NOR_Flash:
        //     Read_from_NOR_Flash();
        //     break;
        // case Send_X123_Command:
        //     Send_X123_Command();
        //     break;
        // case Query_X123_Status:
        //     Query_X123_Status();
        //     break;
        // case Enable_Dual_Command:
        //     Enable_Dual_Command();
        //     break;
        // case Autoset_X123_Input_Offset:
        //     Autoset_X123_Input_Offset();
        //     break;
        // case Autoset_X123_Fast_Threshold:
        //     Autoset_X123_Fast_Threshold();
        //     break;
        // case X123_Request_Spectrum_plus_Status:
        //     X123_Request_Spectrum_plus_Status();
        //     break;
        // case X123_Request_Diagnostic_Data:
        //     X123_Request_Diagnostic_Data();
        //     break;
        // case Trigger_Watchdog_Reset:
        //     Trigger_Watchdog_Reset();
        //     break;
        // case Set_Last_CMD_Watchdog_Timeout:
        //     Set_Last_CMD_Watchdog_Timeout();
        //     break;
        // case Set_detector_power_delay:
        //     Set_detector_power_delay();
        //     break;
        // case Power_Cycle_STEAM:
        //     Power_Cycle_STEAM();
        //     break;
        default:
            status = 3;
            // Command_Reject_Count++;
            break;
    }
    if(status == 0){
        Command_Accept_Count++;
    }
    else{
        Command_Reject_Count++;
    }
    echo_packet(command_buf, status);
    return status;
}




/***************************************************************************//**
 * @brief NOOP Command From Spacecraft
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

int noop(){
    printf("NOOP command Recieved");
    return 0;
}

/***************************************************************************//**
 * @brief reset counters Command From Spacecraft
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

// reset_counters(){

// }

/***************************************************************************//**
 * @brief set steam time Command From Spacecraft
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
void echo_packet(struct punch_packet p, int status){
    char buffer[40];
    int length = 0;
    int VER = 0;
    int TYPE = 0;
    int SHDR = 1;
    int APID = CMD_ECHO_PACKET;
    int GRP = 0;
    int SEQ = Cmd_Echo_Count;
    Cmd_Echo_Count++;
    int LEN = 36 - 7;
    int SCID = 0x0E;
    int FILL1 = 0;
    int FLASH_BLOCK = 0;
    int FILL2 = 0;
    int TIME_QUALITY = 0;
    int err = 1;
    err = gettimeofday(&tvv,NULL);
    if(err == -1) return;
    int USEC = tvv.tv_usec;
    int SEC = tvv.tv_sec;

    
    
    buffer[length] = (VER << 5 | TYPE << 4| SHDR << 3 | (APID >> 8) );
    length++;
    buffer[length] = (APID);
    length++;
    buffer[length] = (GRP << 6| (SEQ >> 8));
    length++;
    buffer[length] = (SEQ);
    length++;
    buffer[length] = (LEN >> 8);
    length++;
    buffer[length] = (LEN);
    length++;
    buffer[length] = (SCID);
    length++;
    buffer[length] = (FILL1 | FLASH_BLOCK << 1);
    length++;
    buffer[length] = (FLASH_BLOCK >> 7);
    length++;
    buffer[length] = (FILL2 | TIME_QUALITY << 2 | USEC << 4);
    length++;
    buffer[length] = (USEC >> 4);
    length++;
    buffer[length] = (USEC >> 12);
    length++;
    buffer[length] = (SEC);
    length++;
    buffer[length] = (SEC >> 8);
    length++;
    buffer[length] = (SEC >> 16);
    length++;
    buffer[length] = (SEC >> 24);
    length++;
    buffer[length] = 0;
    length++;
    buffer[length] = Last_OpCode;
    length++;
    buffer[length] = status;
    length++;
    buffer[length] = Command_Accept_Count;
    length++;
    buffer[length] = Command_Accept_Count >> 8;
    length++;
    buffer[length] = Command_Reject_Count;
    length++;
    buffer[length] = Command_Reject_Count >> 8;
    length++;
    for(int i = 0; i < 11; i++){
        buffer[length] = 0;
        length++;
    }
    int checksum = 0;
    for(int i = 0; i < length; i++){
        checksum += buffer[i];
    }
    buffer[length] = checksum;
    length++;
    buffer[length] = checksum>>8;
    length++;
    buffer[length] = 0x1A;
    length++;
    buffer[length] = 0xCF;
    length++;
    buffer[length] = 0xFC;
    length++;   
    buffer[length] = 0x1D;
    length++;
    
    struct punch_packet w;
    w.buf = buffer;
    w.size = length;
    punch_write(w);
}


// 10 00 00 00 00 01 11 00
// 10 06 00 00 00 08 00 00 00 01 00 00 00 1F 00
// 10 06 00 00 00 08 9E 2D DE 29 00 00 00 F0 01
// 29DE 2D9E
// 10 39 00 00 00 03 09 00 55 00
// 39
/***************************************************************************//**
 * @brief set steam time Command From Spacecraft
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


int set_steam_time(struct punch_packet p){

    unsigned long time_sec = 100;
    unsigned long time_usec = 1000;
    time_sec = p.buf[9]<<24 | p.buf[8] <<16 | p.buf[7] <<8 | p.buf[6];
    time_usec = (0b00001111 & p.buf[12]) <<16 | p.buf[11] <<8 | p.buf[10];
    
    tvv.tv_sec = (time_t)time_sec;
	tvv.tv_usec = (suseconds_t)time_usec;

    printf("time    %lu              %lu\n",time_sec,time_usec);
    struct timezone tzz;
    tzz.tz_dsttime = 0;
    tzz.tz_minuteswest = 0;
	int err = 1;
    err = settimeofday(&tvv,NULL);
    if(err == -1){
        printf("set tiome error %d", errno);
        return 3;
    }
    


    return 0;
}
// /***************************************************************************//**
//  * @brief copy parameter set Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Copy_Parameter_Set(){

// }

// /***************************************************************************//**
//  * @brief dump parameter Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Dump_Parameter_Set(){

// }

// /***************************************************************************//**
//  * @brief set full power mode Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Set_Full_Power_Mode(){

// }

// /***************************************************************************//**
//  * @brief set reduced power mode Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Set_Reduced_Power_Mode(){

// }

// /***************************************************************************//**
//  * @brief set safe mode Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Set_Safe_Mode(){

// }

// /***************************************************************************//**
//  * @brief set mode trigger persistence Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Set_Mode_Trigger_Persistence(){

// }

// /***************************************************************************//**
//  * @brief set max tries full power to safe Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Set_Max_Tries_Full_Power2Safe(){

// }

// /***************************************************************************//**
//  * @brief set command echo packet enable Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Set_CMD_Echo_Packet_Enable(){

// }

// /***************************************************************************//**
//  * @brief issue real time house keeping packets Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Issue_RealTime_HK_Packet(){

// }

/***************************************************************************//**
 * @brief set housekeeping packet rate Command From Spacecraft
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
// 10 39 00 00 00 03 09 00 55 00
int Set_HK_Packet_Rate(struct punch_packet p){
    if (p.size != 10) return 1;
    // printf("HK rate     %d\n", housekeeping_pckt_rate);
    housekeeping_pckt_rate = p.buf[6] | p.buf[7] << 8;
    // printf("HK rate     %d\n", housekeeping_pckt_rate);
    // housekeeping_pckt_rate = p.buf[7] << 8;
    // housekeeping_pckt_rate = p.buf[8] << 16;
    return 0;
}

/***************************************************************************//**
 * @brief set science packet rate Command From Spacecraft
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
// 10 48 00 00 00 03 0F 00 6A 00
int Set_SCI_Packet_Rate(struct punch_packet p){
    if (p.size != 10) return 1;
    science_pckt_rate = p.buf[6] | p.buf[7] << 8;
    return 0;
}

// /***************************************************************************//**
//  * @brief set the log packet route Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Route_LOG_Packet(){

// }

// /***************************************************************************//**
//  * @brief set transmit timeout Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Set_Contact_Tx_Timeout(){

// }

/***************************************************************************//**
 * @brief switch power x123 Command From Spacecraft
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
// HARD ON
// 10 96 00 00 00 02 01 A9 00
// HARD OFF
// 10 96 00 00 00 02 00 A8 00
// SOFT ON
// 10 96 00 00 00 02 11 B9 00
// SOFT OFF
// 10 96 00 00 00 02 10 B8 00
int Switch_Power_X123(struct punch_packet p){
    if (p.size != 9) return 1;
    unsigned char on_off = p.buf[6];
    if(((on_off & 0b11110000)>>4) == 0b0){
        if((on_off & 0b1111) == 0b0){
            xr_detector_turn_off(2);
            // gpio_config_hxr(false);
            // g_param.hxr_detector_on = false;
            // printf("HARD OFF         %d\n",xr_detector_get_status(2));
            return 0;
        }
        if((on_off & 0b1111) == 0b1){
            xr_detector_turn_on(2);
            // gpio_config_hxr(true);
            // g_param.hxr_detector_on = true;
            // printf("HARD ON         %d\n",xr_detector_get_status(2));
            return 0;
        }
    }
    if(((on_off & 0b11110000)>>4) == 0b1){
        if((on_off & 0b1111) == 0b0){
            // gpio_config_sxr(false);
            xr_detector_turn_off(1);
            // g_param.sxr_detector_on = false;
            // printf("SOFT OFF         %d\n",xr_detector_get_status(1));
            return 0;
        }
        if((on_off & 0b1111) == 0b1){
            xr_detector_turn_on(1);
            // gpio_config_sxr(true);
            // g_param.sxr_detector_on = true;
            // printf("SOFT ON         %d\n",xr_detector_get_status(2));
            return 0;
        }
    }
    return 5;
}

// /***************************************************************************//**
//  * @brief set the i2c timeout time Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Set_I2C_Timeout(){

// }

// /***************************************************************************//**
//  * @brief write to NOR flash Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Write_to_NOR_Flash(){

// }

// /***************************************************************************//**
//  * @brief read from NOR flash Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Read_from_NOR_Flash(){

// }

// /***************************************************************************//**
//  * @brief send a x123 command Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Send_X123_Command(){

// }

// /***************************************************************************//**
//  * @brief query x123 status Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Query_X123_Status(){

// }

// /***************************************************************************//**
//  * @brief enable the dual command, Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Enable_Dual_Command(){

// }

// /***************************************************************************//**
//  * @brief autoset the x123 input offset Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Autoset_X123_Input_Offset(){

// }

// /***************************************************************************//**
//  * @brief autoset x123 fast threshold Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Autoset_X123_Fast_Threshold(){

// }

// /***************************************************************************//**
//  * @brief x123 request spectrum plus status Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// X123_Request_Spectrum_plus_Status(){

// }

// /***************************************************************************//**
//  * @brief x123 request diagnostic data Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// X123_Request_Diagnostic_Data(){

// }

// /***************************************************************************//**
//  * @brief trigger watchdog reset Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Trigger_Watchdog_Reset(){

// }

// /***************************************************************************//**
//  * @brief set last cmd watchdog timeout Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Set_Last_CMD_Watchdog_Timeout(){

// }

// /***************************************************************************//**
//  * @brief set detector power delay Command From Spacecraft
//  *   
//  *
//  * @details
//  * 	 
//  *
//  * @note
//  *	 
//  * @param[in] variable_name
//  *  
//  *
//  ******************************************************************************/

// Set_detector_power_delay(){

// }

/***************************************************************************//**
 * @brief Power cycle STEAM Command From Spacecraft
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

// Power_Cycle_STEAM(){

// }



