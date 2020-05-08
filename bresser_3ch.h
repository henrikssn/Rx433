#ifndef Bresser3ch_h
#define Bresser3ch_h

#include "handler.h"
#include "pulse.h"

namespace rx433 {

class Bresser3chHandler : public Handler {
 public:
  bool IsSync(const Pulse& p) override;
  bool Handle(const std::vector<Pulse>& buf) override;

  struct Message {
    uint8_t address, channel;
    float temperature_c;
    uint8_t humidity;
    bool battery_ok;

    bool operator==(const Message other) {
      return address == other.address
        && channel == other.channel
        && temperature_c == other.temperature_c
        && humidity == other.humidity
        && battery_ok == other.battery_ok;
    }
  };

  void RegisterListener(std::function<void(const Message&)>);
  
 private:
  const int pulse_len_us_ = 500;
  std::vector<std::function<void(const Message&)>> listeners;
};

} // namespace rx433

#endif // Bresser3ch_h
