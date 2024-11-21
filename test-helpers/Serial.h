/*
 * Some code here is borrowed from arduino-mock which is licensed under MIT.
 *
 * Copyright (c) 2014, Yasuki Ikeuchi <ikeyasu@gmail.com>
 * Copyright (c) 2013, Anders Arnholm <Anders@Arnholm.se>
 */
#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <cstddef>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class Serial_ {
  public:
    static size_t print(const char[]);
    static size_t print(char);
    static size_t print(unsigned char, int = DEC);
    static size_t print(int, int = DEC);
    static size_t print(unsigned int, int = DEC);
    static size_t print(long, int = DEC);
    static size_t print(unsigned long, int = DEC);
    static size_t print(double, int = 2);

    static size_t println(const char[]);
    static size_t println(char);
    static size_t println(unsigned char, int = DEC);
    static size_t println(int, int = DEC);
    static size_t println(unsigned int, int = DEC);
    static size_t println(long, int = DEC);
    static size_t println(unsigned long, int = DEC);
    static size_t println(double, int = 2);
    static size_t println(void);

    size_t write(uint8_t);
    size_t write(const char *str);
    size_t write(const uint8_t *buffer, size_t size);

    uint8_t begin(uint32_t);

    uint8_t available();
    uint8_t read();

    static void flush();

    static void printf(const char* format, ...) {};
};
extern Serial_ Serial;

#endif // SERIAL_H
