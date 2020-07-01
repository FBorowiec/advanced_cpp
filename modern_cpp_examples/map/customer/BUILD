load("@rules_cc//cc:defs.bzl",  "cc_test",  "cc_library")

cc_library(
  name = "customer",
  hdrs = [
    "customer.hpp",
    "print.hpp",
    "timer.hpp",
  ],
  deps = [
    "//modern_cpp_examples/map/utils:bucket",
  ]
)

cc_test(
  name = "customer_test",
  srcs = ["customer.cpp"],
  deps = [
    ":customer",
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ]
)
