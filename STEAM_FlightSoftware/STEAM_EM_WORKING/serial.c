#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "serial.h"
#include "main.h"
#include <stdint.h>
#include "punch_cmd_handler.h"
#define LOOP_TIMEOUT (2) // 2 sec

typedef enum punch_uart_fsm
{
	RX_WAIT_SOF = 0,
	RX_PAYLOAD,
	RX_EOF
}punch_uart_fsm_t;

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
			set_blocking(punch_dev_fd, 0); // blocking main thread risk
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
	printf("here2\n");
	if (punch_dev_fd == -1)
		return -1;

	// int rc = 0;
	// char rec_byte = 0;
	// int rx_length = 0;
	// unsigned short tot_length = 0;
	// uint64_t start_time = timer_get_time_now();
	// printf("%d", start_time);
	// uint8_t addon = 0;
	// char c = 0;
	// static punch_uart_fsm_t state = RX_WAIT_SOF;
	// do {
	// 	rc = getbyte(punch_dev_fd, &c);
	// 	if(rc == -1)
	// 		return rc;
	// 	// printf("	%d	",timer_get_time_now());
	// 	switch(state)
	// 	{
	// 		/* 0 - 2 | 3 | 4 | 5 - 15 = 2  bytes */
	// 		/* 0 - 1 | 2 - 15 = 2byytes */
	// 		/* 0 - 15 = 2bytes */
	// 		case RX_WAIT_SOF:
	// 		{
	// 			printf("Received: 0x%x | %c\n", c, c);
	// 			// p->buf[rx_length++] = c;
	// 			if(rx_length == 6) /* once you receive the length */
	// 			{
	// 				tot_length = p->buf[4]<<8 | p->buf[5];
	// 				tot_length +=7;
	// 				// if(p->buf[0] & 0b10000)
	// 				// {
	// 				// 	addon = 4;
	// 				// }
	// 				// else
	// 				// {
	// 				// 	addon = 2;
	// 				// }
	// 				state = RX_PAYLOAD;
	// 			}
	// 		}
	// 		break;
	// 		case RX_PAYLOAD:
	// 		{
	// 			if(rx_length < tot_length)//(p->buf[4]<<8 | p->buf[5]  + addon))//*(uint16_t *)&
	// 			{
	// 				p->buf[rx_length++] = c;
	// 			}
	// 			else
	// 			{
	// 				state = RX_EOF;
	// 			}
	// 		}
	// 		break;
			
	// 		case RX_EOF:
	// 		{

	// 		}
	// 		break;
	// 	}

	// 	if(state == RX_EOF)
	// 	{
	// 		break;
	// 	}
	// 	// printf("%d\n",(int64_t)(timer_get_time_now()));
	// 	// printf("%d\n",start_time);
	// }
	// while((int64_t)(timer_get_time_now() - start_time) < LOOP_TIMEOUT); /* loop until timeout */

	// /* check for timer elapse */
	// if(state != RX_EOF)
	// {
	// 	printf("err fetching the packet \n");
	// 	//p->size = 0; /* error fetching the packet */
	// }
	
	// p->size = rx_length;
	// state = RX_WAIT_SOF;
	// return 0;


	unsigned char buf[MAX_PUNCH_PACKET_SIZE];
	buffer_zero(p->buf, rx_buff_length);
	int length = 0;
	int pckt_length = 0;
	int rc = 0;
	char c = 0;

	// 001
	// 010
	// 000
	// 0b00010
	// int err = 1;
    // err = gettimeofday(&tvv,NULL);
    // if(err == -1) return 3;
	// int start = tvv.tv_sec;
	// printf("%d", start);
	while (1) {
		// int err = 1;
    	// err = gettimeofday(&tvv,NULL);
    	// if(err == -1) return 3;
		// printf("			%d			\n", tvv.tv_sec - start);
		// if(LOOP_TIMEOUT < tvv.tv_sec - start){
		// 	printf("here2222\n");
		// 	return -1;

		// }
		// printf("here2222\n");
		rc = getbyte(punch_dev_fd, &c);
		// printf("here2222\n");
		if (rc == -1)
			return rc;
		// printf("here2222\n");
		if (c == 0x10){	// wait for start byte, modify according to protocol
			p->buf[length] = c;
			length++;
			break;
		}
		// printf("here2222\n");
	}
	while (1) {
		if (length == 6){
			printf("5  %d\n", p->buf[5]);
			printf("4  %d\n", p->buf[4]);
			pckt_length = (p->buf[4] << 8 | p->buf[5]) + 7;
			printf("pckt len  %d", pckt_length);
		}
		rc = getbyte(punch_dev_fd, &c);
		if (rc == -1)
			return rc;
		// if (c == 0x62)  // wait for end byte, modify according to protocol
		// 	break;
		p->buf[length] = c;
		length++;
		printf("____length %d\n", length);
		if ((length >= (pckt_length)) && (pckt_length != 0)){
			printf("BREAK\n");
			break;
		}
		if (length > MAX_PACKET_SIZE) {
			fprintf(stderr, "missed end of PUNCH packet, buffer is full\n");
			return -1;
		}
		
	}
	while(getbyte(punch_dev_fd, &c) != -1);
	// buffer_print(buf,length);
	buffer_print(p->buf,length);
	// p->buf = buf;
	p->size = length;
	printf("length %d\n", length);
	// buffer_print(buf,length);
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

int s_write(struct h_packet p)
{
        if (s_dev_fd == -1)
                return -1;

        return write(s_dev_fd, p.buf, p.size);
}

int s_read(struct h_packet *p)
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
		// printf("here");
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