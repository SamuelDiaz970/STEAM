/*
 * serial.h
 *
 */

#ifndef SRC_HEADER_FILES_SERIAL_H_
#define SRC_HEADER_FILES_SERIAL_H_
#include "stdint.h"

#define MAX_PACKET_SIZE 3144//1024
#define MAX_PUNCH_PACKET_SIZE 30
#define MAX_SPEC_PACKET_SIZE 3144//this is the size of the Response Packet 1024-channel spectrum plus Status


struct punch_packet {
	unsigned char *buf;
	int size;
};

struct h_packet {
	unsigned char *buf;
	int size;
};

// struct s_packet {
// 	char *buf;
// 	int size;
// };


int setup_serial_interfaces();
int close_serial_interfaces();

int h_write(struct h_packet p);
int h_read(struct h_packet *p);

int s_write(struct h_packet p);
int s_read(struct h_packet *p);

int punch_write(struct punch_packet p);
int punch_read(struct punch_packet *p);

#endif /* SRC_HEADER_FILES_SERIAL_H_ */