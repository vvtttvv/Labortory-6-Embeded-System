#include "hysteresis_control.h"

uint8_t hysteresis_control_step(
  float measured_c,
  float setpoint_c,
  HysteresisConfig cfg,
  uint8_t prev_output
) {
  float on_threshold = setpoint_c - cfg.half_band_c;
  float off_threshold = setpoint_c + cfg.half_band_c;

  if (measured_c < on_threshold) {
    return 1;
  }
  if (measured_c > off_threshold) {
    return 0;
  }

  return prev_output;
}