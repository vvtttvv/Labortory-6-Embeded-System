#include "dht_driver.h"

#include <DHT.h>

static DHT* s_dht = NULL;

uint8_t dht_driver_read(float* out_temp_c, float* out_humidity_pct) {
  if (s_dht == NULL || out_temp_c == NULL || out_humidity_pct == NULL) {
    return 0;
  }

  float h = s_dht->readHumidity(false);
  float t = s_dht->readTemperature(false, false);
  if (isnan(t) || isnan(h)) {
    return 0;
  }

  *out_temp_c = t;
  *out_humidity_pct = h;
  return 1;
}

void dht_driver_init(uint8_t pin, uint8_t type) {
  static DHT dht_instance(pin, type);
  s_dht = &dht_instance;

  pinMode(pin, INPUT_PULLUP);
  s_dht->begin();
  // DHT11 needs startup time after power-on for stable first sample.
  delay(1200);
}