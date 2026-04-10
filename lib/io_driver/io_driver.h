#ifndef IO_DRIVER_H
#define IO_DRIVER_H

#include <Arduino.h>

void io_init_output(uint8_t pin);
void io_init_input_pullup(uint8_t pin);
void io_write_digital(uint8_t pin, uint8_t value);
uint8_t io_read_digital(uint8_t pin);
int io_read_analog(uint8_t pin);
void io_uart_begin(unsigned long baudrate);
int io_printf(const char* fmt, ...);
int io_stdio_printf(const char* fmt, ...);
void io_uart_print_report(
	float setpoint_c,
	float measured_c,
	float humidity_pct,
	uint8_t output,
	uint8_t enabled
);

#endif