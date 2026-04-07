#ifndef SETPOINT_INPUT_H
#define SETPOINT_INPUT_H

#include <Arduino.h>

void setpoint_input_init(uint8_t pot_pin, float min_c, float max_c);
float setpoint_input_read_c(void);

#endif