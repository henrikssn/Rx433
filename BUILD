cc_library(
  name = "Rx433",
  srcs = [
      "bresser_3ch.cpp",
      "Rx433.cpp",
  ],
  hdrs = [
      "bresser_3ch.h",
      "handler.h",
      "pulse.h",
      "Rx433.h",
  ],
  #deps = ["@arduino-mock//:arduino-mock"],
  deps = ["//test-helpers"],
  includes = ["test-helpers"],
)

cc_test(
  name = "Rx433_test",
  srcs = ["Rx433_test.cc"],
  deps = [
      ":Rx433",
      "@googletest//:gtest_main",
  ],
)


