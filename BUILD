cc_library(
  name = "Rx433",
  srcs = ["Rx433.cpp"],
  hdrs = ["Rx433.h"],
  #deps = ["@arduino-mock//:arduino-mock"],
  deps = ["//test-helpers"],
  includes = ["test-helpers"]
)

cc_test(
  name = "Rx433_test",
  srcs = ["Rx433_test.cc"],
  deps = [":Rx433"])


