#ifndef Rx433_h
#define Rx433_h

// TODO: Make this compile on AVR platforms (this is ESP8266/ESP32 only
//       for now).
#include <Arduino.h>
#include <list>
#include <vector>

// Add this to your sketch for the library to write unhandled packets
// to Serial port. Useful for developing new subclasses of Handler.
//#define DEBUG_PRINT_PULSES

namespace rx433 {

struct Pulse {
  // time_us is the result of microseconds() at the time the packet
  // was recieved.
  // delta_us is the difference to the previous time_us at the time
  // the packet was recieved.
  uint32_t time_us, delta_us;
  // state is true iff the pin was high.
  bool state;
 bool operator==(const Pulse&) const;
};

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

// AddHandler adds a new handler to the supported list of handlers
// and should be called before Setup.
void AddHandler(Handler*);
// Setup should be called once in Arduino setup() function.
void Setup(int rxPin);
// Loop should be called in Arduino loop() function.
void Loop();

} // namespace rx433

// rx433_internal contains implementation specifics and should
// not be relied upon. We will break you without notice.
namespace rx433_internal {

using namespace rx433;

constexpr int kMinPulseLen = 50;
constexpr int kMaxPulseLen = 200;
constexpr int kGlitchUs = 200;

void rxISR();
bool IsSync(const Pulse&);
bool Handle(std::vector<Pulse>);
std::vector<Pulse> FilterGlitches(const std::vector<Pulse>&);

} // namespace rx433_internal

#endif // Rx433_h
