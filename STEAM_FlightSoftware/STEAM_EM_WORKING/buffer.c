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
/**
 * It takes a character and an integer and writes the character to a buffer at the index of the integer
 * 
 * @param insert the character to be inserted into the buffer
 * @param count the number of bytes written to the buffer
 * 
 * @return The count is being returned.
 */
int buffer_write_hk(char insert, int count){
    // printf("insert %u   %d", insert,count);
    if(count >= hk_buff_length)return -1;
    hk_buff[count] = (unsigned char)insert;
    count++;
    // printf("count %d", count);
    return count;
}

/**
 * This function writes a character to a buffer and returns the new buffer length.
 * 
 * @param insert The character to insert into the buffer
 * @param count The current count of the buffer.
 * 
 * @return The count is being returned.
 */
int buffer_write_hr(char insert, int count){
    if(count >= hr_buff_length)return -1;
    hr_buff[count] = (unsigned char)insert;
    count++;
    return count;
}

/**
 * This function writes a single character to a buffer, and returns the new buffer length.
 * 
 * @param insert The character to insert into the buffer
 * @param count The current position in the buffer
 * 
 * @return The count is being returned.
 */
int buffer_write_sr(char insert, int count){
    if(count >= sr_buff_length)return -1;
    sr_buff[count] = (unsigned char)insert;
    count++;
    return count;
}

/**
 * It takes a buffer of bytes, adds them together, and returns the sum.
 * 
 * @return The checksum of the packet.
 */
short checksum_hk(){
    unsigned short checksum_add = 0;
    for(int i = 0; i < hk_buff_length; i++){
        // printf("\n  %x  \n", hk_buff[i]);
        // printf("\ncheck  %d  \n", checksum_add);
        checksum_add += hk_buff[i];
    }
    // buffer_write_hk(,packet_tail_location);
    // unsigned short checksum_return ^= checksum_add;
    return checksum_add;
}

/**
 * It takes a buffer of bytes, adds them up, and returns the sum
 * 
 * @return The checksum of the data in the buffer.
 */
short checksum_hr(){
    unsigned short checksum_add = 0;
    for(int i = 0; i < hr_buff_length; i++){
        // printf("\n  %x  \n", hr_buff[i]);
        // printf("\ncheck  %d  \n", checksum_add);
        checksum_add += hr_buff[i];
    }
    // unsigned short checksum_return ^= checksum_add;
    return checksum_add;
}

/**
 * The function checksum_sr() takes the sum of all the bytes in the array sr_buff[], and returns the
 * sum.
 * 
 * @return The checksum of the sr_buff array.
 */
short checksum_sr(){
    unsigned short checksum_add = 0;
    for(int i = 0; i < sr_buff_length; i++){
        checksum_add += sr_buff[i];
    }
    // unsigned short checksum_return ^= checksum_add;
    return checksum_add;
}

/**
 * It prints the contents of the hk_buff array.
 */
void buffer_print_hk(){
    for(int i = 0; i < hk_buff_length; i++){
        printf("Byte %d, Data %d", i, hk_buff[i]);
    }
}

/**
 * This function takes a pointer to a buffer, a pointer to a buffer length, and a pointer to a buffer
 * index, and then it prints the buffer.
 */
void buffer_print_hr(){
    for(int i = 0; i < hr_buff_length; i++){
        printf("Byte %d, Data %d", i, hr_buff[i]);
    }
}

/**
 * It prints the contents of the buffer.
 */
void buffer_print_sr(){
    for(int i = 0; i < sr_buff_length; i++){
        printf("Byte %d, Data %d", i, sr_buff[i]);
    }
}

/**
 * It zeros out the hk_buff array.
 */
void buffer_zero_hk(){
    for(int i = 0; i < hk_buff_length; i++){
        hk_buff[i] = 0;
    }
}

/**
 * This function takes a pointer to an array of integers, and sets all the values in the array to zero.
 */
void buffer_zero_hr(){
    for(int i = 0; i < hr_buff_length; i++){
        hr_buff[i] = 0;
    }
}

/**
 * This function takes a pointer to a buffer, and a length, and zeros out the buffer.
 */
void buffer_zero_sr(){
    for(int i = 0; i < sr_buff_length; i++){
        sr_buff[i] = 0;
    }
}

/**
 * This function takes a pointer to a character array and a length, and sets all the elements of the
 * array to zero.
 * 
 * @param buffer_print The buffer that will be printed to the screen
 * @param length the length of the buffer
 */
void buffer_zero(char* buffer_print, int length){
    for(int i = 0; i < length; i++){
        buffer_print[i] = 0;
    }
}

/**
 * It takes a pointer to a buffer and the length of the buffer and prints the contents of the buffer
 * 
 * @param buffer_print The buffer to print
 * @param length The length of the buffer
 */
void buffer_print(char* buffer_print, int length){
    for(int i = 0; i < length; i++){
        printf("Byte %d, Data %d    ", i, buffer_print[i]);
    }
}