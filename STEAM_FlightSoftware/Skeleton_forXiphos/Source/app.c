#include "app.h"

/***************************************************************************//**
 * @brief Application Peripherals Setup
 *   
 *
 * @details
 * 	 This function runs all the open and configure commands
 *   This will run in the beginning of main
 *
 * @note
 *	 
 * @param[in] variable_name
 *  
 *
 ******************************************************************************/

void app_peripheral_setup(){
    gpio_open();
    i2c_open();
    uart_open();
    timer_open();

    adc_open();
    xadc_open();
    sxr_open();
    hxr_open();

    enable_interrupts();
}

/***************************************************************************//**
 * @brief Initialize the Timer
 *   
 *
 * @details
 * 	 Initialize the timer for any time functionality. 
 *   These values are subject to change based off of commands from the spacecraft
 *
 * @note
 *	 
 * @param[in] variable_name
 *  
 *
 ******************************************************************************/
void timer_open(){
    //initialize a 1,3, and 10s timer
}


/***************************************************************************//**
 * @brief 
 *   Sends the payload echo packet
 *
 * @details
 * 	 This will configure data to be sent and then send the data
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
 *   Sends the payload housekeeping packet
 *
 * @details
 * 	 This will configure data to be sent and then send the data
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
 *   Sends the payload parameter dump packet
 *
 * @details
 * 	 This will configure data to be sent and then send the data
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
 *   Sends the payload log packet
 *
 * @details
 * 	 This will configure data to be sent and then send the data
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
 *   Sends the payload science packet
 *
 * @details
 * 	 This will configure data to be sent and then send the data
 *
 * @note
 *	 
 * @param[in] variable_name
 *  
 *
 ******************************************************************************/
payload_science_send(){
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