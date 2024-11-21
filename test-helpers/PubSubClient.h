#ifndef PubSubClient_h
#define PubSubClient_h

#include <gmock/gmock.h>

class PubSubClient {
  public:
    MOCK_METHOD2(publish, int(std::string, std::string));
};

#endif // PubSubClient_h


