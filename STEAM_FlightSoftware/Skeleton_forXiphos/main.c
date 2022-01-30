/**
 * @file main.c
 * @author Kanin McGuire
 * @date 1-30-2022
 * @brief Main function
*/
#include "main.h"

/*************************
 * Interrupt handlers
 *************************/
/***************************************************************************//**
 * @brief
 *   This is a timer IRQ that interrupts ever ___ (time increment)
 *
 * @details
 * 	 times are subject to change by commands from spacecraft
 *
 * @note
 *	 
 * @param[in] variable_name
 *  
 *
 ******************************************************************************/
timer_irq_handler(){
	//When a 10s interval is reached, measure and send science packet
	if(time == 10){
		sxr_command("Take Measurements");
		hxr_command("Take Measurements");

		payload_science_send();
	}
	//When a 3s interval is reached, send HMS packet
	if(time == 3){
		payload_HK_send();
	}
	//When a 1s interval is reached
	if(time == 1){
		HMS_collect();
	}
	
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
UART_irq_handler(){
	recieve message //receive via UART
	Decode message

	if from spacecraft (RS244)
	do either
		Command then do
		//send echo command
		payload_echo_send();
		//send parameter dump
		payload_parameter_dump_send();

	if from spectrometer (UART)
		process data returned
}
/***************************************************************************//**
 * @brief
 *   Any error that occurs internally would prompt this interrecpt to send an
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
	//if error occurs, send log message via RS-422
	payload_log_msg_send()
}

main()
{
	//setup peripherals and busses
	app_peripheral_setup();
	while(1){
	}
}


/*
We want to know how to:
- read/write on RS422
- read/write on UART
- read/write on I2C
- read/write to XADC

*/