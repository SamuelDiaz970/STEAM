#ifndef SRC_HEADER_FILES_SPECTROMETER_H_
#define SRC_HEADER_FILES_SPECTROMETER_H_

#include "serial.h"
#include "buffer.h"
#include "stdbool.h"

void tx_rx_spectrum(bool spectrometer, char type);
void echo_pckt(bool spectrometer);

#endif /* SRC_HEADER_FILES_SPECTROMETER_H_ */