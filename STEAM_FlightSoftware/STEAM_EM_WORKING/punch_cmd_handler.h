/*
 * buffer.h
 *
 *  Created on: Mar 30, 2022
 *      Author: Kanin
 */

#ifndef SRC_HEADER_FILES_PUNCH_CMD_HANDLER_H_
#define SRC_HEADER_FILES_PUNCH_CMD_HANDLER_H_

#include "buffer.h"
#include "serial.h"
#include <sys/time.h>
#include "gpio.h"
#include <stdbool.h>
#include "main.h"

struct timeval tvv;


int get_hk_rate();
int get_sci_rate();
int command_handle(struct punch_packet command_buf);
int noop();
void echo_packet(struct punch_packet p, int status);
int set_steam_time(struct punch_packet command_buf);
int Set_HK_Packet_Rate(struct punch_packet p);
int Set_SCI_Packet_Rate(struct punch_packet p);
int Switch_Power_X123(struct punch_packet p);

int get_cmd_accept();
int get_cmd_reject();
int get_last_opcode();
#endif /* SRC_HEADER_FILES_PUNCH_CMD_HANDLER_H_ */