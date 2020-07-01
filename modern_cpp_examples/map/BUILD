load("@rules_cc//cc:defs.bzl",  "cc_test",  "cc_library")

cc_library(
  name = "map",
  hdrs = glob(["*.hpp"]),
)

cc_library(
  name = "bucket",
  hdrs = ["buckets.hpp"],
  visibility = ["//visibility:public",],
)

cc_test(
  name = "map_test",
  srcs = ["map.cpp"],
  deps = [
    ":map",
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ]
)

cc_test(
  name = "customer_test",
  srcs = ["customer.cpp"],
  deps = [
    ":map",
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ]
)
