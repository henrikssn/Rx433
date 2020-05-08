#include "bresser_3ch.h"

namespace rx433 {
namespace {

struct RxMsg {
  std::array<int, 74> bits;

  void push_back(int val) {
    bits.at(bits.size()) = val;
  }
  
  uint16_t getInt(size_t offset, size_t len) {
    if (bits.size() < offset + len) return 0;
    uint16_t result = 0;
    for (int i = offset; i < offset+len; i++) {
      result = (result<<1) | bits.at(i);
    }
    return result;
  }

  float getTemp() {
    uint16_t raw = getInt(12, 12);
    uint16_t temp_10c = ((int16_t) (raw << 4)) / 16;
    return temp_10c / 10.0;
  }

  uint8_t getHumidity() {
    return getInt(28, 8);
  }

  uint8_t getAddress() {
    return getInt(0, 8);
  }

  uint8_t getChannel() {
    return getInt(10, 2);
  }

  bool getBattery() {
    return getInt(8, 1);
  }

  Bresser3chHandler::Message ToMessage() {
    return {.address = getAddress(), .channel = getChannel(), .temperature_c = getTemp(), .humidity = getHumidity(), .battery_ok = getBattery()};
  }
};

} // namespace

bool Bresser3chHandler::IsSync(const Pulse& p) {
  return p.state && p.delta_us < 9*pulse_len_us_ && p.delta_us > 7*pulse_len_us_;
}

bool Bresser3chHandler::Handle(const std::vector<Pulse>& buf) {
  if (buf.empty() || !IsSync(buf.front()) || buf.size() != 74)
    return false;
  RxMsg msg;
  for (int i = 1; i < buf.size(); ++i) {
    const Pulse& p = buf[i];
    if (p.state && p.delta_us > 1*pulse_len_us_ && p.delta_us < 3*pulse_len_us_) {
      msg.push_back(0);
    } else if (p.state && p.delta_us > 3*pulse_len_us_ && p.delta_us < 5*pulse_len_us_) {
      msg.push_back(1);
    } else if (!p.state && p.delta_us > pulse_len_us_/2 && p.delta_us*2 < 3*pulse_len_us_) {
    } else {
#ifdef DEBUG_PRINT_PULSES
      if (i > 5) {
        Serial.printf("Dropping at pulse #%d (%d): %c %dus\n", i, msg.bits.size(), p.state ? 'H' : 'L', p.delta_us);
      }
#endif // DEBUG_PRINT_PULSES
      return false;
    }
  }
  Bresser3chHandler::Message message = msg.ToMessage();
#ifdef DEBUG_PRINT_PULSES
  for (int i = 0; i < msg.bits.size(); i++) {
    Serial.print(msg.bits.at(i));
    if (i%4==3) Serial.print(" ");  
  }
  if (msg.bits.size() < 36) {
    Serial.println();
    return false;
  }
  Serial.print(" ");
  Serial.printf("%.1f / %d", message.temperature_c, message.humidity);
  Serial.println();
  char jsonString[100];
  snprintf(jsonString, 100, "{\"id\":%d, \"channel\": %d, \"battery\": %d, \"temp\": %.1f, \"humidity\":%d}", message.address, message.channel, message.battery_ok, message.temperature_c, message.humidity);
  Serial.println(jsonString);
#endif // DEBUG_PRINT_PULSES
  for (const auto& listener : listeners) {
    listener(message);
  }
  return true;
}

void Bresser3chHandler::RegisterListener(std::function<void(const Message&)> listener) {
  listeners.push_back(listener);
}

} // namespace rx433
