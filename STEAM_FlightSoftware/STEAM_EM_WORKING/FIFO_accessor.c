/*
 * FIFO_accessor.c
 *
 *  Created on: Mar 30, 2022
 *      Author: Kanin
 */

#include "FIFO_accessor.h"

/********************************************************************************************************
 * 										LOCAL DATA STRUCTURES
 ********************************************************************************************************/
// Head of fifo
struct data_fifo* fifo_head_1;
struct data_fifo* fifo_head_0;

/*
*********************************************************************************************************
*                                          fifo_write_1()
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
void fifo_write_1(char insert){
	struct data_fifo* new_data = (struct data_fifo*)malloc(sizeof(struct data_fifo));
	new_data->data = insert;
	new_data->next = NULL;

	if(fifo_head_1 == NULL){//add to current list(empty)
		fifo_head_1 = new_data;
		return;
	}
	struct data_fifo* current = fifo_head_1;
	while(current->next != NULL) current = current->next;
	current->next = new_data;//add to end of the list
}

/*
*********************************************************************************************************
*                                          fifo_write_0()
*
* Description : This function is called when a write into the fifo is needed. It will write the input int
*               into the FIFO buffer. There are two buffers: This one is for the Spectrometer Data
*
* Argument(s) : int insert
*               Data to be inserted
*
* Return(s)   : None.
*
* Notes       : None.
*********************************************************************************************************
*/
void fifo_write_0(char insert){
	struct data_fifo* new_data = (struct data_fifo*)malloc(sizeof(struct data_fifo));
	new_data->data = insert;
	new_data->next = NULL;

	if(fifo_head_0 == NULL){//add to current list(empty)
		fifo_head_0 = new_data;
		return;
	}
	struct data_fifo* current = fifo_head_0;
	while(current->next != NULL) current = current->next;
	current->next = new_data;//add to end of the list
}

/*
*********************************************************************************************************
*                                          fifo_read_1()
*
* Description : 
*
* Argument(s) : None.
*
* Return(s)   : The integer value stored in the buffer.
*
* Notes       : None.
*********************************************************************************************************
*/

char fifo_read_1(void){
	if(fifo_head_1 == NULL) return -1;
	struct data_fifo* data_temp = fifo_head_1;
	fifo_head_1 = fifo_head_1->next;
	int temp = data_temp->data;
    free(data_temp);
    return temp;
}

/*
*********************************************************************************************************
*                                          fifo_read_0()
*
* Description : 
*
* Argument(s) : None.
*
* Return(s)   : The integer value stored in the buffer.
*
* Notes       : None.
*********************************************************************************************************
*/

char fifo_read_0(void){
	if(fifo_head_0 == NULL) return -1;
	struct data_fifo* data_temp = fifo_head_0;
	fifo_head_0 = fifo_head_0->next;
	char temp = data_temp->data;
    free(data_temp);
    return temp;
}


/*
*********************************************************************************************************
*                                          fifo_read_1_checksum()
*
* Description : 
*
* Argument(s) : None.
*
* Return(s)   : The integer value stored in the buffer.
*_0_checksum
* Notes       : None.
*********************************************************************************************************
*/

char fifo_read_1_checksum(void){
	if(fifo_head_1 == NULL) return 0;
    int count = 0;
    int partial_sum_1 = 0;
    int partial_sum_2 = 0;
    
    struct data_fifo* temp_head = fifo_head_1;
	while(temp_head->next != NULL){
        partial_sum_1 += temp_head->data;
        partial_sum_2 += partial_sum_1;
        temp_head = temp_head->next;
        count++;
    }
    partial_sum_1 += temp_head->data;
    partial_sum_2 += partial_sum_1;
    count++;

    if(count != 250){
        return -1;
    }
    partial_sum_1 = partial_sum_1 % 256;
    partial_sum_2 = partial_sum_2 % 256;
    fifo_write_0(partial_sum_1);
    fifo_write_0(partial_sum_2);
    return 1;
}

/*
*********************************************************************************************************
*                                          fifo_read_0_checksum()
*
* Description : 
*
* Argument(s) : None.
*
* Return(s)   : The integer value stored in the buffer.
*
* Notes       : None.
*********************************************************************************************************
*/

char fifo_read_0_checksum(void){
    if(fifo_head_0 == NULL) return 0;
    int count = 0;
    int partial_sum_1 = 0;
    int partial_sum_2 = 0;
    
    struct data_fifo* temp_head = fifo_head_0;
	while(temp_head->next != NULL){
        partial_sum_1 += temp_head->data;
        partial_sum_2 += partial_sum_1;
        temp_head = temp_head->next;
        count++;
    }
    partial_sum_1 += temp_head->data;
    partial_sum_2 += partial_sum_1;
    count++;
 
    if(count != 3122){
        return -1;
    }
    partial_sum_1 = partial_sum_1 % 256;
    partial_sum_2 = partial_sum_2 % 256;
    fifo_write_0(partial_sum_1);
    fifo_write_0(partial_sum_2);
    return 1;
}




void gpio_test_steam(void){
    struct gpiod_chip *chip;
	struct gpiod_line *line;
	int req, value;

	chip = gpiod_chip_open("/dev/gpiochip0");
	if (!chip)
		return -1;

	line = gpiod_chip_get_line(chip, 3);
	if (!line) {
		gpiod_chip_close(chip);
		return -1;
	}

	req = gpiod_line_request_input(line, "gpio_state");
	if (req) {
		gpiod_chip_close(chip);
		return -1;
	}

	value = gpiod_line_get_value(line);

	printf("GPIO value is: %d\n", value);
	gpiod_chip_close(chip);
}