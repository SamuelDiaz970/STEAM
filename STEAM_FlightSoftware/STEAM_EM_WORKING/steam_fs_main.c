#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

/* for timers */
#include <signal.h>
#include <sys/time.h>

#include "serial.h"
#include "gpio.h"
#include "rtc.h"
#include "packet.h"
#include "spectrometer.h"
#include "punch_cmd_handler.h"
#include "main.h"

/* macros */
// #define STEAM_PACKET_SZ 	(256U)
// #define EVERY_3_SEC		    (3U)
// #define EVERY_10_SEC		(10U)
#define HXR_ON_AFTER_BOOT	(55U)
#define LOOP_SLEEP_US		(100000U) /* 10ms delay ( max. 1000ms) */

int science_pckt_rate_;
int housekeeping_pckt_rate_;
/* datastrutures */
// typedef struct steam_xr_param
// {
// 	/* time stamps of events */
// 	unsigned int time_at_boot;
// 	unsigned int time_last_hxr;
// 	unsigned int time_last_sxr;
// 	unsigned int time_last_hk;
// 	unsigned int ref_time_TAI;

// 	unsigned long long time_from_boot;

// 	/* status of spectrometer */
// 	bool hxr_detector_on;
// 	bool sxr_detector_on;

// 	bool hxr_bootup_seq_done;

// 	/* data buffers */
// 	unsigned char hk_packet[STEAM_PACKET_SZ];
// 	unsigned int hz_packet_size;
// 	bool hz_packet_avail;

// } steam_xr_param_t;

// typedef enum xr_type
// {
// 	SOFT_XR = 1,
// 	HARD_XR = 2
// }xr_type_t;


/* global varaibles */
steam_xr_param_t g_param;// = {0,0,0,0,0};

struct punch_packet steam_punch_rx_handle = { 0,};

/* function prototypes */
/* steam app APIs */
int steam_timestamp_init(void);
void steam_process_incoming_messages(void);
void steam_send_periodic_messages(void);

// int xr_detector_turn_on(xr_type_t det);
// int xr_detector_turn_off(xr_type_t det);
// bool xr_detector_get_status(xr_type_t det);

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
	
	gpio_init();
	xr_detector_turn_off(SOFT_XR);
	xr_detector_turn_off(HARD_XR);
// while(1)
// {
// // 	// /* turn off all the spectrometers at boot-up */
// 	xr_detector_turn_off(SOFT_XR);
// 	xr_detector_turn_off(HARD_XR);

// 	usleep(5000000);
// 	xr_detector_turn_on(SOFT_XR);
// 	xr_detector_turn_on(HARD_XR);
// 	usleep(5000000);
// }
	steam_timestamp_init();
	housekeeping_pckt_rate_ = get_hk_rate();
	
	science_pckt_rate_ = get_sci_rate();
	/* turn ON the soft spectrometer */
	// xr_detector_turn_on(HARD_XR);
	xr_detector_turn_on(SOFT_XR);
	
	// tvv.tv_sec = 10000;
	// tvv.tv_usec = 0;
	// // struct timezone tzz;
	// // tzz.tz_dsttime = DST_NONE;
	// // tzz.tz_minuteswest
	// settimeofday(&tvv,NULL);
		
	/* period loop */
	steam_punch_rx_handle.buf = recieve_buff;
	steam_punch_rx_handle.size = 0;
	// science_pckt_rate = 10;
	// housekeeping_pckt_rate = 3;
	while(1)
	{
		housekeeping_pckt_rate_ = get_hk_rate();
		// printf("HK rate     %d\n", housekeeping_pckt_rate_);
		science_pckt_rate_ = get_sci_rate();
		// printf("%d", housekeeping_pckt_rate_);
		// printf("%d\n",g_param.time_from_boot);
		// printf("here22");
		/* pool the UART peripherals */
		//TODO Complete the UART Rx
		punch_read(&steam_punch_rx_handle);
		
		/* process the UART data */
		steam_process_incoming_messages();
		// printf("here");
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
	gpio_deinit();

}

int xr_detector_turn_on(xr_type_t det)
{
	switch(det)
	{
		case SOFT_XR:
			gpio_config_sxr(true);
			g_param.sxr_detector_on = true;
			printf("xr_detector_turn_ON  : SOFT_XR\n");

		break;

		case HARD_XR:
			gpio_config_hxr(true);
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
			gpio_config_sxr(false);
			g_param.sxr_detector_on = false;
			printf("xr_detector_turn_OFF : SOFT_XR\n");
		break;
		case HARD_XR:
			gpio_config_hxr(false);
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

int steam_timestamp_init(void)
{
	g_param.time_at_boot = 0;
	g_param.time_last_hxr = g_param.time_at_boot;
	g_param.time_last_sxr = g_param.time_at_boot;
	g_param.time_last_hk  = g_param.time_at_boot;
}

void steam_send_periodic_messages(void)
{
	if(g_param.time_from_boot - g_param.time_last_hk >= housekeeping_pckt_rate_)
	{
		// int t = 0;
		// t = gettimeofday(&tvv,NULL);
		// printf("seconds: %d		microseconds: %d", tvv.tv_sec, tvv.tv_usec);
		payload_HK_dummy_packet();
		struct punch_packet packet;
		packet.buf = hk_buff;
		packet.size = hk_buff_length;
		punch_write(packet);
		// buffer_print_hk();
		g_param.time_last_hk = g_param.time_from_boot;
		printf("Transmitting the 3 sec message !!\n");
	}

	if((xr_detector_get_status(HARD_XR) == true) && (g_param.time_from_boot - g_param.time_last_hxr >= science_pckt_rate_))
	{
		// payload_science_dummy_packet_hr();
		payload_science_packet_hr(0);
		struct punch_packet packet;
		packet.buf = hr_buff;
		packet.size = hr_buff_length;
		// tx_rx_spectrum(0,0);
		punch_write(packet);
		// buffer_print_hr();
		printf("Requesting the 10 sec message : HXR !!\n");
		g_param.time_last_hxr = g_param.time_from_boot;
	}

	if((xr_detector_get_status(SOFT_XR) == true) && (g_param.time_from_boot - g_param.time_last_sxr >= science_pckt_rate_))
	{
		// payload_science_dummy_packet_sr();
		payload_science_packet_sr(1);
		struct punch_packet packet;
		packet.buf = sr_buff;
		packet.size = sr_buff_length;
		// echo_pckt(1);
		// tx_rx_spectrum(1,0);
		punch_write(packet);
		// buffer_print_sr();
		printf("Requesting the 10 sec message : SXR !!\n");
		g_param.time_last_sxr = g_param.time_from_boot;
	}
}


// void steam_process_incoming_messages(void)
// {
// 	/* process only if there is an incoming message */
// 	if(g_param.hz_packet_avail == true)
// 	{
// 		/* parse and perform actions */
		
// 		g_param.hz_packet_avail = false;
// 	}
// }

void steam_process_incoming_messages(void)
{
	/* process only if there is an incoming message */
	// if(g_param.hz_packet_avail == true)
	// {
	// 	/* parse and perform actions */
	// 	g_param.hz_packet_avail = false;

	// 	/* transmit the echo message back with header frame */
	// 	struct punch_packet packet;
	// 	packet.buf = hr_buff;
	// 	packet.size = ;
	// 	punch_write(packet);
	// }
	if(steam_punch_rx_handle.size != 0)
	{
		if(steam_punch_rx_handle.buf == NULL)
		{
			printf("steam_punch_rx_handle pointing NULL buffer \n");
		}
		else
		{
			/* transmit the exact packet out */
			// punch_write(steam_punch_rx_handle);
			int status = command_handle(steam_punch_rx_handle);
			// printf("Transmitting the data back!!\n");

		}
	
		steam_punch_rx_handle.size = 0;
	}
}


/* initialize a one second timer */
int timer_init(void)
{
	struct itimerval itv;
	/*  register the timer handler */
	signal(SIGALRM, timer_1sec_handle);

	/* Load automatically and then respond every 1 seconds */
	// struct itimerval itv;
	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec= 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec= 0;
	setitimer(ITIMER_REAL, &itv, NULL);
}

void timer_1sec_handle(int sig)
{
	g_param.time_from_boot++;
	printf("system_time_from_boot %llu sec\n",g_param.time_from_boot);
}

uint64_t timer_get_time_now(void)
	{
		return g_param.time_from_boot;
	}