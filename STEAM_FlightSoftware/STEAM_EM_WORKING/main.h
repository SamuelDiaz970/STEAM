

#ifndef SRC_HEADER_FILES_MAIN_H_
#define SRC_HEADER_FILES_MAIN_H_

#include <stdint.h>
#include <stdbool.h>
#define STEAM_PACKET_SZ 	(256U)
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
	HARD_XR = 2
}xr_type_t;

uint64_t timer_get_time_now(void);
int xr_detector_turn_on(xr_type_t det);
int xr_detector_turn_off(xr_type_t det);
bool xr_detector_get_status(xr_type_t det);
// ssteam_xr_param_t g_param;// = {0,0,0,0,0};
#endif