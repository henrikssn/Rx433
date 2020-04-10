#ifndef Rx433_h
#define Rx433_h

#include <Arduino.h>
#include <list>
#include <vector>

#define RX_PIN 4

#define DEBUG_PRINT_PULSES

static int last_sync_us = 0;

namespace rx433 {

struct Pulse {
  uint32_t time_us, delta_us;
  bool state;
};

class Handler {
 public:
  virtual bool IsSync(const Pulse& p) = 0;
  virtual bool Handle(const std::vector<Pulse>& buf) = 0;
};

void AddHandler(Handler*);
void Setup(int rxPin);
void Loop();

} // namespace rx433

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