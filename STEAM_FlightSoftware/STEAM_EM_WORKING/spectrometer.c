
#include "spectrometer.h"
char hr_spec_response_buffer[rx_buff_length];
char sr_spec_response_buffer[rx_buff_length];

/**
 * It takes a packet, sends it to the spectrometer, and then reads the response into the same packet
 * 
 * @param spectrometer 0 for hardware, 1 for software
 * @param type 0 = 1024-channel spectrum
 * @param pckt a pointer to a struct h_packet
 * 
 * @return a pointer to a struct.
 */
int tx_rx_spectrum(bool spectrometer, char type, struct h_packet *pckt){
    char buffer[8];
    int response_length = 0;
    // “1024-channel spectrum”
    if(type == 0){
        char buffer[8] = {0xF5,0xFA, 2, 1, 0, 0, 0xFE, 0x0E};
        // for(int i = 0; i < 8; i++){
        //     printf("%d\n", buffer[i]);
        // }
        // return;
        response_length = 3080;
    }
    // “1024-channel spectrum plus clear spectrum”
    else if(type == 1){
        char buffer[8] = {0xF5,0xFA, 2, 2, 0, 0, 0xFE, 0x0D};
        response_length = 3080;
    }
    // “1024-channel spectrum plus Status”
    else if(type == 2){
        char buffer[8] = {0xF5,0xFA, 2, 3, 0, 0, 0xFE, 0x0C};
        response_length = 3144;
    }
    // “1024-channel spectrum plus Status and clear”
    else if(type == 3){
        char buffer[8] = {0xF5,0xFA, 2, 4, 0, 0, 0xFE, 0x0B};
        response_length = 3144;
    }
    else{
        printf("Incorrect Packet Type Requested");
        return -1;
    }
    // int checksum = 0;
    // for(int k = 0; k<7; k++){
    //     checksum += buffer[k];
    // }
    // checksum = -checksum;
    // buffer[7] = checksum >> 8;
    // buffer[8] = checksum;

    pckt->buf = buffer;
    pckt->size = 8;
    if(spectrometer == 1){
        // struct s_packet soft;
        s_write(*pckt);
        // pckt->buf = sr_spec_response_buffer;
        // pckt->size = response_length;
        s_read(pckt);
    }
    if(spectrometer == 0){
        // struct h_packet hard;
        // hard.buf = buffer;
        // hard.size = 8;
        h_write(*pckt);
        // pckt->buf = hr_spec_response_buffer;
        // pckt->size = response_length;
        h_read(pckt);
    }
    if(pckt->buf[2] == 0xFF) {
        printf("SPECTROMETER ERROR");
        return -1;
    }
    return 0;
}


/**
 * This function sends a packet to the spectrometer and then reads the response. 
 * 
 * The function takes a single argument, a boolean value that determines whether the function is
 * communicating with the spectrometer through the hard or soft serial port. 
 * 
 * The function first creates a packet to send to the spectrometer. The packet is a 9 byte array. The
 * first 8 bytes are the packet header and the last byte is the checksum. The checksum is calculated by
 * adding up the first 8 bytes and then taking the two's complement of the sum. 
 * 
 * The function then sends the packet to the spectrometer. If the spectrometer is connected to the hard
 * serial port, the function calls the h_write function. If the spectrometer is connected to the soft
 * serial port, the function calls the s_write function. 
 * 
 * The function then reads the response from the spectrometer. If the spectrometer is connected to the
 * 
 * @param spectrometer 0 for hard, 1 for soft
 * 
 * @return No response
 */
void echo_pckt(bool spectrometer){
    
    
    char buffer[9] = {0xF5,0xFA, 0xF1, 0x7F, 0, 1 ,0xAA,0,0};//, 0x04, 0x0A
    char buffer_resp[9] = {0xF5,0xFA, 0x8F, 0x7F, 0, 1, 0xAA, 0xFC, 0x58};
    int checksum = 0;
    for(int k = 0; k<7; k++){
        checksum += buffer[k];
    }
    checksum = -checksum;
    buffer[7] = checksum >> 8;
    buffer[8] = checksum;
    int response_length = 0;
    // “1024-channel spectrum”
    // if(type == 0){
    //     char buffer[8] = {0xF5,0xFA, 2, 1, 0, 0, 0xFE, 0x0E};
    //     // for(int i = 0; i < 8; i++){
    //     //     printf("%d\n", buffer[i]);
    //     // }
    //     // return;
    //     response_length = 3080;
    // }
    // // “1024-channel spectrum plus clear spectrum”
    // else if(type == 1){
    //     char buffer[8] = {0xF5,0xFA, 2, 2, 0, 0, 0xFE, 0x0D};
    //     response_length = 3080;
    // }
    // // “1024-channel spectrum plus Status”
    // else if(type == 2){
    //     char buffer[8] = {0xF5,0xFA, 2, 3, 0, 0, 0xFE, 0x0C};
    //     response_length = 3144;
    // }
    // // “1024-channel spectrum plus Status and clear”
    // else if(type == 3){
    //     char buffer[8] = {0xF5,0xFA, 2, 4, 0, 0, 0xFE, 0x0B};
    //     response_length = 3144;
    // }
    // else{
    //     printf("Incorrect Packet Type Requested");
    //     return;
    // }
    if(spectrometer == 1){
        struct h_packet soft;
        soft.buf = buffer;
        soft.size = 9;
        // buffer_print(buffer, 9);
        s_write(soft);
        char echo_resp[9] = {0,0,0,0,0,0,0,0,0};
        soft.buf = echo_resp;
        soft.size = 9;
        s_read(&soft);
        buffer_print(soft.buf, 9);
        for(int i = 0; i < 9; i++){
            printf("Expected:   %x      Got:    %x\n",buffer_resp[i], soft.buf[i]);
        }
    }
    if(spectrometer == 0){
        struct h_packet hard;
        hard.buf = buffer;
        hard.size = 9;
        // buffer_print(buffer, 9);
        h_write(hard);
        char echo_resp[9] = {0,0,0,0,0,0,0,0,0};
        hard.buf = echo_resp;
        hard.size = 9;
        h_read(&hard);
        buffer_print(echo_resp, 9);
        for(int i = 0; i < 9; i++){
            printf("Expected:   %d      Got:    %d\n",buffer_resp[i], echo_resp[i]);
        }
    }
    return;
}