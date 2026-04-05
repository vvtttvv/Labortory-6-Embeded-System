#ifndef APP_STATE_H
#define APP_STATE_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

typedef struct {
  float measured_c;
  float measured_humidity_pct;
  float setpoint_c;
  float hysteresis_c;
  uint8_t control_enabled;
  uint8_t actuator_on;
  uint32_t sample_id;
} AppState;

extern AppState g_app_state;
extern SemaphoreHandle_t g_state_mutex;

#endif