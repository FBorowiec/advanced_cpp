load("@rules_cc//cc:defs.bzl",  "cc_test",  "cc_library")

cc_test(
  name = "ex1",
  srcs = ["ex1.cpp"],
  deps = [
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ]
)

cc_test(
  name = "ex2",
  srcs = ["ex2.cpp"],
  deps = [
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ]
)

cc_test(
  name = "ex3",
  srcs = ["ex3.cpp"],
  deps = [
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ]
)

cc_test(
  name = "ex4",
  srcs = ["ex4.cpp"],
  deps = [
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ]
)

cc_library(
  name = "map",
  srcs = ["map/customer.cpp",],
  hdrs = glob(["map/*.hpp"]),
)

cc_test(
  name = "map_test",
  srcs = ["map/map.cpp"],
  deps = [
    ":map",
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ]
)

cc_test(
  name = "specexe",
  srcs = ["specexe.cpp"],
  deps = [
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ]
)

cc_test(
  name = "sharedptrloop",
  srcs = ["sharedptrloop.cpp"],
  deps = [
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ]
)
