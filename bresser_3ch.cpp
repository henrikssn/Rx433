#include "bresser_3ch.h"

namespace rx433 {
namespace {

struct RxMsg {
  std::vector<int> bits;
  
  uint16_t getInt(size_t offset, size_t len) {
    if (bits.size() < offset + len) return 0;
    uint16_t result = 0;
    for (int i = offset; i < offset+len; i++) {
      result = (result<<1) | bits.at(i);
    }
    return result;
  }

  int16_t getTemp() {
    uint16_t raw = getInt(12, 12);
    return ((int16_t) (raw << 4)) / 16;
  }

  uint16_t getHumidity() {
    return getInt(28, 8);
  }

  uint16_t getId() {
    return getInt(0, 8);
  }

  uint8_t getChannel() {
    return getInt(10, 2);
  }

  uint8_t getBattery() {
    return getInt(8, 1);
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
      msg.bits.push_back(0);
    } else if (p.state && p.delta_us > 3*pulse_len_us_ && p.delta_us < 5*pulse_len_us_) {
      msg.bits.push_back(1);
    } else if (!p.state && p.delta_us > pulse_len_us_/2 && p.delta_us*2 < 3*pulse_len_us_) {
    } else {
      if (i > 5) {
        Serial.printf("Dropping at pulse #%d (%d): %c %dus\n", i, msg.bits.size(), p.state ? 'H' : 'L', p.delta_us);
      }
      return false;
    }
  }
  for (int i = 0; i < msg.bits.size(); i++) {
    Serial.print(msg.bits.at(i));
    if (i%4==3) Serial.print(" ");  
  }
  if (msg.bits.size() < 36) {
    Serial.println();
    return false;
  }
  Serial.print(" ");
  Serial.printf("%d / %d", msg.getTemp(), msg.getHumidity());
  Serial.println();
  char jsonString[100];
  snprintf(jsonString, 100, "{\"id\":%d, \"channel\": %d, \"battery\": %d, \"temp\": %d, \"humidity\":%d}", msg.getId(), msg.getChannel(), msg.getBattery(), msg.getTemp(), msg.getHumidity());
  char topic[50];
  snprintf(topic, 50, "%s/%d", sensor_topic_, msg.getId());
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(jsonString);

  bool success = false;
  for (int i = 0; !success && i < 10; ++i) {
    success = mqtt_client_->publish(topic, jsonString);
    delay(i+1);
  }
  return success;
}

} // namespace rx433
