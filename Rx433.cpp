#include "Rx433.h"

#include <Arduino.h>
#include <vector>

namespace rx433 {
namespace {
  using namespace rx433_internal;

  static int rx_pin_;
  static std::vector<rx433::Handler*> handlers;
  static std::list<std::vector<rx433::Pulse>> pulse_stream_queue;
}

void AddHandler(Handler* handler) {
  handlers.push_back(handler);
}

void Setup(int rxPin) {
  rx_pin_ = rxPin;
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
  buf = FilterGlitches(buf);
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
  static int last_changed = 0;
  static uint32_t sync_pulse_us = 0;
  static std::vector<rx433::Pulse> pulse_stream;
  bool pulse_stream_done = false;
  if (pulse_stream_queue.size() > 10) return;
  int now = micros();
  // Need to invert as p represents state before transition.
  Pulse p = {static_cast<uint32_t>(now),
             static_cast<uint32_t>(now-last_changed),
             static_cast<bool>(digitalRead(rx_pin_))};
  if (pulse_stream.empty() && IsSync(p)) {
    sync_pulse_us = p.delta_us;
    pulse_stream.push_back(p);
  } else if (p.delta_us * 4 > 3 * sync_pulse_us) {
    pulse_stream_done = true;
  } else {
    if (pulse_stream.size() > kMaxPulseLen) {
      pulse_stream_done = true;
    } else if (!pulse_stream.empty()) {
      pulse_stream.push_back(p);
    }
  }
  if (pulse_stream_done && pulse_stream.size() > kMinPulseLen) {
    pulse_stream_queue.push_back(std::move(pulse_stream));
    pulse_stream_done = false;
    // Need to reset as it was move:d above.
    pulse_stream.clear();
    if (IsSync(p)) {
      sync_pulse_us = p.delta_us;
      pulse_stream.push_back(p);
    }
  }
  last_changed = now;
}

std::vector<Pulse> FilterGlitches(const std::vector<Pulse>& pulses) {
  if (pulses.empty()) return pulses;
  std::vector<Pulse> result;
  result.reserve(pulses.size());
  result.push_back(pulses.at(0));
  for (unsigned int i = 1; i < pulses.size(); ++i) {
    const Pulse p = pulses[i];
    if (p.delta_us < kGlitchUs) {
      Pulse& last = result.back();
      last.delta_us += p.delta_us;
      if (++i < pulses.size()) {
        const Pulse next = pulses[i];
        last.delta_us += next.delta_us;
      }
    } else {
      result.push_back(p);
    }
  }
  if (result.size() != pulses.size()) {
#ifdef DEBUG_PRINT_PULSES
    Serial.printf("Dropped %d pulses in glitch filter.\n", pulses.size()-result.size());
#endif
    return result;
  } else {
    return pulses;
  }
}

} // namespace rx433_internal

