#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

/* for timers */
#include <signal.h>
#include <sys/time.h>

#include "serial.h"


/* macros */
#define STEAM_PACKET_SZ 	(256U)
#define EVERY_3_SEC		(3U)
#define EVERY_10_SEC		(10U)
#define HXR_ON_AFTER_BOOT	(55U)
#define LOOP_SLEEP_US		(100000U) /* 10ms delay ( max. 1000ms) */


/* datastrutures */
typedef struct steam_xr_param
{
	/* time stamps of events */
	unsigned int time_at_boot;
	unsigned int time_last_hxr;
	unsigned int time_last_sxr;
	unsigned int time_last_hk;
	unsigned int ref_time_TAI;

	unsigned long long time_from_boot;

	/* status of spectrometer */
	bool hxr_detector_on;
	bool sxr_detector_on;

	bool hxr_bootup_seq_done;

	/* data buffers */
	unsigned char hk_packet[STEAM_PACKET_SZ];
	unsigned int hz_packet_size;
	bool hz_packet_avail;

} steam_xr_param_t;

typedef enum xr_type
{
	SOFT_XR = 1,
	HARD_XR
}xr_type_t;


/* global varaibles */
steam_xr_param_t g_param = {0,};

/* function prototypes */
/* steam app APIs */
int steam_timestamp_init(void);
void steam_process_incoming_messages(void);
void steam_send_periodic_messages(void);

int xr_detector_turn_on(xr_type_t det);
int xr_detector_turn_off(xr_type_t det);
bool xr_detector_get_status(xr_type_t det);

/* peripheral APIs */
int timer_init(void);
void timer_1sec_handle(int sig);

int main(void)
{
	printf("Application starts\n");
	/* do initialization */
	timer_init();

	if(setup_serial_interfaces() != 0)
	{
		printf("serial init failed!\n");
		goto error_main;
	}
	//TODO Include the IO initialization 

	/* turn off all the spectrometers at boot-up */
	xr_detector_turn_off(SOFT_XR);
	xr_detector_turn_off(HARD_XR);

	steam_timestamp_init();

	/* turn ON the soft spectrometer */
	xr_detector_turn_on(SOFT_XR);

	/* period loop */
	while(1)
	{
		/* pool the UART peripherals */
		//TODO Complete the UART Rx
		
		/* process the UART data */
		steam_process_incoming_messages();

		/* send periodics messages */
		steam_send_periodic_messages();

		/* turn ON the hard spectrometer after 55 seconds from the boot-up time */
		if(g_param.hxr_bootup_seq_done != true)
		{
			if(g_param.hxr_detector_on == false && \
				g_param.time_from_boot - g_param.time_at_boot > HXR_ON_AFTER_BOOT)
			{
				g_param.time_last_hxr = g_param.time_from_boot;
				xr_detector_turn_on(HARD_XR);
				g_param.hxr_bootup_seq_done = true;
			}
		}
		/* sleep for few ms */
		usleep(LOOP_SLEEP_US);
	}

	/* close all the peripherals and terminate during error condition */
error_main:
	printf("Application faced an error. Terminating...\n");
	close_serial_interfaces();

}


int xr_detector_turn_on(xr_type_t det)
{
	switch(det)
	{
		case SOFT_XR:
			//TODO: Perform the GPIO action
			g_param.sxr_detector_on = true;
			printf("xr_detector_turn_ON  : SOFT_XR\n");

		break;

		case HARD_XR:
			//TODO: Perform the GPIO action
			g_param.hxr_detector_on = true;
			printf("xr_detector_turn_ON  : HARD_XR\n");
		break;
	}

	/* perform a feedback check to conclude
	   the operation of spectrometer */
	   
	return 0;
}

int xr_detector_turn_off(xr_type_t det)
{
	switch(det)
	{
		case SOFT_XR:
			//TODO: Perform the GPIO action
			g_param.sxr_detector_on = false;
			printf("xr_detector_turn_OFF : SOFT_XR\n");
		break;
		case HARD_XR:
			//TODO: Perform the GPIO action
			g_param.hxr_detector_on = false;
			printf("xr_detector_turn_OFF : HARD_XR\n");
		break;
	}

	/* perform a feedback check to conclude
	   the operation of spectrometer */
	return 0;
}

bool xr_detector_get_status(xr_type_t det)
{
	switch(det)
	{
		case SOFT_XR:
			return g_param.sxr_detector_on;
		break;
		case HARD_XR:
			return g_param.hxr_detector_on;
		break;
	}
}


/* initialize a one second timer */
int timer_init(void)
{
	/*  register the timer handler */
	signal(SIGALRM, timer_1sec_handle);

	/* Load automatically and then respond every 1 seconds */
	struct itimerval itv;
	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec= 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec= 0;
	setitimer(ITIMER_REAL, &itv, NULL);
}

void timer_1sec_handle(int sig)
{
	g_param.time_from_boot++;
	printf("handle count is %llu\n",g_param.time_from_boot);
}

int steam_timestamp_init(void)
{
	g_param.time_at_boot = 0;
	g_param.time_last_hxr = g_param.time_at_boot;
	g_param.time_last_sxr = g_param.time_at_boot;
	g_param.time_last_hk  = g_param.time_at_boot;
}

void steam_send_periodic_messages(void)
{
	if(g_param.time_from_boot - g_param.time_last_hk >= EVERY_3_SEC)
	{
		g_param.time_last_hk = g_param.time_from_boot;
		printf("Transmitting the 3 sec message !!\n");
	}

	if((g_param.hxr_detector_on == true) && (g_param.time_from_boot - g_param.time_last_hxr >= EVERY_10_SEC))
	{
		printf("Requesting the 10 sec message : HXR !!\n");
		g_param.time_last_hxr = g_param.time_from_boot;
	}

	if((g_param.sxr_detector_on == true) && (g_param.time_from_boot - g_param.time_last_sxr >= EVERY_10_SEC))
	{
		printf("Requesting the 10 sec message : SXR !!\n");
		g_param.time_last_sxr = g_param.time_from_boot;
	}
}


void steam_process_incoming_messages(void)
{
	/* process only if there is an incoming message */
	if(g_param.hz_packet_avail == true)
	{
		/* parse and perform actions */

		g_param.hz_packet_avail = false;
	}
}
