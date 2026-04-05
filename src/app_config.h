#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// Pins are selected to work on Arduino Uno and Mega.
#define PIN_DHT 7
#define PIN_POT A0
#define PIN_TILT 3
#define PIN_ACTUATOR_LED 13

#define DHT_TYPE 11

// Set to 1 only if tilt switch is physically connected to PIN_TILT.
#define USE_TILT_ENABLE 0

// Temperature setpoint range in Celsius.
#define SETPOINT_MIN_C 18.0f
#define SETPOINT_MAX_C 35.0f

// Hysteresis half-band in Celsius.
#define HYSTERESIS_HALF_BAND_C 0.8f

// Task periods in milliseconds.
#define PERIOD_ACQUIRE_MS 2000
#define PERIOD_CONTROL_MS 2500
#define PERIOD_ACTUATE_MS 2000
#define PERIOD_REPORT_MS 1000

#endif