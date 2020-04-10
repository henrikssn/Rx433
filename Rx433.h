#ifndef Rx433_h
#define Rx433_h

// TODO: Make this compile on AVR platforms (this is ESP8266/ESP32 only
//       for now).
#include <Arduino.h>
#include <list>
#include <vector>

#include "handler.h"
#include "pulse.h"
#include "bresser_3ch.h"

// Add this to your sketch for the library to write unhandled packets
// to Serial port. Useful for developing new subclasses of Handler.
//#define DEBUG_PRINT_PULSES

namespace rx433 {

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
static int rxPin_ = 0;

static std::vector<rx433::Handler*> handlers;
static std::list<std::vector<rx433::Pulse>> pulse_stream_queue;

void rxISR();
bool IsSync(const Pulse&);
bool Handle(std::vector<Pulse>);
std::vector<Pulse> FilterGlitches(const std::vector<Pulse>&);

} // namespace rx433_internal

#endif // Rx433_h
