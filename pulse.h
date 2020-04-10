#ifndef Pulse_h
#define Pulse_h

#include <Arduino.h>

namespace rx433 {

struct Pulse {
  // time_us is the result of microseconds() at the time the packet
  // was recieved.
  // delta_us is the difference to the previous time_us at the time
  // the packet was recieved.
  uint32_t time_us, delta_us;
  // state is true iff the pin was high.
  bool state;
 
 bool operator==(const Pulse& other) const {
   return time_us == other.time_us
      && delta_us == other.delta_us
      && state == other.state;
 }
};

} // namespace rx433

#endif // Pulse_h
