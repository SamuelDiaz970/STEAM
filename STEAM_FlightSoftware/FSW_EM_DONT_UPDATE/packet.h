/*
 *  packet.h
 *
 *  Created on: Mar 30, 2022
 *      Author: Kanin
 */


#ifndef SRC_HEADER_FILES_PACKET_H_
#define SRC_HEADER_FILES_PACKET_H_
#include "buffer.h"
// global char hk_buff[hk_buff_length];
// global char hr_buff[hr_buff_length];
// global char sr_buff[sr_buff_length];


void packet_head(int buff, int SHDR, int APID, int SEQ, int LEN);
int ShortToChar(char buff, int length, short var);
int IntToChar(char buff, int length, int var);
void payload_science_dummy_packet_hr();
void payload_science_dummy_packet_sr();
void payload_HK_dummy_packet();

#endif /* SRC_HEADER_FILES_PACKET_H_ */