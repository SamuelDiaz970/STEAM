#include "uart.h"
#include "spectrometer_handler.h"

/***************************************************************************//**
 * @brief Soft X-Ray Spectrometer Initialization
 *
 * @details
 *
 * @note
 *	 
 * @param[in] variable_name
 *
 ******************************************************************************/
sxr_open(){

}

/***************************************************************************//**
 * @brief Send Soft X-Ray Spectrometer Command
 *
 * @details
 *
 * @note
 *	 
 * @param[in] variable_name
 *
 ******************************************************************************/
sxr_command(string command){
    uart_write();   
}

/***************************************************************************//**
 * @brief Hard X-Ray Spectrometer Initialization
 *
 * @details
 *
 * @note
 *	 
 * @param[in] variable_name
 *
 ******************************************************************************/
hxr_open(){

}

/***************************************************************************//**
 * @brief Send Hard X-Ray Spectrometer Command
 *
 * @details
 *
 * @note
 *	 
 * @param[in] variable_name
 *
 ******************************************************************************/
hxr_command(string command){
    uart_write();   
}

