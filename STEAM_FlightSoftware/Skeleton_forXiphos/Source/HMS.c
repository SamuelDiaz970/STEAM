/***************************************************************************//**
 * @brief
 *   
 *
 * @details Get the HMS data using I2C, UART, and XADC
 * 	 
 *
 * @note
 *	 
 * @param[in] variable_name
 *  
 *
 ******************************************************************************/
HMS_collect(){
    //Get Thermistor, voltage sensor, current sensor values
    i2c_read(ADC);
    
    //Get health from both spectrometers
    sxr_command("Get Health");
    hxr_command("Get Health");

    //Get health from the Q7
    xadc_read();
}










