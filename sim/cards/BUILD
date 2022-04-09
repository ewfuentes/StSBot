
cc_library(
  name = "block",
  hdrs = ["block.hh"],
  deps = ["//sim:combat",],
)

cc_test(
  name = "block_test",
  srcs = ["block_test.cc"],
  deps = [
    ":block",
    "@com_google_googletest//:gtest_main",
  ],
)