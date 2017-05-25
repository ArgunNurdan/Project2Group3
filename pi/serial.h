#ifndef SERIAL_H
#define SERIAL_H

#include "rs232.h"
#include <stdint.h>

#define BD_RATE 115200

/** initilize serial port
 */
bool init_serial();
/** close serial port
 */
void close_serial();
/**
 * send next step data
 * @return 0 success, non-zero error
 */
int sendNextStep();
/**
 * send coordinate
 * @param x
 * @param y
 * @return 0 success, non-zero error
 */
#endif //Serial.h
