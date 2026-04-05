#include "io_driver.h"

void io_init_output(uint8_t pin) {
  pinMode(pin, OUTPUT);
}

void io_init_input_pullup(uint8_t pin) {
  pinMode(pin, INPUT_PULLUP);
}

void io_write_digital(uint8_t pin, uint8_t value) {
  digitalWrite(pin, value ? HIGH : LOW);
}

uint8_t io_read_digital(uint8_t pin) {
  return (uint8_t)(digitalRead(pin) == HIGH ? 1 : 0);
}

int io_read_analog(uint8_t pin) {
  return analogRead(pin);
}