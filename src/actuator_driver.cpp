#include "actuator_driver.h"

#include "io_driver.h"

static uint8_t s_pin = 13;

void actuator_driver_init(uint8_t pin) {
  s_pin = pin;
  io_init_output(s_pin);
  io_write_digital(s_pin, 0);
}

void actuator_driver_set(uint8_t on) {
  io_write_digital(s_pin, on ? 1 : 0);
}