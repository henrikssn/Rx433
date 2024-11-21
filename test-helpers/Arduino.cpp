#include "Arduino.h"

namespace arduino_test {
  uint64_t currentTimeUs = 0;
  bool pinStates[24] = {};
} // namespace arduino_test

void cli() {}
void sei() {}

void attachInterrupt(uint8_t, void (*)(void), int mode) {}
void detachInterrupt(uint8_t) {}

void pinMode(uint8_t, uint8_t) {}
void digitalWrite(uint8_t, uint8_t) {}
int digitalRead(uint8_t pin) {
  return arduino_test::pinStates[pin];
};
int analogRead(uint8_t) {return 0;}
void analogReference(uint8_t mode) {}
void analogWrite(uint8_t, int) {}

unsigned long millis(void) {
  return arduino_test::currentTimeUs / 1000;
}

unsigned long micros(void) {
  return arduino_test::currentTimeUs;
}

void delay(unsigned long) {}
void delayMicroseconds(unsigned int us) {}
