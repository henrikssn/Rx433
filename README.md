# Rx433
An arduino library for asynchronously demodulating OOK packets often sent by 433Mhz transmitters.

To use:

```c++

class MyHandler : public rx433::Handler {
  bool IsSync(const rx433::Pulse& p) override {
    // Return true if this is (possibly) a sync pulse.
  }
  
  bool Handle(const std::vector<Pulse>& buf) {
    // Insert demodulation code here. Return true if successful.
  }
}

void setup() {
  rx433::AddHandler(new MyHandler);
  rx433::Setup(4); // RX module on pin 4;
  // ...
}

void loop() {
  rx433::Loop();
  // ...
}

```
