/*
 * FIFO_accessor.c
 *
 *  Created on: Mar 30, 2022
 *      Author: Kanin
 */

// #include "FIFO_accessor.h"
#include "buffer.h"
/********************************************************************************************************
 * 										LOCAL DATA STRUCTURES
 ********************************************************************************************************/
// Head of fifo
// struct data_fifo* fifo_head_1;
// struct data_fifo* fifo_head_0;
// global char hk_buff[hk_buff_length];
// global char hr_buff[hr_buff_length];
// global char sr_buff[sr_buff_length];
/*
*********************************************************************************************************
*                                          buffer_write_1()
*
* Description : This function is called when a write into the fifo is needed. It will write the input int
*               into the FIFO buffer. There are two buffers: This one is for the HouseKeeping Data
*
* Argument(s) : int insert
*               Data to be inserted
*
* Return(s)   : None.
*
* Notes       : None.
*********************************************************************************************************
*/
int buffer_write_hk(char insert, int count){
    if(count >= hk_buff_length)return -1;
    hk_buff[count] = insert;
    return count++;
}

int buffer_write_hr(char insert, int count){
    if(count >= hr_buff_length)return -1;
    hr_buff[count] = insert;
    return count++;
}

int buffer_write_sr(char insert, int count){
    if(count >= sr_buff_length)return -1;
    sr_buff[count] = insert;
    return count++;
}

short checksum_hk(){
    unsigned short checksum_add = 0;
    for(int i = 0; i < hk_buff_length; i++){
        checksum_add += hk_buff[i];
    }
    // unsigned short checksum_return ^= checksum_add;
    return checksum_add;
}

short checksum_hr(){
    unsigned short checksum_add = 0;
    for(int i = 0; i < hr_buff_length; i++){
        checksum_add += hr_buff[i];
    }
    // unsigned short checksum_return ^= checksum_add;
    return checksum_add;
}

short checksum_sr(){
    unsigned short checksum_add = 0;
    for(int i = 0; i < sr_buff_length; i++){
        checksum_add += sr_buff[i];
    }
    // unsigned short checksum_return ^= checksum_add;
    return checksum_add;
}

void buffer_print_hk(){
    for(int i = 0; i < hk_buff_length; i++){
        printf("Byte %d, Data %d", i, hk_buff[i]);
    }
}

void buffer_print_hr(){
    for(int i = 0; i < hr_buff_length; i++){
        printf("Byte %d, Data %d", i, hr_buff[i]);
    }
}

void buffer_print_sr(){
    for(int i = 0; i < sr_buff_length; i++){
        printf("Byte %d, Data %d", i, sr_buff[i]);
    }
}

void buffer_zero_hk(){
    for(int i = 0; i < hk_buff_length; i++){
        hk_buff[i] = 0;
    }
}

void buffer_zero_hr(){
    for(int i = 0; i < hr_buff_length; i++){
        hr_buff[i] = 0;
    }
}

void buffer_zero_sr(){
    for(int i = 0; i < sr_buff_length; i++){
        sr_buff[i] = 0;
    }
}