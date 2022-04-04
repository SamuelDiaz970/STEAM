/*
 * FIFO_accessor.h
 *
 *  Created on: Mar 30, 2022
 *      Author: Kanin
 */

#ifndef SRC_HEADER_FILES_FIFO_ACCESSOR_H_
#define SRC_HEADER_FILES_FIFO_ACCESSOR_H_

#include <stdio.h>
#include <stdlib.h>
// #include <gpiod.h>

//Data Event FIFO
struct data_fifo {
	int data;
	// pointer to next event
	struct data_fifo* next;
};

void fifo_write_1(char insert);
void fifo_write_0(char insert);
char fifo_read_1(void);
char fifo_read_0(void);
char fifo_read_1_checksum(void);
char fifo_read_1_checksum(void);

#endif /* SRC_HEADER_FILES_FIFO_ACCESSOR_H_ */
