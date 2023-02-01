#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <windows.h>

HANDLE open_serial_port(const char *device, uint32_t baud_rate);

int write_port(HANDLE port, uint8_t *buffer, size_t size);

SSIZE_T read_port(HANDLE port, uint8_t *buffer, size_t size);

#endif // !SERIAL_PORT_H_