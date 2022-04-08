
cc_library(
  name = "combat",
  hdrs = ["combat.hh"],
  srcs = ["combat.cc"],
)

cc_test(
  name = "combat_test",
  srcs = ["combat_test.cc"],
  deps = [
    ":combat",
    "@com_google_googletest//:gtest_main",
  ],
)