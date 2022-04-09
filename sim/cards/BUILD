
cc_library(
  name = "get_unique_id",
  hdrs = ["get_unique_id.hh"],
  srcs = ["get_unique_id.cc"],
)

cc_library(
  name = "defend",
  hdrs = ["defend.hh"],
  deps = [
  "//sim:combat",
  ":get_unique_id",
  ],
)

cc_test(
  name = "defend_test",
  srcs = ["defend_test.cc"],
  deps = [
    ":defend",
    "@com_google_googletest//:gtest_main",
  ],
)