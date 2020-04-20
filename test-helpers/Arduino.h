/*
 * Some code here is borrowed from arduino-mock which is licensed under MIT.
 *
 * Copyright (c) 2014, Yasuki Ikeuchi <ikeyasu@gmail.com>
 * Copyright (c) 2013, Anders Arnholm <Anders@Arnholm.se>
 */

#ifndef ARDUINO_H
#define ARDUINO_H

#include "Serial.h"

#include <stdint.h>

#define ICACHE_RAM_ATTR

#define CHANGE 1
#define FALLING 2
#define RISING 3

void cli();
void sei();

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t pin);
int analogRead(uint8_t);
void analogReference(uint8_t mode);
void analogWrite(uint8_t, int);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);

#endif // ARDUINO_H
