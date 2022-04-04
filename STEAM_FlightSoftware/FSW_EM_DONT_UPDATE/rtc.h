/*
 * RTC.h
 *
 */

#ifndef SRC_HEADER_FILES_RTC_H_
#define SRC_HEADER_FILES_RTC_H_

#include <time.h>

int set_rtc_time();
int get_rtc_time(struct tm *tm);
#endif /* SRC_HEADER_FILES_RTC_H_ */