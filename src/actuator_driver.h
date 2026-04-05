#ifndef ACTUATOR_DRIVER_H
#define ACTUATOR_DRIVER_H

#include <Arduino.h>

void actuator_driver_init(uint8_t pin);
void actuator_driver_set(uint8_t on);

#endif