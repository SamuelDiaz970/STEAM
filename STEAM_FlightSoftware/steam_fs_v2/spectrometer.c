
#include "spectrometer.h"
char hr_spec_response_buffer[rx_buff_length];
char sr_spec_response_buffer[rx_buff_length];

void tx_rx_spectrum(bool spectrometer, char type){
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
        return;
    }
    if(spectrometer == 1){
        struct s_packet soft;
        soft.buf = buffer;
        soft.size = 8;
        s_write(soft);
        soft.buf = sr_spec_response_buffer;
        soft.size = response_length;
        s_read(&soft);
    }
    if(spectrometer == 0){
        struct h_packet hard;
        hard.buf = buffer;
        hard.size = 8;
        h_write(hard);
        hard.buf = hr_spec_response_buffer;
        hard.size = response_length;
        h_read(&hard);
    }    
}