#include "app_runtime.h"

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <semphr.h>
#include <util/delay.h>

#include "actuator_driver.h"
#include "app_config.h"
#include "app_state.h"
#include "dht_driver.h"
#include "hysteresis_control.h"
#include "io_driver.h"
#include "setpoint_input.h"

static void task_control(void* pv);
static void task_report(void* pv);

static uint8_t dht_reading_is_plausible(float t_c, float h_pct) {
  if (t_c < 5.0f || t_c > 50.0f) {
    return 0;
  }
  if (h_pct < 10.0f || h_pct > 95.0f) {
    return 0;
  }
  return 1;
}

void app_runtime_init(void) {
  io_uart_begin(115200);
  dht_driver_init(PIN_DHT, DHT_TYPE);
  setpoint_input_init(PIN_POT, SETPOINT_MIN_C, SETPOINT_MAX_C);

  actuator_driver_init(PIN_ACTUATOR_LED);
  g_state_mutex = xSemaphoreCreateMutex();

  if (g_state_mutex == NULL) {
    io_printf("ERR: mutex create failed\n");
    while (1) {
    }
  }

  g_app_state.hysteresis_c = HYSTERESIS_HALF_BAND_C;
  BaseType_t rc1 = xTaskCreate(task_control, "control", STACK_CONTROL_BYTES, NULL, 2, NULL);
  BaseType_t rc2 = xTaskCreate(task_report, "report", STACK_REPORT_BYTES, NULL, 1, NULL);

  if (rc1 != pdPASS || rc2 != pdPASS) {
    io_printf("ERR: xTaskCreate failed (RAM/stack)\n");
    while (1) {
      actuator_driver_set(1);
      _delay_ms(150);
      actuator_driver_set(0);
      _delay_ms(150);
    }
  }

  io_printf("INFO: started\n");
}

static void task_control(void* pv) {
  (void)pv;
  TickType_t last_wake = xTaskGetTickCount();
  HysteresisConfig cfg;
  uint8_t dht_fail_streak = 0;

  for (;;) {
    float measured_t = 0.0f;
    float measured_h = 0.0f;
    uint8_t dht_ok = dht_driver_read(&measured_t, &measured_h);
    if (dht_ok && !dht_reading_is_plausible(measured_t, measured_h)) {
      dht_ok = 0;
    }
    float setpoint = setpoint_input_read_c();

    uint8_t enabled = 1;

    if (xSemaphoreTake(g_state_mutex, pdMS_TO_TICKS(20)) == pdTRUE) {
      cfg.half_band_c = g_app_state.hysteresis_c;

      if (dht_ok) {
        g_app_state.measured_c = measured_t;
        g_app_state.measured_humidity_pct = measured_h;
        dht_fail_streak = 0;
      } else {
        dht_fail_streak++;
        if (dht_fail_streak == 1 || dht_fail_streak % 5 == 0) {
          io_printf("WARN: DHT read fail count=%u\n", dht_fail_streak);
        }
      }
      g_app_state.setpoint_c = setpoint;
      g_app_state.control_enabled = enabled;

      if (g_app_state.control_enabled) {
        g_app_state.actuator_on = hysteresis_control_step(
          g_app_state.measured_c,
          g_app_state.setpoint_c,
          cfg,
          g_app_state.actuator_on
        );
      } else {
        g_app_state.actuator_on = 0;
      }

      actuator_driver_set(g_app_state.actuator_on);
      g_app_state.sample_id++;

      xSemaphoreGive(g_state_mutex);
    } else {
      io_printf("ERR: CTRL mutex timeout\n");
    }

    vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(PERIOD_CONTROL_MS));
  }
}

static void task_report(void* pv) {
  (void)pv;
  TickType_t last_wake = xTaskGetTickCount();

  for (;;) {
    AppState copy;

    if (xSemaphoreTake(g_state_mutex, pdMS_TO_TICKS(20)) == pdTRUE) {
      copy = g_app_state;
      xSemaphoreGive(g_state_mutex);

      io_uart_print_report(
        copy.setpoint_c,
        copy.measured_c,
        copy.measured_humidity_pct,
        copy.actuator_on,
        copy.control_enabled
      );
    } else {
      io_printf("ERR: RPT mutex timeout\n");
    }

    vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(PERIOD_REPORT_MS));
  }
}
