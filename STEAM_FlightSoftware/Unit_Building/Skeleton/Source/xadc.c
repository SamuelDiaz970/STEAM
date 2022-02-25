// insert here
#include "i2c.h"
#include <libpa3.h>//unsure if this is correct
//or
#include "pa3adc.h"

adc_config(){
    
}

//Test ASAP
//Will this give us just the 4 outputs or the 13 given by sensors
/*
Read voltage from Q7 - sensors (XADC)
Read current from Q7 - sensors (XADC)
Read temp from Q7 - sensors (XADC)// Not in the below read where from
Read ADC current to Q7 - not sensors (XADC)//Not in sensors where from
Read custom PCB temp - not sensors (XADC)//Not in sensors where from
*/
//check register offsets in vivado also try and Cat sensors to see how that is done
/***************************************************************************//**
 * @brief This reads the values of the xadc and returns them in the form of an array
 *   
 *
 * @details
 * 	 This function takes in the adc format using the libpa3 library and also 
 *   probes the adc using i2c and gets back current and voltage readings
 *	 
 * @param[in] readings
 *      This is a pointer to an array that will hold the readings data
 *  
 *
 ******************************************************************************/
xadc_read(float * readings){
char adc_fmt[4];
// *adc_fmt = get_adc_output_msg_fmt(BOARD_Q7_REVB, 4);//second variable either 4 for 4 channels or needs to be repeated 0-3 

// pa3sercomm_s adc;
// // float readings[4];
// int cmd_adc(&adc, BOARD_Q7_REVB, readings);//error check
    int board_id = BOARD_Q7_REVB;
    int rc;
	int i;
	// float readings[MAX_NB_ADC_CHANNELS];

	rc = cmd_adc(p, board_id, readings);
	if (rc) {
		return rc;
	}
	for (i = 0; get_adc_output_msg_fmt(board_id, i); i++) {
		adc_fmt[i] = get_adc_output_msg_fmt(board_id, i);
	}

	return 0;
}

/*************************************************************
 * 
 * 
 *                  XADC SECTION
 * 
 * 
 *************************************************************/

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
q7_voltage(){


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
q7_current(){


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
q7_temp(){


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
q7_adc_current(){


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
pcb_temp(){


}
/*************************************************************
 * 
 * 
 *                  ADC SECTION
 * 
 * 
 *************************************************************/

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
sc_voltage(){


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
sc_current(){


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
sc_temp(){


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
l_bracket_temp(){


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
mosfet_1_voltage(){


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
mosfet_1_current(){


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
mosfet_2_voltage(){


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
mosfet_2_current(){


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
ext_connector_temp(){


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
surf_wall_temp(){


}


