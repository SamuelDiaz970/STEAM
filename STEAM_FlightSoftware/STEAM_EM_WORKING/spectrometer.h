#ifndef SRC_HEADER_FILES_SPECTROMETER_H_
#define SRC_HEADER_FILES_SPECTROMETER_H_

#include "serial.h"
#include "buffer.h"
#include "stdbool.h"

int tx_rx_spectrum(bool spectrometer, char type, struct h_packet *pckt);
void echo_pckt(bool spectrometer);

#endif /* SRC_HEADER_FILES_SPECTROMETER_H_ */