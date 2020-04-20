#include "Rx433.h"

#include <Arduino.h>
#include <vector>

namespace rx433 {
namespace {
  using namespace rx433_internal;
}

void AddHandler(Handler* handler) {
  handlers.push_back(handler);
}

void Setup(int rxPin) {
  rxPin_ = rxPin;
  pinMode(rxPin, INPUT);
  attachInterrupt(rxPin, rxISR, CHANGE);
}

void Loop() {
  if (pulse_stream_queue.empty()) return;
  std::vector<Pulse> buf =
    std::move(pulse_stream_queue.front());
  cli();
  pulse_stream_queue.pop_front();
  sei();
  Handle(buf);
}

} // namespace rx433

namespace rx433_internal {

bool ICACHE_RAM_ATTR IsSync(const Pulse& p) {
  for (Handler* handler : handlers) {
    if (handler->IsSync(p)) return true;
  }
  return false;
}

bool Handle(std::vector<Pulse> buf) {
#ifdef DEBUG_PRINT_PULSES
  Serial.printf("Pulse length: %d, sync pulse %d: %dus\n",
                buf.size(), buf[0].state, buf[0].delta_us);
  Serial.print("High: ");
  for (const Pulse p : buf)  {
    if (p.state) Serial.printf("%4d, ", p.delta_us);
  }
  Serial.println();
  Serial.print("Low:  ");
  for (const Pulse p : buf)  {
    if (!p.state) Serial.printf("%4d, ", p.delta_us);
  }
  Serial.println();
#endif // DEBUG_PRINT_PULSES
  for (Handler* handler : handlers) {
    if (handler->Handle(buf)) return true;
  }
  return false;
}

void ICACHE_RAM_ATTR rxISR() {
  static int last_changed = 0, sync_pulse_us = 0;
  static int last_state = 0;
  static std::vector<rx433::Pulse> pulse_stream;
  int now = micros();
  // Need to invert as p represents state before transition.
  Pulse p = {now, now - last_changed, digitalRead(rxPin_)};

  // Ignore very short glitches that we missed.
  if (p.state == last_state) return;
  last_state = p.state;

  // Streamed glich filter
  // Ignore any pulse (1 or 0) shorter than kGlitchUs.
  if (!pulse_stream.empty() && p.delta_us < kGlitchUs) {
    last_changed -= pulse_stream.back().delta_us;
    pulse_stream.pop_back();
    return;
  }
  last_changed = now;

  // Queue is full, drop pulse
  if (pulse_stream_queue.size() > 10) return;

  // Create pulse

  bool pulse_stream_done = false;
  if (pulse_stream.empty() && IsSync(p)) {
    sync_pulse_us = p.delta_us;
    pulse_stream.push_back(p);
  } else if (p.delta_us * 4 > 3 * sync_pulse_us) {
    pulse_stream_done = true;
  } else if (pulse_stream.size() > kMaxPulseLen) {
    pulse_stream_done = true;
  } else if (!pulse_stream.empty()) {
    // Put pulses after sync in stream.
    pulse_stream.push_back(p);
  }

  if (pulse_stream_done) {
    if (pulse_stream.size() >= kMinPulseLen) {
      // A complete pulse stream to handle.
      pulse_stream_queue.push_back(std::move(pulse_stream));
    }
    pulse_stream_done = false;
    // Need to reset also when it was move:d above.
    pulse_stream.clear();
    if (IsSync(p)) {
      sync_pulse_us = p.delta_us;
      pulse_stream.push_back(p);
    }
  }
}

} // namespace rx433_internal

namespace {
  using namespace rx433_internal;
}
