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

struct TestData {
  bool firstPulse;
  uint16_t high[200];
  uint16_t low[200];
};

TestData testData{
  .firstPulse = 0,
  .high = {981, 1953, 1945, 1946, 976, 1952, 1949, 977, 1947, 972, 1954, 967, 982, 985, 987, 985, 1957, 1950, 984, 1952, 983, 986, 1961, 1972, 1955, 1953, 1953, 1955, 982, 980, 1947, 979, 1957, 982, 981, 1963, 3908, 978, 1949, 1946, 1948, 978, 1634, 166, 1951, 965, 1949, 977, 1971, 983, 980, 979, 967, 986, 1955, 1953, 982, 1968, 985, 984, 1955, 1953, 1970, 1952, 1968, 1952, 965, 984, 1953, 970, 1952, 982, 982, 1957, 3909, 980, 1949, 1955, 1951, 978, 1948, 1966, 980, 1948, 980, 1952, 982, 980, 984, 984, 981, 1958, 1954, 969, 1931, 982, 965, 1949, 1970, 1949, 1964, 1956, 1967, 982, 968, 1966, 972, 1954, 985, 975, 1954, 3886, 978, 1955, 1948, 1952, 979, 1967, 1951, 978, 1945, 980, 1967, 978, 982, 976, 972, 983, 1956, 1956, 979, 1953, 985, 984, 1954, 1955, 1968, 1954, 1970, 1950, 878, 978, 1955, 967, 1713, 52, 979, 986, 1955, 3911, 980, 1952, 1948, 1952, 977, 1949, 1965, 985, 1945, 976, 1958, 976, 984, 982, 979, 982, 1959, 1958, 973, 1953, 981, 964, 1951, 1971, 1954, 1966, 1949, 1951, 982, 979, 1951, 986, 1954, 980, 980, 1952, 6154, 90, 5582, 721},
  .low = {4399, 480, 502, 489, 499, 482, 502, 484, 484, 502, 484, 497, 494, 483, 471, 477, 489, 481, 495, 483, 478, 489, 474, 476, 478, 496, 479, 492, 481, 482, 500, 484, 476, 497, 478, 474, 495, 475, 499, 489, 499, 486, 482, 164, 486, 495, 498, 488, 477, 478, 482, 482, 495, 495, 475, 494, 481, 478, 481, 476, 477, 496, 480, 479, 481, 481, 499, 492, 478, 495, 492, 484, 480, 489, 482, 490, 484, 493, 482, 499, 485, 483, 482, 484, 499, 480, 482, 493, 482, 477, 479, 493, 476, 498, 489, 504, 496, 492, 487, 483, 482, 480, 479, 483, 480, 496, 489, 480, 493, 483, 480, 498, 501, 494, 483, 497, 482, 482, 483, 483, 502, 482, 484, 482, 480, 485, 490, 496, 476, 492, 482, 481, 492, 479, 475, 499, 477, 479, 480, 481, 585, 498, 481, 493, 494, 167, 505, 477, 495, 476, 491, 480, 500, 483, 500, 484, 484, 476, 484, 504, 477, 485, 492, 482, 482, 479, 492, 473, 490, 494, 482, 495, 493, 483, 476, 486, 481, 499, 482, 480, 498, 479, 476, 498, 479, 483, 127, 718, 64, 154, 492}
};

void highInterrupt(int i) {
    arduino_test::pinStates[4] = 1;
    arduino_test::currentTimeUs += testData.high[i];
}

void lowInterrupt(int i) {
      arduino_test::pinStates[4] = 0;
      arduino_test::currentTimeUs += testData.low[i];
}

void runSampleData() {
  using rx433_internal::rxISR;
  arduino_test::currentTimeUs = 1000000;
  arduino_test::pinStates[4] = !testData.firstPulse;
  rxISR();
  for (int i = 0; i < 200; ++i) {
    if (testData.firstPulse) {
      highInterrupt(i);
      rxISR();
      lowInterrupt(i);
      rxISR();
    } else {
      lowInterrupt(i);
      rxISR();
      highInterrupt(i);
      rxISR();
    }
  }
}

class MockHandler : public Handler {
 public:
  bool IsSync(const Pulse& p) override {
     return p.delta_us == 4399;
  }

  MOCK_METHOD1(Handle, bool(const std::vector<Pulse>&));
};


TEST(RxISR, WorksOnSampleData) {
  std::vector<Pulse> expected;
  MockHandler mock;
  AddHandler(&mock);
  EXPECT_CALL(mock, Handle(ContainerEq(expected)));
  runSampleData();
}
