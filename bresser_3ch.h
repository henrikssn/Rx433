#ifndef Bresser3ch_h
#define Bresser3ch_h

#include <PubSubClient.h>

#include "handler.h"
#include "pulse.h"

#define PULSE_LEN_US 500

namespace rx433 {

class Bresser3chHandler : public Handler {
 public:
  Bresser3chHandler(PubSubClient& client, const char* topic) : mqtt_client_(&client), sensor_topic_(topic) {}
  bool IsSync(const Pulse& p) override;
  bool Handle(const std::vector<Pulse>& buf) override;
 private:
  const int pulse_len_us_ = 500;
  const char* sensor_topic_;
  PubSubClient* mqtt_client_;
};

} // namespace rx433

#endif // Bresser3ch_h
