#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Arduino.h"
#include "Rx433.h"

#include <stdint.h>
#include <vector>

namespace {
using namespace arduino_test;
using namespace rx433;
using namespace testing;
} // namespace

class MockHandler : public Handler {
 public:
  MOCK_METHOD1(IsSync, bool(const Pulse&));
  MOCK_METHOD1(Handle, bool(const std::vector<Pulse>&));
};


TEST(RxISR, AddHandlerDoesNotCrash) {
  std::vector<Pulse> expected;
  MockHandler mock;
  AddHandler(&mock);
}
