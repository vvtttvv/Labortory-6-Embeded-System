#include "debounce_input.h"

#include "io_driver.h"

void debounce_input_init(DebounceInput* d, uint8_t pin, uint16_t debounce_ms) {
  if (d == NULL) {
    return;
  }

  d->pin = pin;
  d->debounce_ms = debounce_ms;
  d->last_change_ms = 0;

  io_init_input_pullup(pin);
  d->last_raw = io_read_digital(pin);
  d->stable_state = d->last_raw;
}

uint8_t debounce_input_update(DebounceInput* d, uint32_t now_ms) {
  if (d == NULL) {
    return 1;
  }

  uint8_t raw = io_read_digital(d->pin);
  if (raw != d->last_raw) {
    d->last_raw = raw;
    d->last_change_ms = now_ms;
  }

  if ((uint32_t)(now_ms - d->last_change_ms) >= d->debounce_ms) {
    d->stable_state = d->last_raw;
  }

  return d->stable_state;
}