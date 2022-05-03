
/*
 * buffer.h
 *
 *  Created on: Mar 30, 2022
 *      Author: Kanin
 */

#ifndef SRC_HEADER_FILES_BUFFER_H_
#define SRC_HEADER_FILES_BUFFER_H_

#include <stdio.h>
#include <stdlib.h>

#define hk_buff_length 256
#define hr_buff_length 3128
#define sr_buff_length 3128

// char send_buff[hk_buff_length];
#define rx_buff_length 3144
unsigned char recieve_buff[rx_buff_length];//for science for now

char hk_buff[hk_buff_length];
char hr_buff[hr_buff_length];
char sr_buff[sr_buff_length];

int buffer_write_hk(char insert, int count);
int buffer_write_hr(char insert, int count);
int buffer_write_sr(char insert, int count);
short checksum_hk();
short checksum_hr();
short checksum_sr();
void buffer_print_hk();
void buffer_print_hr();
void buffer_print_sr();
void buffer_zero_hk();
void buffer_zero_hr();
void buffer_zero_sr();
void buffer_print(char* buffer_print, int length);
void buffer_zero(char* buffer_print, int length);
#endif /* SRC_HEADER_FILES_BUFFER_H_ */
