#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#define PIN_DHT 7
#define PIN_POT A0
#define PIN_TILT 3
#define PIN_ACTUATOR_LED 13

#define DHT_TYPE 11

// For potentiometer
#define SETPOINT_MIN_C 18.0f
#define SETPOINT_MAX_C 35.0f

// Hysteresis half-band in Celsius.
#define HYSTERESIS_HALF_BAND_C 0.8f

#define PERIOD_ACQUIRE_MS 2000
#define PERIOD_CONTROL_MS 2500
#define PERIOD_ACTUATE_MS 2000
#define PERIOD_REPORT_MS 1000

// Conservative stack sizes for Uno (bytes in this FreeRTOS port variant)
#define STACK_CONTROL_BYTES 448
#define STACK_REPORT_BYTES 192

#endif