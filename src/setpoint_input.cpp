#include "setpoint_input.h"

#include "io_driver.h"

static uint8_t s_pot_pin = A0;
static float s_min_c = 18.0f;
static float s_max_c = 35.0f;

void setpoint_input_init(uint8_t pot_pin, float min_c, float max_c) {
  s_pot_pin = pot_pin;
  s_min_c = min_c;
  s_max_c = max_c;
}

float setpoint_input_read_c(void) {
  int raw = io_read_analog(s_pot_pin);
  if (raw < 0) raw = 0;
  if (raw > 1023) raw = 1023;

  float ratio = (float)raw / 1023.0f;
  return s_min_c + ratio * (s_max_c - s_min_c);
}