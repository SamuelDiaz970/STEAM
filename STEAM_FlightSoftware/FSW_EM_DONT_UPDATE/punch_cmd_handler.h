/*
 * buffer.h
 *
 *  Created on: Mar 30, 2022
 *      Author: Kanin
 */

#ifndef SRC_HEADER_FILES_PUNCH_CMD_HANDLER_H_
#define SRC_HEADER_FILES_PUNCH_CMD_HANDLER_H_

#include "buffer.h"
#include <sys/time.h>

struct itimerval itv;

void command_handle(char *command_buf);
void noop();
void set_steam_time(char *buffer);
#endif /* SRC_HEADER_FILES_PUNCH_CMD_HANDLER_H_ */