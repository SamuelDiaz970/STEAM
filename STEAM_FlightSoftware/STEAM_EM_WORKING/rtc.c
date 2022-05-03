#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>

#include "rtc.h"

static int rtc_dev_fd = -1;

static int open_rtc()
{
	if (rtc_dev_fd != -1)
		return rtc_dev_fd;
	rtc_dev_fd = open("/dev/rtc0", O_RDWR);
	return rtc_dev_fd;
}

static void close_rtc()
{
	if (rtc_dev_fd != -1)
		close(rtc_dev_fd);
	rtc_dev_fd = -1;
}

int set_rtc_time(struct tm time)
{
	int fd, rc;
	fd = open_rtc();
	if (fd == -1)
		perror("Could not open RTC device: ");

	rc = ioctl(fd, RTC_SET_TIME, &time);
	if (rc)
		fprintf(stderr, "RTC ioctl RTC_SET_TIME failed\n");

	close_rtc();
	return 0;
}

int get_rtc_time(struct tm *time)
{
	int fd, rc;
	fd = open_rtc();
	if (fd == -1)
		perror("Could not open RTC device: ");

	rc = ioctl(fd, RTC_RD_TIME, time);
	if (rc)
		fprintf(stderr, "RTC ioctl RTC_RD_TIME failed\n");

	close_rtc();
	return 0;
}
