#ifndef Handler_h
#define Handler_h

#include <Arduino.h>
#include <vector>

#include "pulse.h"

namespace rx433 {

// Handler should be implemented by a class that supports a specific
// modulation scheme.
class Handler {
 public:
  // IsSync takes a single Pulse and returns true if this is possibly
  // a valid sync pulse for this handler. Try to make this as specific
  // as possible as it improves performance of the interrupt handler.
  virtual bool IsSync(const Pulse& p) = 0;
  // Handle takes a vector of pulses and demodulates them, returning
  // true if successful. If this returns false, the library will give
  // the pulse vector to the next handler.
  virtual bool Handle(const std::vector<Pulse>& buf) = 0;
};

} // namespace rx433

#endif // Handler_h