#include "io_driver.h"

#include <stdarg.h>
#include <stdio.h>

// On UNO it increases RAM/flash usage, I was forced to disable it to fit the code

// static int io_serial_putchar(char c, FILE* stream) {
//   (void)stream;
//   if (c == '\n') {
//     Serial.write('\r');
//   }
//   Serial.write((uint8_t)c);
//   return 0;
// }
//
// static int io_serial_getchar(FILE* stream) {
//   (void)stream;
//   while (!Serial.available()) {
//   }
//   return Serial.read();
// }
//
// static FILE io_serial_stdio;
//
// void io_bind_stdio_to_serial(void) {
//   fdev_setup_stream(&io_serial_stdio, io_serial_putchar, io_serial_getchar, _FDEV_SETUP_RW);
//   stdout = &io_serial_stdio;
//   stdin = &io_serial_stdio;
// }

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

void io_uart_begin(unsigned long baudrate) {
  Serial.begin(baudrate);
}

int io_printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  char buffer[80];
  int written = vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  Serial.print(buffer);
  return written;
}

int io_stdio_printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  char buffer[80];
  int written = vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  Serial.print(buffer);
  return written;
}

void io_uart_print_report(
  float setpoint_c,
  float measured_c,
  float humidity_pct,
  uint8_t output,
  uint8_t enabled
) {
  Serial.print(F("SetPoint:"));
  Serial.print(setpoint_c, 2);
  Serial.print(' ');
  Serial.print(F("Value:"));
  Serial.print(measured_c, 2);
  Serial.print(' ');
  Serial.print(F("Humidity:"));
  Serial.print(humidity_pct, 1);
  Serial.print(' ');
  Serial.print(F("Output:"));
  Serial.print(output);
  Serial.print(' ');
  Serial.print(F("Enable:"));
  Serial.println(enabled);
}