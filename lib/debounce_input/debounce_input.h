#ifndef DEBOUNCE_INPUT_H
#define DEBOUNCE_INPUT_H

#include <Arduino.h>

typedef struct {
  uint8_t pin;
  uint8_t stable_state;
  uint8_t last_raw;
  uint32_t last_change_ms;
  uint16_t debounce_ms;
} DebounceInput;

void debounce_input_init(DebounceInput* d, uint8_t pin, uint16_t debounce_ms);
uint8_t debounce_input_update(DebounceInput* d, uint32_t now_ms);

#endif