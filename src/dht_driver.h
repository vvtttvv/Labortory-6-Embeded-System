#ifndef DHT_DRIVER_H
#define DHT_DRIVER_H

#include <Arduino.h>

void dht_driver_init(uint8_t pin, uint8_t type);
uint8_t dht_driver_read(float* out_temp_c, float* out_humidity_pct);

#endif