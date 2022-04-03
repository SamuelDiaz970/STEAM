#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "serial.h"

static int h_dev_fd = -1;
static int s_dev_fd = -1;
static int punch_dev_fd = -1;

static int set_interface_attribs(int fd, int speed, int parity)
{
	struct termios tty;
	if (tcgetattr (fd, &tty) != 0) {
		fprintf(stderr, "error %d from tcgetattr", errno);
		return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	//disable IGNBRK for mismatched speed tests; otherwise receive break as \000 chars
	tty.c_iflag &= ~IGNBRK;			// disable break processing
	tty.c_lflag = 0;                	// no signaling chars, echo, canonical processing
	tty.c_oflag = 0;                	// no remapping, no delays
	tty.c_cc[VMIN]  = 0;            	// read doesn't block
	tty.c_cc[VTIME] = 5;            	// 0.5 seconds read timeout
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
	tty.c_cflag |= (CLOCAL | CREAD);	// ignore modem controls,
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;
	if (tcsetattr (fd, TCSANOW, &tty) != 0) {
		fprintf(stderr, "error %d from tcsetattr", errno);
		return -1;
	}

	return 0;
}

static void set_blocking(int fd, int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0) {
		fprintf(stderr, "error %d from tggetattr", errno);
		return;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;            // 5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
		fprintf(stderr, "error %d setting term attributes", errno);
}

static int getbyte(int fd, char *c)
{
	int n;
	n = read(fd, c, 1);
	if (n < 1)
		return -1;
	return 0;
}

static int serial_write()
{
	return 0;
}

static int open_dev(const char* name)
{
	int fd = open(name, O_RDWR | O_SYNC);
	if (fd == -1) {
		fprintf(stderr, "could not open device %s\n", name);
		return fd;
	}
}

int setup_serial_interfaces()
{
	int rc , err;
	s_dev_fd = open_dev("/dev/ttyS2");
	h_dev_fd = open_dev("/dev/ttyS3");
	punch_dev_fd = open_dev("/dev/ttyPS1");
	if (s_dev_fd > -1) {
		fprintf(stdout, "configuring soft spectrometer serial device\n");
		err = set_interface_attribs(s_dev_fd, B115200, 0);
		if (err) {
			fprintf(stderr, "failed to configure soft spectrometer serial device\n");
			rc = -1;
		} else {
			set_blocking(s_dev_fd, 0);
		}
	}
	else
	{
		rc = -1;
	}
	if (h_dev_fd > -1) {
		fprintf(stdout, "configuring hard spectrometer serial device\n");
		err = set_interface_attribs(h_dev_fd, B115200, 0);
		if (err) {
			fprintf(stderr, "failed to configure hard spectrometer serial device\n");
			rc = -1;
		} else {
			set_blocking(h_dev_fd, 0);
		}
	}
	else
	{
		rc = -1;
	}
	if (punch_dev_fd > -1) {
		fprintf(stdout, "configuring PUNCH serial device\n");
		err = set_interface_attribs(punch_dev_fd, B115200, 0);
		if (err) {
			fprintf(stderr, "failed to configure PUNCH serial device\n");
			rc = -1;
		} else {
			set_blocking(punch_dev_fd, 1); // blocking main thread risk
		}
	}
	else
	{
		rc = -1;
	}

	return rc;
}

int close_serial_interfaces()
{
	if (s_dev_fd != -1) {
		close(s_dev_fd);
		s_dev_fd = -1;
	}
	if (h_dev_fd != -1) {
		close(h_dev_fd);
		h_dev_fd = -1;
	}
	if (punch_dev_fd != -1) {
		close(punch_dev_fd);
		punch_dev_fd = -1;
	}
	return 0;
}

int punch_write(struct punch_packet p)
{
	if (punch_dev_fd == -1)
		return -1;

	return write(punch_dev_fd, p.buf, p.size);
}

int punch_read(struct punch_packet *p)
{
	if (punch_dev_fd == -1)
		return -1;

	char buf[MAX_PUNCH_PACKET_SIZE];
	int length = 0;
	int rc;
	char c;

	while (1) {
		rc = getbyte(punch_dev_fd, &c);
		if (rc == -1)
			return rc;
		if (c == 0x61)	// wait for start byte, modify according to protocol
			break;
	}
	while (1) {
		rc = getbyte(punch_dev_fd, &c);
		if (rc == -1)
			return rc;
		if (c == 0x62)  // wait for end byte, modify according to protocol
			break;
		buf[length] = c;
		length++;
		if (length >= MAX_PACKET_SIZE) {
			fprintf(stderr, "missed end of PUNCH packet, buffer is full\n");
			return -1;
		}
	}
	p->buf = buf;
	p->size = length;
	return 0;
}

int h_write(struct h_packet p)
{
        if (h_dev_fd == -1)
                return -1;

        return write(h_dev_fd, p.buf, p.size);
}

int h_read(struct h_packet *p)
{
	if (h_dev_fd == -1)
		return -1;

	char buf[MAX_SPEC_PACKET_SIZE];
	int length = 0;
	int data_len = 0;
	int rc;
	char c;

	while (1) {
		rc = getbyte(h_dev_fd, &c);
		if (rc == -1)
			return rc;
		if (c == 0xf5){  // wait for start byte, modify according to protocol
			length++;
			break;
		}
	}
	while (1) {
		rc = getbyte(h_dev_fd, &c);
		if (rc == -1)
			return rc;
		
		// set length of data in bytes
		if (length == 4)
			data_len = c << 8;
		if (length == 5)
			data_len |= c;

		// at last byte, break
		if (length == 7+data_len) {
			buf[length] = c;
			break;
		}

		buf[length] = c;
		length++;
		if (length >= MAX_SPEC_PACKET_SIZE) {
			fprintf(stderr, "missed end of HXR packet, buffer is full\n");
			return -1;
		}
	}
	p->buf = buf;
	p->size = length;

	return 0;
}

int s_write(struct s_packet p)
{
        if (s_dev_fd == -1)
                return -1;

        return write(s_dev_fd, p.buf, p.size);
}

int s_read(struct s_packet *p)
{
	if (s_dev_fd == -1)
                return -1;

	char buf[MAX_SPEC_PACKET_SIZE];
	int length = 0;
	int data_len = 0;
	int rc;
	char c;

	while (1) {
		rc = getbyte(s_dev_fd, &c);
		// printf("here");
		if (rc == -1)
			return rc;
		// printf("here");
		if (c == 0xf5){  // wait for start byte, modify according to protocol
			printf("found f5: %x",c);
			buf[length] = c;
			printf("Inside buffer: %x", buf[length]);
			length++;
			break;
		}
	}
	while (1) {
		printf("here");
		rc = getbyte(s_dev_fd, &c);
		if (rc == -1)
			return rc;

		// set length of data in bytes
		if (length == 4)
			data_len = c << 8;
		if (length == 5)
			data_len |= c;

		// at last byte, break
		if (length == 7+data_len) {
			buf[length] = c;
			break;
		}

		buf[length] = c;
		length++;
		if (length >= MAX_SPEC_PACKET_SIZE) {
			fprintf(stderr, "missed end of SXR packet, buffer is full\n");
			return -1;
		}
	}
	
	p->buf = buf;
	printf("point buff: %x\n", p->buf[0]);
	printf("point buff: %x\n", p->buf[1]);
	p->size = length;

	return 0;
	// see punch_read
}