/**
 * @file scheduler.c
 * @author Kanin McGuire
 * @date September 27th, 2020
 * @brief Contains all the scheduler functions
 *
 */



//***********************************************************************************
// Include files
//***********************************************************************************
#include "scheduler.h"
//** Standard Libraries

//** Silicon Lab include files

//** User/developer include files


//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// Private variables
//***********************************************************************************
static unsigned int event_scheduled;

//***********************************************************************************
// Private functions
//***********************************************************************************


//***********************************************************************************
// Global functions
//***********************************************************************************
/**************************************************************************
 * @file scheduler.c
 ****************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ****************************************************************************
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchant ability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 ***************************************************************************/

/***************************************************************************//**
 * @brief
 *	Set event_scheduled variable to zero
 *
 * @details
 *	Disables IRQ's if IRQ's are enabled. Sets event_scheduled variable to zero. Enables IRQ if they got disabled in our function.
 *
 * @note
 *	Interrupt status is the same as when it entered when it exits.
 *	Function called once to set initialize the scheduler
 *
 *
 *
 ******************************************************************************/

void scheduler_open(void){
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_CRITICAL();
	event_scheduled = 0;
	CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief
 *	Adds an event to the scheduler
 *
 * @details
 *	Disables IRQ's if IRQ's are enabled. Or's a new event into private variable event_sheduled. Enables IRQ if they got disabled in our function.
 *
 * @note
 *	Interrupt status is the same as when it entered when it exits.
 *
 * @param[in] event
 *	Event being added to the scheduler
 *
 *
 ******************************************************************************/

void add_scheduled_event(uint32_t event){
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_CRITICAL();
	event_scheduled |= event;
	CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief
 *	Removes an event to the scheduler
 *
 * @details
 *	Disables IRQ's if IRQ's are enabled. Uses &= ~ to remove an event from private variable event_sheduled. Enables IRQ if they got disabled in our function.
 *
 * @note
 *	Interrupt status is the same as when it entered when it exits.
 *
 * @param[in] event
 *	Event being removed from the scheduler
 *
 *
 ******************************************************************************/

void remove_scheduled_event(uint32_t event){
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_CRITICAL();
	event_scheduled &= ~event;
	CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief
 *	Gets the scheduled events.
 *
 * @details
 *	This function returns the private variable event_scheduled
 *
 * @note
 *	Called to check what events are scheduled
 *
 * @return
 * 	returns the private variable event_scheduled
 *
 *
 *
 ******************************************************************************/

uint32_t get_scheduled_events(void){
	//CORE_DECLARE_IRQ_STATE;
	//CORE_ENTER_CRITICAL();
	return event_scheduled;
	//CORE_EXIT_CRITICAL();
}
