#ifndef HYSTERESIS_CONTROL_H
#define HYSTERESIS_CONTROL_H

#include <Arduino.h>

typedef struct {
  float half_band_c;
} HysteresisConfig;

uint8_t hysteresis_control_step(
  float measured_c,
  float setpoint_c,
  HysteresisConfig cfg,
  uint8_t prev_output
);

#endif